#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <openssl/ssl.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include <krad/loop/loop.h>
#include <krad/app/types.h>
#include "internal.h"
#include "error.h"

struct kr_web {
  kr_loop *loop;
  kr_pool *pool;
  void *user;
  kr_web_event_cb *event_cb;
};

int path_event(kr_web_path *path, kr_web_event_type type) {
  int ret;
  kr_web_event event;
  event.path = path;
  event.type = type;
  event.user = path->web->user;
  if (path->event_cb) {
    event.user_path = path->user;
    ret = path->event_cb(&event);
  } else {
    ret = path->web->event_cb(&event);
  }
  return ret;
}

kr_web_path *web_path(kr_web *web) {
  kr_web_path *path;
  path = kr_pool_slice(web->pool);
  if (!path) return NULL;
  memset(path, 0, sizeof(*path));
  path->web = web;
  return path;
}

kr_loop *path_loop(kr_web_path *path) {
  return path->web->loop;
}

kr_loop *kr_web_loop(kr_web *web) {
  if (!web) return NULL;
  return web->loop;
}

kr_web_path_info *kr_web_path_info_get(kr_web_path *path) {
  if (!path) return NULL;
  return &path->info;
}

int kr_web_path_user_type_get(kr_web_path *path) {
  if (!path) return 0;
  return path->user_type;
}

int kr_web_path_user_type_set(kr_web_path *path, int type) {
  if (!path) return -1;
  path->user_type = type;
  return 0;
}

void *kr_web_path_user_get(kr_web_path *path) {
  if (!path) return NULL;
  return path->user;
}

int kr_web_path_user_set(kr_web_path *path, void *user) {
  if (!path) return -1;
  path->user = user;
  return 0;
}

int kr_web_path_handler_set(kr_web_path *path, kr_web_event_cb *handler) {
  if (!path) return -1;
  path->event_cb = handler;
  return 0;
}

int kr_web_path_remove(kr_web_path *path) {
  int ret;
  if (!path) return -1;
  path_event(path, KR_WEB_CLOSE);
  switch (path->info.type) {
    case KR_WEB_SERVER:
      ret = server_path_close(path);
      break;
    case KR_WEB_CLIENT:
      ret = client_path_close(path);
      break;
    default:
      printk("Interweb: remove unhandled path type");
      break;
  }
  ret = kr_pool_release(path->web->pool, path);
  return ret;
}

int kr_web_path_create(kr_web *web, kr_web_path_setup_info *setup,
 void *user, kr_web_event_cb *handler) {
  kr_web_path *path;
  if (!web) return -1;
  if (!setup) return -2;
  if (!user) return -3;
  path = web_path(web);
  if (!path) return -4;
  if (handler) path->event_cb = handler;
  path->user = user;
  path->info.type = setup->type;
  if (setup->type == KR_WEB_CLIENT) {
    return client_path_connect(path, &setup->client);
  } else if (setup->type == KR_WEB_SERVER) {
    return server_path_open(path, &setup->server);
  }
  return -5;
}

int kr_web_destroy(kr_web *web) {
  int i;
  kr_web_path *path;
  if (!web) return -1;
  printk("Web: Destroying");
  i = 0;
  while ((path = kr_pool_iterate_active(web->pool, &i))) {
    kr_web_path_remove(path);
  }
  kr_loop_destroy(web->loop);
  kr_pool_destroy(web->pool);
  printk("Web: Destroyed");
  return 0;
}

kr_web *kr_web_create(kr_web_setup *setup) {
  kr_web *web;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_loop_setup loop_setup;
  if (!setup) return NULL;
  if (setup->npaths < 1) return NULL;
  if (!setup->user) return NULL;
  if (!setup->event_cb) return NULL;
  printk("Interweb: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*web);
  pool_setup.size = sizeof(kr_web_path);
  pool_setup.slices = setup->npaths;
  pool = kr_pool_create(&pool_setup);
  if (!pool) return NULL;
  web = kr_pool_overlay_get(pool);
  memset(web, 0, sizeof(*web));
  web->pool = pool;
  web->user = setup->user;
  web->event_cb = setup->event_cb;
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_interweb");
  loop_setup.master = setup->master_loop;
  web->loop = kr_loop_create(&loop_setup);
  printk("Interweb: Created");
  return web;
}
