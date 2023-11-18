#include <krad/mem/pool.h>
#include "router2.h"

struct kr_router2 {
  kr_pool *path_pool;
};

int kr_router2_dispatch(kr_router2 *router, kr_crate *crate) {
  int i;
  int ret;
  kr_router_path *path;
  i = 0;
  ret = -1;
  while ((path = kr_pool_iterate_active(router->path_pool, &i))) {
    ret = path->disp(path->user, crate);
    if (ret) break;
  }
  return ret;
}

int kr_router2_add(kr_router2 *router, kr_router_path *path) {
  kr_router_path *new;
  if (!router || !path) return -1;
  new = kr_pool_slice(router->path_pool);
  if (new == NULL) return -1;
  *new = *path;
  return 0;
}

int kr_router2_destroy(kr_router2 *router) {
  if (!router) return -1;
  kr_pool_destroy(router->path_pool);
  return 0;
}

kr_router2 *kr_router2_create(kr_router2_setup *setup) {
  kr_router2 *router;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (setup == NULL) return NULL;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*router);
  pool_setup.size = sizeof(kr_router_path);
  pool_setup.slices = setup->npaths;
  pool = kr_pool_create(&pool_setup);
  router = kr_pool_overlay_get(pool);
  memset(router, 0, sizeof(*router));
  router->path_pool = pool;
  return router;
}
