#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include <krad/adapter/monitor.h>
#include <krad/compositor/types.h>
#include <krad/compositor/io.h>
#include <krad/adapter/adapter.h>
#include "xpdr.h"
#include "event.h"

#include "../adapter/adapters.c"

struct kr_xpdr_path {
  union {
    kr_adapter adapter;
    struct {
      kr_adapter *adapter;
      kr_adapter_path adapter_path;
      union {
        kr_mixer_path *mixer_port;
        kr_compositor_path *compositor_port;
      };
    } link;
  };
  kr_xpdr_path_mode mode;
  kr_xpdr_path_info info;
  const kr_adapter_spec *adapter_func;
  kr_xpdr_type adapter_type;
  void *user;
  kr_xpdr *xpdr;
};

struct kr_xpdr {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_pool *path_pool;
  kr_adapter_monitor *monitor;
  void *user;
  kr_xpdr_event_cb *event_cb;
  kr_fds_get_cb *fds_get_cb;
  kr_loop *master_loop;
};

static void adapter_event(kr_adapter_event *event);
static void run_event_full(kr_xpdr_path *path, kr_xpdr_event_type type,
 kr_patchset *ps);
static void run_event(kr_xpdr_path *path, kr_xpdr_event_type type);
static int link_destroy(kr_xpdr_path *path);
static int link_create(kr_xpdr_path *path, void *peer_user);
static int path_create(kr_xpdr *xpdr, kr_xpdr_path *ap,
 kr_xpdr_path_info *info, void *user, void *peer_user);
static void remove_adapter_links(kr_xpdr_path *ctx_path);

static void adapter_event(kr_adapter_event *event) {
  if (event->type == KR_ADAPTER_DISCONNECTED) {
    printk("Adapter Disconnected");
    return;
  }
  if (event->type == KR_ADAPTER_RECONNECTED) {
    printk("Adapter Reconnected");
    return;
  }
  if (event->type != KR_ADAPTER_PROCESS) {
    printk("Wasn't a process event");
    return;
  }
}

static void run_event_full(kr_xpdr_path *path, kr_xpdr_event_type type,
 kr_patchset *ps) {
  kr_xpdr *xpdr;
  kr_xpdr_event event;
  xpdr = path->xpdr;
  event.user = xpdr->user;
  event.type = type;
  event.path = path;
  event.user_path = path->user;
  if (type == KR_XPDR_PATCH) {
    event.patchset = ps;
  } else {
    if (type == KR_XPDR_CREATE) {
      event.info = path->info;
      kr_xpdr_path_info_get(path, &event.info);
    }
    event.patchset = NULL;
  }
  path->xpdr->event_cb(&event);
  path->user = event.user_path;
}

static void run_event(kr_xpdr_path *path, kr_xpdr_event_type type) {
  run_event_full(path, type, NULL);
}

static int link_destroy(kr_xpdr_path *path) {
  int ret;
  int r;
  ret = 0;
  if (path->mode == KR_AUDIO_IN || path->mode == KR_VIDEO_IN) {
    r = path->adapter_func->unlink(&path->link.adapter_path);
    if (r != 0) {
      printke("XPDR: Problem unlinking adapter path");
      ret += r;
    }
  }
  if (path->mode == KR_AUDIO_IN || path->mode == KR_AUDIO_OUT) {
    r = kr_mixer_remove(path->link.mixer_port);
    if (r != 0) {
      printke("XPDR: Problem removing mixer port");
      ret += r;
    }
  }
  if (path->mode == KR_VIDEO_IN || path->mode == KR_VIDEO_OUT) {
    r = kr_compositor_remove(path->link.compositor_port);
    if (r != 0) {
      printke("XPDR: Problem removing compositor port");
      ret += r;
    }
  }
  if (path->mode == KR_AUDIO_OUT || path->mode == KR_VIDEO_OUT) {
    r = path->adapter_func->unlink(&path->link.adapter_path);
    if (r != 0) {
      printke("XPDR: Problem unlinking adapter path");
      ret += r;
    }
  }
  return ret;
}

static int link_create(kr_xpdr_path *path, void *peer_user) {
  int ret;
  int r;
  kr_mixer_port_setup mp_setup;
  kr_compositor_port_setup cp_setup;
  ret = 0;
  path->link.adapter_path.info = &path->info;
  char text[4096];
  memset(text, 0, sizeof(text));
  ret = kr_xpdr_path_info_to_text(text, &path->info, sizeof(text) - 1);
  printk("XPDR: link create:\n\n%s\n", text);
  if (path->mode == KR_AUDIO_IN || path->mode == KR_AUDIO_OUT) {
    kr_mixer_port_setup_init(&mp_setup);
    if (path->mode == KR_AUDIO_IN) {
      mp_setup.info.type = KR_MXR_SOURCE;
    } else {
      mp_setup.info.type = KR_MXR_OUTPUT;
    }
    mp_setup.audio_user = path;
    mp_setup.control_user = peer_user;
    if (path->info.type == KR_JACK_IN || path->info.type == KR_JACK_OUT) {
      mp_setup.driver = 1;
      mp_setup.info.channels = path->info.jack_in.channels;
    }
    path->link.mixer_port = kr_mixer_port(path->xpdr->mixer, &mp_setup);
    if (path->link.mixer_port == NULL) {
      printke("XPDR: Creating mixer port returned NULL");
      ret += -1;
    }
    path->link.adapter_path.user = (void *)path->link.mixer_port;
    path->link.adapter_path.fd = kr_mixer_port_fd(path->link.mixer_port);
    path->link.adapter_path.read = (kr_avio *)kr_mixer_read;
    path->link.adapter_path.write = (kr_avio *)kr_mixer_write;
  }
  if (path->mode == KR_VIDEO_IN || path->mode == KR_VIDEO_OUT) {
    if (path->mode == KR_VIDEO_IN) {
      cp_setup.info.type = KR_COM_SOURCE;
      kr_compositor_source_info_init(&cp_setup.info.source);
    } else {
      cp_setup.info.type = KR_COM_OUTPUT;
      kr_compositor_output_info_init(&cp_setup.info.output);
    }
    cp_setup.frame_user = path;
    cp_setup.control_user = peer_user;
    path->link.compositor_port = kr_compositor_port(path->xpdr->compositor, &cp_setup);
    if (path->link.compositor_port == NULL) {
      printke("XPDR: Creating compositor port returned NULL");
      ret += -1;
    }
    path->link.adapter_path.user = (void *)path->link.compositor_port;
    path->link.adapter_path.fd = kr_compositor_port_fd(path->link.compositor_port);
    path->link.adapter_path.read = (kr_avio *)kr_compositor_read;
    path->link.adapter_path.write = (kr_avio *)kr_compositor_write;
  }
  r = path->adapter_func->link(path->link.adapter, &path->link.adapter_path);
  if (r != 0) {
    printke("XPDR: Adapter create link problem");
    ret += r;
  }
  printk("XPDR: link create done");
  return ret;
}

static int path_create(kr_xpdr *xpdr, kr_xpdr_path *ap,
 kr_xpdr_path_info *info, void *user, void *peer_user) {
  /*Start debug*/
  char string[8192];
  kr_xpdr_path_info_to_text(string, info, sizeof(string));
  printk("XPDR: path_create:\n%s\n", string);
  /*End debug*/
  int ret;
  kr_xpdr_path *path;
  path = kr_pool_slice(xpdr->path_pool);
  if (path == NULL) return -1;
  path->xpdr = xpdr;
  path->info = *info;
  path->user = user;
  if (ap) path->link.adapter = &ap->adapter;
  path->mode = xpdr_type_modes[path->info.type];
  path->adapter_type = adapter_path_adapter_type[path->info.type];
  path->adapter_func = &adapters[path->adapter_type];
  if (path->mode == KR_ADAPTER_CTX) {
    path->adapter.master_loop = xpdr->master_loop;
    path->adapter.info = &path->info;
    path->adapter.event_cb = adapter_event;
    path->adapter.user = path;
    if ((path->info.type < 1)
     || (path->info.type >= nadapter_types)) {
      printke("XPDR: Adapter type invalid.");
      ret = -2;
    } else {
      if (adapters[path->adapter_type].open == NULL) {
        printke("XPDR: Adapter type is not supported on this app.");
        ret = -3;
      } else {
        printk("XPDR: adapter context create");
        ret = path->adapter_func->open(&path->adapter);
      }
    }
  } else {
    ret = link_create(path, peer_user);
  }
  if (ret) {
    kr_pool_release(xpdr->path_pool, path);
    return -4;
  }
  run_event(path, KR_XPDR_CREATE);
  printk("XPDR: path create done");
  return ret;
}

static void remove_adapter_links(kr_xpdr_path *ctx_path) {
  int ret;
  int i;
  kr_xpdr *xpdr;
  kr_xpdr_path *path;
  xpdr = ctx_path->xpdr;
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    if (path->mode == KR_ADAPTER_CTX) continue;
    if (&ctx_path->adapter != path->link.adapter) continue;
    ret = kr_xpdr_remove(path);
    if (ret) printke("XPDR: Failure removing an adapter path");
  }
}

int kr_xpdr_path_info_get(kr_xpdr_path *path, kr_xpdr_path_info *info) {
  if (!path || !info) return -1;
  *info = path->info;
  return 0;
}

int kr_xpdr_ctl(kr_xpdr_path *path, kr_patchset *patchset) {
  int ret;
  if (!path || !patchset) return -1;
  printk("XPDR: control");
  if (path->mode != KR_ADAPTER_CTX && path->adapter_type == KR_AUX) {
    path->link.adapter_path.naux_fds = KR_AUX_NFDS;
    ret = path->xpdr->fds_get_cb(path->xpdr->user,
     path->link.adapter_path.aux_fds, path->link.adapter_path.naux_fds);
    if (ret) {
      printke("XPDR: AUX port did not get fds, this ... won't go well.. %d", ret);
      return -1;
    } else {
      printk("XPDR: AUX port got %d fds", path->link.adapter_path.naux_fds);
      ret = path->adapter_func->lctl(&path->link.adapter_path, patchset);
      if (ret) return ret;
    }
  }
  ret = kr_patchset_apply(&path->info, patchset);
  if (ret) {
    printke("XPDR: Patchset apply ret %d", ret);
    return ret;
  }
  patchset->on = &path->info;
  run_event_full(path, KR_XPDR_PATCH, patchset);
  return 0;
}

int kr_xpdr_remove(kr_xpdr_path *path) {
  int ret;
  ret = -1;
  kr_xpdr *xpdr;
  if (path == NULL) return -1;
  xpdr = path->xpdr;
  printk("XPDR: remove");
  if (path->mode == KR_ADAPTER_CTX) {
    remove_adapter_links(path);
    ret = path->adapter_func->close(&path->adapter);
    if (ret != 0) {
      printke("XPDR: Problem closing adapter ctx");
    }
  } else {
    ret = link_destroy(path);
  }
  run_event(path, KR_XPDR_DESTROY);
  kr_pool_release(xpdr->path_pool, path);
  return ret;
}

int kr_xpdr_link(kr_xpdr_path *path, kr_xpdr_path_info *info, void *user,
 void *peer_user) {
  int ret;
  if ((path == NULL) || (info == NULL)) return -1;
  ret = path_create(path->xpdr, path, info, user, peer_user);
  return ret;
}

int kr_xpdr_open(kr_xpdr *xpdr, kr_xpdr_path_info *info, void *user) {
  int ret;
  if ((xpdr == NULL) || (info == NULL)) return -1;
  ret = path_create(xpdr, NULL, info, user, NULL);
  return ret;
}

int kr_xpdr_monitor(kr_xpdr *xpdr, int on) {
  if (xpdr == NULL) return -1;
  if ((on != 0) && (on != 1)) return -2;
  if (on == 1) {
    if (xpdr->monitor != NULL) {
      printke("XPDR: Monitor was already enabled");
      return -3;
    }
    xpdr->monitor = kr_adapter_monitor_create();
    printk("XPDR: Enabled adapter monitor");
    kr_adapter_monitor_wait(xpdr->monitor, 0);
  } else {
    if (xpdr->monitor == NULL) {
      printke("XPDR: Monitor was already disabled");
      return -4;
    }
    kr_adapter_monitor_destroy(xpdr->monitor);
    xpdr->monitor = NULL;
    printk("XPDR: Disabled adapter monitor");
  }
  return 0;
}

int kr_xpdr_destroy(kr_xpdr *xpdr) {
  int i;
  int ret;
  kr_xpdr_path *path;
  if (xpdr == NULL) return -1;
  printk("XPDR: Destroying");
  kr_xpdr_monitor(xpdr, 0);
  i = 0;
  while ((path = kr_pool_iterate_active(xpdr->path_pool, &i))) {
    ret = kr_xpdr_remove(path);
    if (ret) printke("XPDR: Failure removing an adapter context");
  }
  kr_pool_destroy(xpdr->path_pool);
  printk("XPDR: Destroyed");
  return 0;
}

kr_xpdr *kr_xpdr_create(kr_xpdr_setup *setup) {
  kr_xpdr *xpdr;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (setup == NULL) return NULL;
  printk("XPDR: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_xpdr);
  pool_setup.size = sizeof(kr_xpdr_path);
  pool_setup.slices = setup->path_count;
  pool = kr_pool_create(&pool_setup);
  xpdr = kr_pool_overlay_get(pool);
  memset(xpdr, 0, sizeof(kr_xpdr));
  xpdr->path_pool = pool;
  xpdr->mixer = setup->mixer;
  xpdr->compositor = setup->compositor;
  xpdr->user = setup->user;
  xpdr->event_cb = setup->event_cb;
  xpdr->fds_get_cb = setup->fds_get_cb;
  xpdr->master_loop = setup->master_loop;
  printk("XPDR: Created");
  kr_adapters_debug_info();
  return xpdr;
}
