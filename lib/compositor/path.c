#include <unistd.h>
#include <sys/eventfd.h>
#include <krad/app/debug.h>
#include <krad/app/notice.h>
#include <cairo.h>
#include "path.h"

typedef enum {
  PATH_UNUSED,
  PATH_ACTIVE,
  PATH_DESTROY
} comp_path_state;

struct kr_compositor_path {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
  comp_path_state state;
  int damage;
  kr_compositor *compositor;
  kr_convert converter;
  kr_perspective *perspective;
  union {
    kr_graph_elem elem;
    void *ptr;
  } g;
  int in_fd;
  int out_fd;
  int have_in;
  int have_frame;
  int have_out;
  kr_frame in;
  kr_frame frame;
  kr_frame out;
};

typedef struct {
  kr_compositor_path_info *info;
  void *control_user;
  void *frame_user;
  kr_compositor_path *from;
  kr_compositor_path *to;
} kr_compositor_path_setup;

static void path_release(kr_compositor_path *path);
static kr_vertex_type path_vertex_type(kr_compositor_path_type type);
static int path_info_check(kr_compositor_path_info *info);
static int path_setup_check(kr_compositor_path_setup *setup);
static void run_event_full(kr_compositor_path *path,
 kr_compositor_event_type type, kr_patchset *ps);
static void run_event(kr_compositor_path *path,
 kr_compositor_event_type type);
static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup);

#include "render.c"
#include "io.c"
#include "process.c"

static void path_release(kr_compositor_path *path) {
  int ret;
  if (path == NULL) {
    printke("Compositor: path release with null path");
    return;
  } else {
    printk("Compositor: path release");
  }
  if ((path->info.type == KR_COM_SOURCE)
   || (path->info.type == KR_COM_OUTPUT)) {
    ret = kr_loop_del(path->compositor->render, path->in_fd);
    if (ret) {
      printke("Compositor: couldn't remove path fd from loop");
    }
    if (path->in_fd != -1) {
      ret = kr_notifier_destroy(path->in_fd);
      if (ret) printke("Compositor: path frame out fd destroy fail!");
    }
    if (path->out_fd != -1) {
      ret = kr_notifier_destroy(path->out_fd);
      if (ret) printke("Compositor: path frame out fd destroy fail!");
    }
  }
  if (path->perspective != NULL) {
    kr_perspective_destroy(&path->perspective);
  }
  kr_image_convert_clear(&path->converter);
  kr_graph_remove(path->compositor->graph, &path->g.elem);
  if (path->info.type == KR_COM_INPUT) {
    ret = kr_give_notice(path->compositor->graph_fd);
    if (ret) printke("Compositor: notify comp graph input removed failed");
  }
  kr_pool_release(path->compositor->path_pool, path);
}

static kr_vertex_type path_vertex_type(kr_compositor_path_type type) {
  switch (type) {
    case KR_COM_SOURCE:
      return KR_VERTEX_SOURCE;
    case KR_COM_OVERLAY:
      return KR_VERTEX_SOURCE;
    case KR_COM_BUS:
      return KR_VERTEX_BUS;
    case KR_COM_OUTPUT:
      return KR_VERTEX_OUTPUT;
    default:
      break;
  }
  return -1;
}

static int path_info_check(kr_compositor_path_info *info) {
  if ((info->type != KR_COM_OVERLAY)
   && (info->type != KR_COM_SOURCE)
   && (info->type != KR_COM_INPUT)
   && (info->type != KR_COM_BUS)
   && (info->type != KR_COM_OUTPUT)) {
    return -4;
  }
  /*
  if ((info->width == 0) || (info->height == 0)) {
    return -1;
  }
  */
  return 0;
}

static int path_setup_check(kr_compositor_path_setup *setup) {
  kr_compositor_path_info *info;
  info = setup->info;
  if (setup->frame_user == NULL) {
    /* FIXME HRMMM */
  }
  if (setup->control_user == NULL) {
    /* FIXME HRMMM */
  }
  return path_info_check(info);
}

static void run_event_full(kr_compositor_path *path,
 kr_compositor_event_type type, kr_patchset *ps) {
  kr_compositor_event event;
  event.user = path->compositor->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = type;
  if (type == KR_COM_PATCH) {
    event.patchset = ps;
  } else {
    if (type == KR_COM_CREATE) event.info = path->info;
    event.patchset = NULL;
  }
  path->compositor->event_cb(&event);
  path->control_user = event.user_path;
}

static void run_event(kr_compositor_path *path,
 kr_compositor_event_type type) {
  run_event_full(path, type, NULL);
}

static kr_compositor_path *path_create(kr_compositor *comp,
 kr_compositor_path_setup *setup) {
  int ret;
  kr_vertex_type vertex_type;
  kr_compositor_path *path;
  kr_compositor_path_type type;
  kr_event harness;
  if (!comp || !setup) return NULL;
  ret = path_setup_check(setup);
  if (ret) {
    printke("Compositor: path_setup_check failed: %d", ret);
    return NULL;
  }
  path = kr_pool_slice(comp->path_pool);
  if (path == NULL) {
    printke("Compositor: No free slices in pool");
    return NULL;
  }
  path->g.ptr = NULL;
  type = setup->info->type;
  if (type == KR_COM_INPUT) {
    path->g.elem = kr_graph_link(comp->graph, &setup->to->g.elem,
     &setup->from->g.elem, path);
  } else {
    vertex_type = path_vertex_type(setup->info->type);
    path->g.elem = kr_graph_vertex(comp->graph, vertex_type, path);
  }
  if (path->g.ptr == NULL) {
    kr_pool_release(comp->path_pool, path);
    return NULL;
  }
  if (type == KR_COM_SOURCE || type == KR_COM_OUTPUT) {
    path->in_fd = kr_notifier_create();
    if (path->in_fd < 0) printke("Compositor: path frame in fd FAIL");
    path->out_fd = kr_notifier_create();
    if (path->out_fd < 0) printke("Compositor: path frame out fd FAIL");
    if (path->in_fd != -1 && path->out_fd != -1) {
      harness.fd = path->in_fd;
      harness.user = path;
      harness.handler = port_event;
      harness.events = EPOLLIN;
      ret = kr_loop_add(comp->render, &harness);
    }
    if (ret || path->in_fd == -1 || path->out_fd == -1) {
      ret = kr_notifier_destroy(path->in_fd);
      if (ret) printke("Compositor: path frame in fd destroy fail!");
      ret = kr_notifier_destroy(path->out_fd);
      if (ret) printke("Compositor: path frame out fd destroy fail!");
      kr_graph_remove(comp->graph, &path->g.elem);
      kr_pool_release(comp->path_pool, path);
      return NULL;
    }
  }
  path->compositor = comp;
  path->info = *setup->info;
  path->frame_user = setup->frame_user;
  path->control_user = setup->control_user;
  path->damage = 0;
  path->have_in = 0;
  path->have_frame = 0;
  path->have_out = 0;
  kr_image_convert_init(&path->converter);
  path->state = PATH_ACTIVE;
  if (type == KR_COM_INPUT) {
    ret = kr_give_notice(path->compositor->graph_fd);
    if (ret) printke("Compositor: notify comp graph input added failed");
  }
  run_event(path, KR_COM_CREATE);
  return path;
}

int kr_compositor_ctl(kr_compositor_path *path, kr_patchset *patchset) {
  int ret;
  if (!path || !patchset) return -1;
  kr_patchset_apply(&path->info, patchset);
  ret = kr_give_notice(path->compositor->path_fd);
  if (ret) printke("Compositor: notify path patch failed");
  run_event_full(path, KR_COM_PATCH, patchset);
  return 0;
}

int kr_compositor_remove(kr_compositor_path *path) {
  uint64_t n;
  int ret;
  if (path == NULL) return -1;
  /*FIXME*/
  run_event(path, KR_COM_DESTROY);
  if ((path->info.type != KR_COM_SOURCE)
   && (path->info.type != KR_COM_OUTPUT)) {
    path_release(path);
  } else {
    path->state = PATH_DESTROY;
    n = 1;
    ret = write(path->in_fd, &n, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
      printke("Compositor: kr_compositor_remove writing to fd");
    }
  }
  return 0;
}

int kr_compositor_bus(kr_compositor *c, kr_compositor_path_info *i, void *user) {
  kr_compositor_path *path;
  kr_compositor_path_setup setup;
  if ((c == NULL) || (i == NULL) || (user == NULL)) return -1;
  setup.info = i;
  setup.control_user = user;
  setup.frame_user = NULL;
  path = path_create(c, &setup);
  if (path == NULL) return -2;
  return 0;
}

kr_compositor_path *kr_compositor_port(kr_compositor *com,
 kr_compositor_port_setup *port_setup) {
  kr_compositor_path_setup setup;
  if ((com == NULL) || (port_setup == NULL)) return NULL;
  setup.info = &port_setup->info;
  setup.control_user = port_setup->control_user;
  setup.frame_user = port_setup->frame_user;
  return path_create(com, &setup);
}

int kr_compositor_link(kr_compositor_path *to, kr_compositor_path *fr,
  kr_compositor_path_info *info, void *user) {
  kr_compositor_path_setup setup;
  kr_compositor_path *path;
  if (fr == NULL || to == NULL || info == NULL || user == NULL) return -1;
  if (fr->compositor != to->compositor) return -2;
  setup.info = info;
  setup.control_user = user;
  setup.frame_user = NULL;
  setup.from = fr;
  setup.to = to;
  path = path_create(to->compositor, &setup);
  if (path == NULL) return -2;
  return 0;
}

int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info) {
  if (!path || !info) return -1;
  *info = path->info;
  return 0;
}

size_t kr_compositor_path_size() {
  return sizeof(kr_compositor_path);
}
