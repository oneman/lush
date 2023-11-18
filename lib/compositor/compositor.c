#include <krad/loop/loop.h>
#include <krad/mem/pool.h>
#include <krad/draw/text.h>
#include <krad/draw/vector.h>
#include <krad/graph/graph.h>
#include <krad/compositor/compositor.h>
#include "io.h"

struct kr_compositor {
  kr_pool *text_pool;
  kr_pool *vector_pool;
  kr_pool *path_pool;
  void *user;
  kr_compositor_event_cb *event_cb;
  kr_graph *graph;
  kr_loop *render;
  FT_Library ftlib;
  struct {
    int path_fd;
    int graph_fd;
  };
};

#include "path.c"

static void overlays_free(kr_compositor *compositor);
static void overlays_create(kr_compositor *compositor);

static void overlays_free(kr_compositor *com) {
  int i;
  i = 0;
  void *overlay;
  while ((overlay = kr_pool_iterate_active(com->text_pool, &i))) {
    kr_text_clear(overlay);
    kr_pool_release(com->text_pool, overlay);
  }
  while ((overlay = kr_pool_iterate_active(com->vector_pool, &i))) {
    kr_pool_release(com->vector_pool, overlay);
  }
  kr_pool_destroy(com->vector_pool);
  kr_pool_destroy(com->text_pool);
  FT_Done_FreeType(com->ftlib);
}

static void overlays_create(kr_compositor *compositor) {
  kr_pool_setup setup;
  setup.shared = 0;
  setup.overlay_sz = 0;
  setup.size = kr_vector_sizeof();
  setup.slices = KR_COMP_VECTORS_MAX;
  compositor->vector_pool = kr_pool_create(&setup);
  setup.size = kr_text_sizeof();
  setup.slices = KR_COMP_TEXTS_MAX;
  compositor->text_pool = kr_pool_create(&setup);
  FT_Init_FreeType(&compositor->ftlib);
}

int kr_compositor_destroy(kr_compositor *com) {
  int ret;
  if (com == NULL) return -1;
  printk("Compositor: Destroying");
  kr_loop_destroy(com->render);
  overlays_free(com);
  kr_graph_destroy(com->graph);
  ret = kr_notifier_destroy(com->graph_fd);
  if (ret) printke("Compositor: graph notifier fd destroy fail!");
  ret = kr_notifier_destroy(com->path_fd);
  if (ret) printke("Compositor: path notifier fd destroy fail!");
  kr_pool_destroy(com->path_pool);
  printk("Compositor: Destroyed");
  return 0;
}

kr_compositor *kr_compositor_create(kr_compositor_setup *setup) {
  int ret;
  kr_compositor *com;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_loop_setup loop_setup;
  kr_graph_setup graph_setup;
  kr_event harness;
  if (setup == NULL) return NULL;
  printk("Compositor: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(kr_compositor);
  pool_setup.size = kr_compositor_path_size();
  pool_setup.slices = KR_COMP_PATHS_MAX;
  pool = kr_pool_create(&pool_setup);
  com = kr_pool_overlay_get(pool);
  memset(com, 0, sizeof(kr_compositor));
  com->path_pool = pool;
  com->user = setup->user;
  com->event_cb = setup->event_cb;
  overlays_create(com);
  memset(&graph_setup, 0, sizeof(graph_setup));
  graph_setup.use_locking = 1;
  com->graph = kr_graph_create(&graph_setup);
  com->graph_fd = kr_notifier_create();
  if (com->graph_fd < 0) printke("Compositor: graph notice fd FAIL");
  com->path_fd = kr_notifier_create();
  if (com->path_fd < 0) printke("Compositor: path notice fd FAIL");
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_compositor");
  loop_setup.priority = KR_PRIORITY_REALTIME;
  com->render = kr_loop_create(&loop_setup);
  harness.fd = com->graph_fd;
  harness.user = com;
  harness.handler = graph_event;
  harness.events = EPOLLIN;
  ret = kr_loop_add(com->render, &harness);
  if (ret) printke("Compositor: graph fd add");
  harness.fd = com->path_fd;
  harness.user = com;
  harness.handler = path_patch_event;
  harness.events = EPOLLIN;
  ret = kr_loop_add(com->render, &harness);
  if (ret) printke("Compositor: path fd add");
  printk("Compositor: Created");
  return com;
}
