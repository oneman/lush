#include <stdio.h>
#include <alloca.h>
#include <krad/app/server.h>

typedef struct {
  kr_router *router;
} kr_testapp;

static void path_print(kr_path *path);
static int test_path(char *str);
static int test_kr_path();
static int test_kr_router();

static void path_print(kr_path *path) {
  char *name;
  int len;
  int ret;
  if (path == NULL) {
    fprintf(stderr, "path_print called with null kr_path\n");
    return;
  }
  printf("printing %d steps\n", kr_path_steps(path));
  for (;;) {
    len = kr_path_cur_name(path, &name);
    printf("Component: (%d chars) %s\n", len, name);
    ret = kr_path_step(path);
    if (ret != 0) break;
  }
  len = kr_path_last(path, &name);
  printf("Last is (%d chars) %s\n", len, name);
}

static int test_path(char *str) {
  int ret;
  int len;
  kr_path *path;
  kr_path_alloca(path);
  len = strlen(str);
  printf("testing path %s\n", str);
  ret = kr_path_parse(path, str, len);
  if (ret != 0) {
    printf("kr_path_parse on %s returned %d\n", str, ret);
    return ret;
  }
  path_print(path);
  printf("rewind and again\n");
  kr_path_rewind(path);
  path_print(path);
  return 0;
}

static int test_kr_path() {
  char *str;
  int ret;
  str  = "/monkey/coconut";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/mixer/Music1/analog/drive";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/compositor/Video4";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/mixer/mic/volume/level";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/transponder/jackin";
  ret = test_path(str);
  if (ret != 0) return ret;
  return ret;
}

static kr_mount *find_mount(kr_testapp *app, char *name) {
  kr_mount *mnt;
  size_t len;
  len = strlen(name);
  mnt = kr_router_resolve(app->router, name, len);
  if (mnt) {
    printk("Resolved: %s -> %p %p", name, mnt->func, mnt->user);
    return mnt;
  } else {
    printk("Not Found: %s", name);
    return NULL;
  }
}

static int test_mounts(kr_testapp *app) {
  find_mount(app, "coconut");
  find_mount(app, "monkey");
  find_mount(app, "coconut");
  return 0;
}

static int unlink_routes(kr_testapp *app) {
  int ret;
  kr_mount *mnt;
  mnt = find_mount(app, "coconut");
  if (!mnt) return -1;
  ret = kr_router_unlink(app->router, mnt);
  return ret;
}

static int setup_mounts(kr_testapp *app) {
  kr_mount *mnt;
  kr_mount new;
  snprintf(new.name, sizeof(new.name), "coconut");
  new.user = "Coconut String";
  new.func = "FAKEFUNC";
  //new.method = KR_GET;
  mnt = kr_router_mount(app->router, &new);
  if (!mnt) return -1;
  return 0;
}

static int test_kr_router() {
  int ret;
  kr_router_setup setup;
  kr_testapp app;
  memset(&app, 0, sizeof(app));
  memset(&setup, 0, sizeof(setup));
  ret = 0;
  setup.routes_max = 32;
  setup.maps_max = 32;
  snprintf(setup.name, sizeof(setup.name), "Test");
  setup.user = "Bongo";
  app.router = kr_router_create(&setup);
  if (!app.router) return -1;
  ret = setup_mounts(&app);
  if (ret) return ret;
  ret = test_mounts(&app);
  if (ret) return ret;
  ret = unlink_routes(&app);
  if (ret) return ret;
  ret = test_mounts(&app);
  if (ret) return ret;
  ret = kr_router_destroy(app.router);
  return ret;
}

int main (int argc, char *argv[]) {
  int ret;
  if (0) {
    ret = test_kr_path();
  }
  ret = test_kr_router();
  return ret;
}
