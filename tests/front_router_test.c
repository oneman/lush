#include <stdio.h>
#include <string.h>
#include <krad/app/debug.h>
#include <krad/app/front_router.h>

typedef struct {
  kr_front_router *router;
  kr_front_route *monkeyroute;
} kr_testapp;

static kr_front_route *resolve(kr_testapp *app, char *name) {
  kr_front_route *mnt;
  size_t len;
  len = strlen(name);
  mnt = kr_front_resolve(app->router, name, len);
  if (mnt) {
    printk("Resolved: %s -> %p %p", name, mnt->func, mnt->user);
    return mnt;
  } else {
    printk("Not Found: %s", name);
    return NULL;
  }
}

static int test_routes(kr_testapp *app) {
  resolve(app, "coconut");
  resolve(app, "monkey");
  resolve(app, "coconut");
  return 0;
}

static int add_routes(kr_testapp *app) {
  kr_front_route *route;
  kr_front_route new;
  snprintf(new.str, sizeof(new.str), "coconut");
  new.len = strlen(new.str);
  new.user = "Coconut String";
  new.func = "FAKEFUNC";
  new.type = KR_FR_FULL;
  route = kr_front_add(app->router, &new);
  if (!route) return -1;
  return 0;
}

static int test_kr_front_router() {
  int ret;
  kr_front_router_setup setup;
  kr_testapp app;
  memset(&app, 0, sizeof(app));
  memset(&setup, 0, sizeof(setup));
  ret = 0;
  setup.nroutes = 32;
  app.router = kr_front_router_create(&setup);
  if (!app.router) return -1;
  ret = add_routes(&app);
  if (ret) return ret;
  ret = test_routes(&app);
  if (ret) return ret;
  ret = kr_front_router_destroy(app.router);
  return ret;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test_kr_front_router();
  return ret;
}
