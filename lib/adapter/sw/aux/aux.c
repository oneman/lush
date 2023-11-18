#include <krad/app/notice.h>
#include <krad/mem/pool.h>
#include "aux.h"

#define KR_AUX_PORT_NFRAMES 4

typedef struct {
  kr_pool *pool;
  kr_adapter *adapter;
  int frameno;
  union {
    kr_frame frame[KR_AUX_PORT_NFRAMES];
    kr_audio audio[KR_AUX_PORT_NFRAMES];
  };
} aux_port;

static void setup_audio(kr_adapter_path *path);
static void setup_frames(kr_adapter_path *path);
static int aux_send_err(kr_event *e);
static int aux_recv(kr_event *e);
static int mc_event(kr_event *e);
static int map_mem(kr_adapter_path *path);
static int close_port(kr_adapter_path *path);

static void setup_audio(kr_adapter_path *path) {
  if (!path) return;
  int i;
  int a;
  int ret;
  kr_audio_setup setup;
  aux_port *port;
  port = (aux_port *)path->handle;
  a = 0;
  i = 0;
  while ((setup.mem = kr_pool_iterate_active(port->pool, &i))) {
    setup.audio = &port->audio[a];
    setup.channels = KR_AUX_KLUDGE_NCHANNELS;
    setup.rate = 48000;
    setup.count = KR_AUX_KLUDGE_PERIOD_NFRAMES;
    ret = kr_audio_init(&setup);
    if (ret) printk("AUX Port: setup audio");
    a++;
  }
  printk("AUX Port: setup %d periods", a);
}

static void setup_frames(kr_adapter_path *path) {
  int i;
  int ret;
  int f;
  kr_image_setup setup;
  aux_port *port;
  port = (aux_port *)path->handle;
  f = 0;
  i = 0;
  while ((setup.mem = kr_pool_iterate_active(port->pool, &i))) {
    port->frame[f].type = KR_IMAGE_RAW;
    setup.img = &port->frame[f].image;
    setup.info.w = KR_AUX_KLUDGE_WIDTH;
    setup.info.h = KR_AUX_KLUDGE_HEIGHT;
    setup.info.fmt = KR_AUX_KLUDGE_FMT;
    ret = kr_image_init2(&setup);
    if (ret) printk("AUX Port: setup frames ret %d", ret);
    port->frame[f].image.release = NULL;
    port->frame[f].image.owner = NULL;
    f++;
  }
  printk("AUX Port: setup %d frames", f);
}

static int aux_send_err(kr_event *e) {
  int ret;
  int do_close;
  kr_adapter_path *path;
  ret = 0;
  do_close = 0;
  if (!e) return -1;
  path = (kr_adapter_path *)e->user;
  if (e->events & EPOLLIN) {
    printk("AUX: Send port got EPOLLIN ???");
  }
  if (e->events & EPOLLHUP) {
    printk("AUX: Port send fd got hangup");
    do_close = 1;
  }
  if (e->events & EPOLLERR) {
    printk("AUX: Port send fd got error");
    do_close = 1;
  }
  if (do_close) close_port(path);
  return ret;
}

static int aux_recv(kr_event *e) {
  int ret;
  int expect;
  uint64_t n;
  int do_close;
  kr_adapter_path *path;
  kr_frame *frame;
  kr_audio *audio;
  aux_port *port;
  if (!e) return -1;
  path = (kr_adapter_path *)e->user;
  port = (aux_port *)path->handle;
  ret = 0;
  do_close = 0;
  if (e->events & EPOLLIN) {
    n = kr_take_notice(path->aux_fds[1]);
    if (n != 1) printke("Got AUX recv EPOLLIN notice %llu", n);
    if (0) {
      frame = &port->frame[port->frameno++ % KR_AUX_PORT_NFRAMES];
      ret = path->write(path->user, (kr_av *)frame);
      expect = 1;
    } else {
      audio = &port->audio[port->frameno++ % KR_AUX_PORT_NFRAMES];
      ret = path->write(path->user, (kr_av *)audio);
      expect = 0;
    }
    if (ret != expect) {
      printke("AUX Port: writing ret %d expect %d", ret, expect);
    } else {
      /*printk("AUX Port path->write ret 1 :)");*/
    }
  }
  if (e->events & EPOLLHUP) {
    printk("AUX: Port got hangup");
    do_close = 1;
  }
  if (e->events & EPOLLERR) {
    printk("AUX: Port got error");
    do_close = 1;
  }
  if (do_close) close_port(path);
  return ret;
}

static int mc_event(kr_event *e) {
  int ret;
  int do_close;
  uint64_t n;
  kr_adapter_path *path;
  kr_frame frame;
  //aux_port *port;
  ret = 0;
  do_close = 0;
  if (!e) return -1;
  path = (kr_adapter_path *)e->user;
  //port = (aux_port *)path->handle;
  if (e->events & EPOLLIN) {
    //printk("AUX: Compositor EPOLLIN");
    if (path->info->type == KR_AUX_VIDEO_IN
     || path->info->type == KR_AUX_AUDIO_IN) {
      ret = kr_take_notice(path->fd);
    } else {
      ret = path->read(path->user, (kr_av *)&frame);
    }
    if (ret != 1) {
      printke("AUX Port: read ret %d", ret);
    } else {
      /*printk("MC read ret 1 :)");*/
      n = kr_give_notice(path->aux_fds[0]);
      if (n) printke("Got AUX comp event EPOLLIN give notice %llu", n);
    }
  }
  if (e->events & EPOLLHUP) {
    printk("AUX: Port got hangup");
    do_close = 1;
  }
  if (e->events & EPOLLERR) {
    printk("AUX: Port got error");
    do_close = 1;
  }
  if (do_close) close_port(path);
  return ret;
}

static int map_mem(kr_adapter_path *path) {
  int ret;
  int fd;
  aux_port *port;
  kr_pool_setup setup;
  kr_image_info img_info;
  ret = 0;
  memset(&setup, 0, sizeof(setup));
  setup.shared = 1;
  fd = path->aux_fds[2];
  port = (aux_port *)path->handle;
  if (path->info->type == KR_AUX_VIDEO_IN
   || path->info->type == KR_AUX_VIDEO_OUT) {
    img_info.w = KR_AUX_KLUDGE_WIDTH;
    img_info.h = KR_AUX_KLUDGE_HEIGHT;
    img_info.fmt = KR_AUX_KLUDGE_FMT;
    setup.slices = KR_AUX_PORT_NFRAMES;
    setup.size = kr_image_sizeof(&img_info);
  } else if (path->info->type == KR_AUX_AUDIO_IN
   || path->info->type == KR_AUX_AUDIO_OUT) {
    setup.slices = KR_AUX_PORT_NPERIODS;
    setup.size = KR_AUX_KLUDGE_NCHANNELS * KR_AUX_KLUDGE_PERIOD_NFRAMES * sizeof(float);
  }
  port->pool = kr_pool_open(fd, &setup);
  if (!port->pool) {
    printke("AUX Port: failed to map mem pool");
    ret = -1;
  }
  return ret;
}

static int close_port(kr_adapter_path *path) {
  int i;
  int ret;
  int err;
  int fail;
  aux_port *port;
  fail = 0;
  port = (aux_port *)path->handle;
  for (i = 0; i < path->naux_fds; i++) {
    if (path->aux_fds[i] == -1) continue;
    printk("Closing FD %d: %d", i, path->aux_fds[i]);
    ret = kr_loop_close(port->adapter->loop, path->aux_fds[i]);
    if (ret) {
      err = errno;
      fail = 1;
      printke("AUX: error closing port fd: %s", strerror(err));
    }
    path->aux_fds[i] = -1;
  }
  path->naux_fds = 0;
  return fail;
}

int kr_aux_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  int i;
  aux_port *port;
  kr_event harness;
  kr_adapter *adapter;
  if (!path) return -1;
  if (!patchset) return -2;
  port = (aux_port *)path->handle;
  adapter = port->adapter;
  if (path->naux_fds < 3) {
    printk("Aux: adapter path controlled ?");
    return 0;
  }
  printk("AUX: Port Connected! I got %d fds", path->naux_fds);
  for (i = 0; i < path->naux_fds; i++) {
    if (path->aux_fds[i] == -1) continue;
    printk("Got FD %d: %d", i, path->aux_fds[i]);
  }
  map_mem(path);
  if (path->info->type == KR_AUX_VIDEO_IN
   || path->info->type == KR_AUX_VIDEO_OUT) {
    setup_frames(path);
  } else if (path->info->type == KR_AUX_AUDIO_IN
   || path->info->type == KR_AUX_AUDIO_OUT) {
    setup_audio(path);
  }
  harness.user = path;
  harness.fd = path->aux_fds[0];
  harness.handler = aux_send_err;
  harness.events = 0;
  kr_loop_add(adapter->loop, &harness);
  harness.user = path;
  harness.fd = path->aux_fds[1];
  harness.handler = aux_recv;
  harness.events = EPOLLIN;
  kr_loop_add(adapter->loop, &harness);
  harness.user = path;
  harness.fd = path->fd;
  harness.events = EPOLLIN;
  harness.handler = mc_event;
  kr_loop_add(adapter->loop, &harness);
  return 0;
}

int kr_aux_unlink(kr_adapter_path *path) {
  aux_port *port;
  if (!path) return -1;
  if (path->naux_fds) {
    printk("AUX: Port unlinking, closing fds");
    close_port(path);
  }
  port = (aux_port *)path->handle;
  free(port);
  printk("AUX: adapter path removed");
  return 0;
}

int kr_aux_link(kr_adapter *adp, kr_adapter_path *path) {
  aux_port *port;
  if (!adp) return -1;
  if (!path) return -2;
  port = kr_allocz(1, sizeof(*port));
  port->adapter = adp;
  path->handle = port;
  printk("Aux: adapter path created");
  return 0;
}

int kr_aux_ctl(kr_adapter *adp, kr_patchset *patchset) {
  if (!adp) return -1;
  if (!patchset) return -2;
  printk("AUX: adapter controlled");
  return 0;
}

int kr_aux_adapter_close(kr_adapter *adp) {
  int ret;
  if (!adp) return -1;
  ret = kr_loop_destroy(adp->loop);
  if (ret != 0) {
    printke("AUX: Adapter trouble unlooping");
  }
  adp->handle = NULL;
  printk("AUX: adapter destroyed");
  return 0;
}

int kr_aux_open(kr_adapter *adapter) {
  static int aux_adapter_count = 0;
  kr_loop_setup loop_setup;
  if (!adapter) return -1;
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_aux_%d",
   aux_adapter_count);
  loop_setup.master = adapter->master_loop;
  adapter->loop = kr_loop_create(&loop_setup);
  printk("AUX: adapter created");
  return 0;
}
