#if !defined(_app_router2_H)
# define _app_router2_H (1)

#include "common.h"
#include <krad/gen/info_functions.h>

typedef struct kr_router2 kr_router2;

typedef struct {
  int npaths;
} kr_router2_setup;

typedef int (kr_dispatcher)(void *user, kr_crate *crate);

typedef struct {
  kr_dispatcher *disp;
  void *user;
} kr_router_path;

int kr_router2_dispatch(kr_router2 *router, kr_crate *crate);
int kr_router2_add(kr_router2 *router, kr_router_path *path);
int kr_router2_destroy(kr_router2 *router);
kr_router2 *kr_router2_create(kr_router2_setup *setup);

#endif
