#include <krad/app/debug.h>
#include <krad/app/router2.h>
#include <krad/app/patch.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/mem/pool.h>
#include <krad/loop/loop.h>
#include <krad/web/web.h>
#include <krad/web/crate.h>
#include <krad/tasks/pool.h>
#include <krad/ops/media/media.h>
#include <krad/ops/media/pool.h>

#include <krad/tasks/pool.c>
#include <krad/ops/media/pool.c>
#include <krad/ops/media/media.c>

#define KR_OPS_COMMAND_MAX 512
#define KR_OPS_TASKS_MAX 64
#define KR_OPS_CLIENTS_MAX 64
#define KR_OPS_MEDIA_MAX 64
#define KR_OPS_TIMEOUT_SEC 30

typedef struct {
  kr_ops_info info;
  kr_media_pool *media;
  kr_task_pool *tasks;
  kr_pool *client_pool;
  kr_loop *loop;
  kr_app_server *app;
  kr_web *web;
  kr_router2 *router;
} kr_ops;

typedef struct {
  kr_web_path *path;
  kr_ops *server;
  kr_crate crate;
} kr_ops_client;

static int client_pack_crate(kr_ops_client *client, kr_crate *crate);
static int client_pack_crate_chunk(kr_ops_client *client, kr_crate *crate);
static int client_pack_all(kr_ops_client *client);
static int client_pack_all_media(kr_ops_client *client);
static int media_dispatch(void *user, kr_crate *crate);
static int task_dispatch(void *user, kr_crate *crate);
static int tasks_dispatch(void *user, kr_crate *crate);
static int web_client_pack_created(kr_web_client *client, char *addr,
 size_t addr_len);
static int web_client_pack_ok(kr_web_client *client);
static int web_client_pack_not_found(kr_web_client *client);
static int web_client_pack_error(kr_web_client *client, int err);
static void web_headers(kr_ops *server, kr_web_path *path);
static void web_data(kr_ops *server, kr_web_path *path);
static int web_event(kr_web_event *event);
static void web_client_close(kr_web_path *path);
static int client_destroy(kr_ops_client *client);
static kr_ops_client *client_create(kr_ops *server);
static int task_event(kr_task_event *event);
static int media_event(kr_media_event *event);
static int signal_event(kr_event *event);
static int handle_startup(kr_app_server *app_server, void *user);
static int handle_shutdown(void *user);

static int get_file(kr_web_client *client, char *name, size_t len) {
  static char *root = NULL;
  static int root_len;
  if (!root) {
    root = getenv("KRAD_WEB_ROOT");
    if (!root) {
      printke("Get File: No KRAD_WEB_ROOT env var");
      return -1;
    }
    root_len = strlen(root);
  }
  int ret;
  kr_http_header hdr;
  kr_http_response res;
  char filepath[512];
  int filepath_len;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  kr_file *file;
  uint8_t *file_data;
  size_t file_sz;
  if ((len > 3) && (memcmp(name, "/web", 4) == 0)) {
    name += 4;
    len -= 4;
  }
  if ((len == 0) || ((len == 1) && (name[0] == '/'))) {
    name = "/ops_index.html";
    len = strlen(name);
  }
  if ((root_len + len) >= sizeof(filepath)) return -1;
  filepath_len = snprintf(filepath, sizeof(filepath), "%.*s%.*s", root_len,
   root, (int)len, name);
  printk("Web Server: looking at this filepath: '%s'", filepath);
  file = kr_file_open(filepath);
  if (file) {
    file_sz = kr_file_size(file);
    printk("Yey! file size is %zu", file_sz);
    file_data = malloc(file_sz);
    ret = kr_file_read(file, file_data, file_sz);
  } else {
    printke("boo no file");
    return -1;
  }
  hdrsz = sizeof(hdrbuf);
  printk("Get file: %.*s", filepath_len, filepath);
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_http_length_header_init(&hdr, file_sz, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_http_response_connection_close(&res);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, file_data, file_sz);
  if (file) {
    kr_file_close(&file);
    free(file_data);
  }
  return ret;
}

static int client_pack_crate(kr_ops_client *client, kr_crate *crate) {
  int ret;
  kr_web_client *web_client;
  kr_http_response res;
  kr_http_header hdr;
  char json[8192 * 8];
  uint8_t hdrbuf[512];
  size_t sz;
  size_t hdrsz;
  kr_packing_plan plan;
  web_client = kr_web_path_get_client(client->path);
  if (!web_client) return -1;
  memset(&plan, 0, sizeof(plan));
  plan.json = json;
  plan.sz = sizeof(json);
  plan.format = KR_FORMAT_JSON_PAYLOAD;
  ret = kr_crate_pack(&plan, crate);
  if (ret < 0) {
    printke("Ops: couldn't kr_crate_pack");
    return -1;
  }
  sz = ret;
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_http_length_header_init(&hdr, sz, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(web_client, &res);
  ret = kr_web_client_pack(web_client, (uint8_t *)json, sz);
  return ret;
}

static int temp_kr_http_pack_chunk_size(uint8_t *buf, uint32_t sz, size_t max) {
  int ret;
  ret = snprintf((char *)buf, max, "%x\r\n", sz);
  return ret;
}

static int client_pack_crate_chunk(kr_ops_client *client, kr_crate *crate) {
  int ret;
  kr_web_client *web_client;
  char json[8192];
  uint8_t chunkbuf[32];
  size_t sz;
  kr_packing_plan plan;
  static const char *chunk_end = "\r\n";
  web_client = kr_web_path_get_client(client->path);
  if (!web_client) return -1;
  memset(&plan, 0, sizeof(plan));
  plan.json = json;
  plan.sz = sizeof(json);
  plan.format = KR_FORMAT_JSON_PAYLOAD;
  ret = kr_crate_pack(&plan, crate);
  if (ret < 0) {
    printke("Ops: couldn't kr_crate_pack");
    return -1;
  }
  sz = ret;
  ret = temp_kr_http_pack_chunk_size(chunkbuf, ret, sizeof(chunkbuf));
  ret = kr_web_client_pack(web_client, (uint8_t *)chunkbuf, ret);
  ret = kr_web_client_pack(web_client, (uint8_t *)json, sz);
  ret = kr_web_client_pack(web_client, (uint8_t *)chunk_end, 2);
  return ret;
}

static int client_pack_all(kr_ops_client *client) {
  int i;
  kr_crate *crate;
  kr_ops *server;
  int ret;
  int chunks;
  kr_task *task;
  kr_task_info *info;
  kr_web_client *web_client;
  kr_http_response res;
  kr_http_header hdr;
  static const char *chunk_trailer = "0\r\n\r\n";
  server = client->server;
  web_client = kr_web_path_get_client(client->path);
  if (!web_client) return -1;
  kr_http_response_init(&res, KR_HTTP_OK);
  hdr.key = "Transfer-Encoding";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "chunked";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  hdr.key = "Access-Control-Allow-Origin";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "*";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(web_client, &res);
  i = 0;
  chunks = 0;
  crate = &client->crate;
  /* FIXME */
  if (kr_task_pool_active(server->tasks) > 0) {
    ret = kr_web_client_pack(web_client, (uint8_t *)"6\r\n{\"t\": \r\n", 11);
    ret = kr_web_client_pack(web_client, (uint8_t *)"1\r\n[\r\n", 6);
    while ((task = kr_task_pool_iterate(server->tasks, &i))) {
      info = kr_task_get_info(task);
      snprintf(crate->address, sizeof(crate->address), "/task/%s", info->id);
      crate->addr_len = 42;
      crate->type = KR_TASK_INFO;
      crate->info.task_info = *info;
      if (chunks > 0) {
        ret = kr_web_client_pack(web_client, (uint8_t *)"2\r\n,\n\r\n", 7);
      }
      ret = client_pack_crate_chunk(client, crate);
      if (ret < 1) return -1;
      chunks++;
    }
    ret = kr_web_client_pack(web_client, (uint8_t *)"2\r\n]}\r\n", 7);
  }
  ret = kr_web_client_pack(web_client, (uint8_t *)chunk_trailer, 5);
  return 0;
}

static int client_pack_all_media(kr_ops_client *client) {
  int i;
  kr_crate *crate;
  kr_ops *server;
  int ret;
  int chunks;
  kr_media *media;
  kr_media_info *info;
  kr_web_client *web_client;
  kr_http_response res;
  kr_http_header hdr;
  static const char *chunk_trailer = "0\r\n\r\n";
  server = client->server;
  web_client = kr_web_path_get_client(client->path);
  if (!web_client) return -1;
  kr_http_response_init(&res, KR_HTTP_OK);
  hdr.key = "Transfer-Encoding";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "chunked";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  hdr.key = "Access-Control-Allow-Origin";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "*";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(web_client, &res);
  i = 0;
  chunks = 0;
  crate = &client->crate;
  /* FIXME */
  if (kr_media_pool_active(server->media) > 0) {
    ret = kr_web_client_pack(web_client, (uint8_t *)"6\r\n{\"m\": \r\n", 11);
    ret = kr_web_client_pack(web_client, (uint8_t *)"1\r\n[\r\n", 6);
    while ((media = kr_media_pool_iterate(server->media, &i))) {
      info = kr_media_get_info(media);
      snprintf(crate->address, sizeof(crate->address), "/media/%s", info->id);
      crate->addr_len = 43;
      crate->type = KR_MEDIA_INFO;
      crate->info.media_info = *info;
      if (chunks > 0) {
        ret = kr_web_client_pack(web_client, (uint8_t *)"2\r\n,\n\r\n", 7);
      }
      ret = client_pack_crate_chunk(client, crate);
      if (ret < 1) return -1;
      chunks++;
    }
    ret = kr_web_client_pack(web_client, (uint8_t *)"2\r\n]}\r\n", 7);
  }
  ret = kr_web_client_pack(web_client, (uint8_t *)chunk_trailer, 5);
  return 0;
}

static int media_dispatch(void *user, kr_crate *crate) {
  int ret;
  kr_ops *server;
  kr_media *media;
  kr_media_info *info;
  uuid_t id;
  media = NULL;
  uuid_clear(id);
  server = (kr_ops *)user;
  if (!server) return -1;
  if (!crate) return -1;
  if (crate->method != KR_GET) return 0;
  if ((crate->addr_len == 6) && (memcmp(crate->address, "/media", 6) == 0)) {
    printk("I would have said, hey get all media!");
    return 3;
  }
  if ((crate->method != KR_GET) && (crate->method != KR_PATCH)
   && (crate->method != KR_DELETE)) return 0;
  if ((crate->addr_len != 43) || (memcmp(crate->address, "/media/", 7) != 0)) {
    return 0;
  }
  ret = uuid_parse(crate->address + 7, id);
  if (ret != 0) return 0;
  media = kr_media_find(server->media, &id);
  if (media) {
    if (crate->method == KR_GET) {
      info = kr_media_get_info(media);
      crate->type = KR_MEDIA_INFO;
      crate->info.media_info = *info;
      return 1;
    }
  }
  return 0;
}

static int task_dispatch(void *user, kr_crate *crate) {
  int ret;
  kr_ops *server;
  kr_task *task;
  kr_task_info *info;
  kr_task_setup setup;
  uuid_t id;
  task = NULL;
  uuid_clear(id);
  server = (kr_ops *)user;
  if (!server) return -1;
  if (!crate) return -1;
  if (crate->method == KR_POST) {
    if (crate->type != KR_TASK_PARAMS_INFO) return 0;
    if (crate->addr_len != 5) return 0;
    if (memcmp(crate->address, "/task", 5) != 0) return 0;
    setup.info = &crate->info.task_params_info;
    setup.user = server;
    task = kr_task_create(server->tasks, &setup);
    if (!task) return 0;
    info = kr_task_get_info(task);
    snprintf(crate->address, sizeof(crate->address), "/task/%s", info->id);
    crate->addr_len = 42;
    kr_task_start(task);
    return 1;
  }
  if ((crate->method != KR_GET) && (crate->method != KR_PATCH)
   && (crate->method != KR_DELETE)) return 0;
  if ((crate->addr_len != 42) || (memcmp(crate->address, "/task/", 6) != 0)) {
    return 0;
  }
  ret = uuid_parse(crate->address + 6, id);
  if (ret != 0) return 0;
  task = kr_task_find(server->tasks, &id);
  if (task) {
    if (crate->method == KR_GET) {
      info = kr_task_get_info(task);
      crate->type = KR_TASK_INFO;
      crate->info.task_info = *info;
      return 1;
    }
    if (crate->method == KR_PATCH) {
      /*kr_task_ctl(task);*/
      return 1;
    }
    if (crate->method == KR_DELETE) {
      kr_task_destroy(server->tasks, task);
      return 1;
    }
  }
  return 0;
}

static int tasks_dispatch(void *user, kr_crate *crate) {
  kr_ops *server;
  server = user;
  if (!server) return -1;
  if (!crate) return -1;
  if (crate->method != KR_GET) return 0;
  if ((crate->addr_len == 6) && (memcmp(crate->address, "/tasks", 6) == 0))
   return 2;
  return 0;
}

static int web_client_pack_created(kr_web_client *client, char *addr,
  size_t addr_len) {
  int ret;
  kr_http_response res;
  kr_http_header hdr;
  char *body;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  size_t len;
  body = "Created!";
  len = strlen(body);
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_CREATED);
  kr_http_length_header_init(&hdr, len, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  hdr.key = "Location";
  hdr.key_len = strlen(hdr.key);
  hdr.value = addr;
  hdr.value_len = addr_len;
  kr_http_response_add_header(&res, &hdr);
  hdr.key = "Access-Control-Allow-Origin";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "*";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, (uint8_t *)body, len);
  return ret;
}

static int web_client_pack_ok(kr_web_client *client) {
  int ret;
  kr_http_response res;
  kr_http_header hdr;
  char *body;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  size_t len;
  body = "Success!";
  len = strlen(body);
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_http_length_header_init(&hdr, len, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  hdr.key = "Access-Control-Allow-Origin";
  hdr.key_len = strlen(hdr.key);
  hdr.value = "*";
  hdr.value_len = strlen(hdr.value);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, (uint8_t *)body, len);
  return ret;
}

static int web_client_pack_not_found(kr_web_client *client) {
  int ret;
  kr_http_response res;
  kr_http_header hdr;
  char *body;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  size_t len;
  body = "Not Found!";
  len = strlen(body);
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_NOT_FOUND);
  kr_http_length_header_init(&hdr, len, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, (uint8_t *)body, len);
  return ret;
}

static int web_client_pack_error(kr_web_client *client, int err) {
  int ret;
  kr_http_response res;
  kr_http_header hdr;
  char *body;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  size_t len;
  body = "Error!";
  len = strlen(body);
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_ERROR);
  kr_http_length_header_init(&hdr, len, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, (uint8_t *)body, len);
  return ret;
}

static void web_headers(kr_ops *server, kr_web_path *path) {
  int ret;
  kr_ops_client *ops_client;
  kr_web_client *web_client;
  kr_http_request *req;
  ops_client = client_create(server);
  if (ops_client == NULL) {
    return;
  }
  ops_client->crate.client = ops_client;
  ops_client->path = path;
  kr_web_path_user_set(path, ops_client);
  web_client = kr_web_path_get_client(path);
  if (web_client == NULL) {
    return;
  }
  req = kr_web_client_get_request(web_client);
  if (req == NULL) {
    return;
  }
  if (req->req_line.method == KR_HTTP_GET) {
    ret = get_file(web_client, req->req_line.path, req->req_line.path_len);
    if (ret == 0) return;
  }
  ret = kr_http_request_to_crate(&ops_client->crate, req);
  if (ret < 0) {
    return;
  }
  if ((req->req_line.method == KR_HTTP_GET)
   || (req->req_line.method == KR_HTTP_PATCH)
   || (req->req_line.method == KR_HTTP_DELETE)) {
    ret = kr_router2_dispatch(server->router, &ops_client->crate);
    if (ret == 3) {
      client_pack_all_media(ops_client);
    } else if (ret == 2) {
      client_pack_all(ops_client);
    } else if (ret == 1) {
      if (req->req_line.method == KR_HTTP_GET) {
        client_pack_crate(ops_client, &ops_client->crate);
      } else {
        web_client_pack_ok(web_client);
      }
    } else {
      web_client_pack_not_found(web_client);
    }
  } else {
    if (req->req_line.method != KR_HTTP_POST) {
      web_client_pack_error(web_client, 405);
    }
  }
}

static void web_data(kr_ops *server, kr_web_path *path) {
  int ret;
  char buf[8192 * 8];
  kr_ops_client *ops_client;
  kr_web_client *web_client;
  kr_crate *crate;
  //kr_packing_plan plan;
  ops_client = kr_web_path_user_get(path);
  web_client = kr_web_path_get_client(path);
  if (!ops_client || !web_client) return;
  crate = &ops_client->crate;
  ret = kr_web_client_unpack(web_client, (uint8_t *)buf, sizeof(buf));
  if (ret < 0) {
    return;
  }
  //memset(&plan, 0, sizeof(plan));
  //plan.format = KR_FORMAT_JSON_PAYLOAD;
  //plan.json = buf;
  //plan.len = ret;
  //ret = kr_crate_unpack(crate, &plan);
  buf[ret] = '\0';
  ret = kr_task_params_info_unpack_json(buf, &crate->info);
  if (ret < 0) {
    web_client_pack_error(web_client, 500);
    printke("Ops: couldn't kr_crate_unpack");
    return;
  }
  crate->type = KR_TASK_PARAMS_INFO;
  ret = kr_router2_dispatch(server->router, crate);
  if (ret == 1) {
    if (crate->method == KR_POST) {
      web_client_pack_created(web_client, crate->address, crate->addr_len);
    }
  } else {
    web_client_pack_error(web_client, 500);
  }
}

static int web_event(kr_web_event *event) {
  int ret;
  kr_ops *server;
  ret = 0;
  server = event->user;
  switch (event->type) {
    case KR_WEB_OPEN:
      printk("Ops: Open event from InterWeb");
      break;
    case KR_WEB_CLOSE:
      printk("Ops: Close event from InterWeb");
      if (kr_web_path_get_client(event->path)) {
        web_client_close(event->path);
      }
      break;
    case KR_WEB_HEADERS:
      printk("Ops: Headers event from InterWeb");
      web_headers(server, event->path);
      break;
    case KR_WEB_DATA:
      printk("Ops: Data event from InterWeb");
      web_data(server, event->path);
      break;
    default:
      printke("Ops: Unknown event from Interweb");
      ret = -1;
      break;
  }
  return ret;
}

static void web_client_close(kr_web_path *path) {
  int ret;
  kr_ops_client *client;
  client = kr_web_path_user_get(path);
  if (client != NULL) {
    ret = client_destroy(client);
    if (ret == -1) {
      printke("Ops: failed to destroy client");
    }
  }
}

static int client_destroy(kr_ops_client *client) {
  printk("Ops: client_destroy called");
  return kr_pool_release(client->server->client_pool, client);
}

static kr_ops_client *client_create(kr_ops *server) {
  kr_ops_client *client;
  printk("Ops: creating a new client");
  client = kr_pool_slice(server->client_pool);
  if (client == NULL) {
    printke("Ops: cannot create new ops client.");
    return NULL;
  }
  memset(client, 0, sizeof(*client));
  client->server = server;
  return client;
}

static int media_event(kr_media_event *event) {
  printk("media event %s happened!",
   kr_media_event_type_to_str(event->type));
  return 0;
}

static int task_event(kr_task_event *event) {
  printk("task event %s happened!",
   kr_task_event_type_to_str(event->type));
  return 0;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  char *name;
  kr_ops *server;
  struct signalfd_siginfo fdsi;
  server = (kr_ops *)event->user;
  name = server->info.sysname;
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      default:
        printk("%s: Got Signal %u", name, fdsi.ssi_signo);
    }
  }
  return 0;
}

static int handle_startup(kr_app_server *app_server, void *user) {
  int ret;
  kr_ops *server;
  kr_web_setup web_setup;
  kr_router2_setup router_setup;
  kr_task_pool_setup tasks_setup;
  kr_pool_setup cpool_setup;
  kr_media_pool_setup mpool_setup;
  server = (kr_ops *)user;
  printk("%s: Startup", server->info.sysname);
  server->app = app_server;
  server->loop = kr_app_server_mainloop_get(app_server);
  memset(&cpool_setup, 0, sizeof(cpool_setup));
  cpool_setup.size = sizeof(kr_ops_client);
  cpool_setup.slices = KR_OPS_CLIENTS_MAX;
  server->client_pool = kr_pool_create(&cpool_setup);
  if (server->client_pool == NULL) {
    printke("Ops: couldn't create ops client pool");
    return -1;
  }
  memset(&mpool_setup, 0, sizeof(mpool_setup));
  mpool_setup.nmedia = KR_OPS_TASKS_MAX;
  mpool_setup.loop = server->loop;
  mpool_setup.event_cb = media_event;
  server->media = kr_media_pool_create(&mpool_setup);
  if (server->media == NULL) {
    printke("Ops: couldn't create ops media pool");
    return -1;
  }
  memset(&web_setup, 0, sizeof(kr_web_setup));
  web_setup.event_cb = web_event;
  web_setup.user = server;
  web_setup.npaths = KR_OPS_TASKS_MAX;
  web_setup.master_loop = server->loop;
  server->web = kr_web_create(&web_setup);
  /* KLUDGE */
  if (getenv("KRAD_WEB_PORT")) {
    kr_web_path_setup_info new;
    memset(&new, 0, sizeof(new));
    new.type = KR_WEB_SERVER;
    new.server.type = KR_WEB_SERVER_HTTP;
    new.server.port = atoi(getenv("KRAD_WEB_PORT"));
    kr_web_path_create(server->web, &new, server, NULL);
  }
  /* END KLUDGE */
  memset(&router_setup, 0, sizeof(router_setup));
  router_setup.npaths = KR_OPS_TASKS_MAX;
  server->router = kr_router2_create(&router_setup);
  if (server->router == NULL) {
    printke("Ops: could not create front router");
    return -1;
  }
  kr_router_path path;
  path.user = server;
  path.disp = tasks_dispatch;
  ret = kr_router2_add(server->router, &path);
  if (ret != 0) return ret;
  path.disp = task_dispatch;
  ret = kr_router2_add(server->router, &path);
  if (ret != 0) return ret;
  //path.disp = media_list_dispatch;
  //ret = kr_router2_add(server->router, &path);
  path.disp = media_dispatch;
  ret = kr_router2_add(server->router, &path);
  if (ret != 0) return ret;
  memset(&tasks_setup, 0, sizeof(kr_task_pool_setup));
  tasks_setup.ntasks = KR_OPS_TASKS_MAX;
  tasks_setup.loop = server->loop;
  tasks_setup.event_cb = task_event;
  tasks_setup.media_pool = server->media;
  server->tasks = kr_task_pool_create(&tasks_setup);
  return 0;
}

static int handle_shutdown(void *user) {
  kr_ops *server;
  server = (kr_ops *)user;
  printk("%s: Shutdown", server->info.sysname);
  if (server->web) {
    kr_web_destroy(server->web);
    server->web = NULL;
  }
  kr_pool_destroy(server->client_pool);
  if (server->router) {
    kr_router2_destroy(server->router);
    server->router = NULL;
  }
  if (server->tasks) {
    kr_task_pool_destroy(server->tasks);
    server->tasks = NULL;
  }
  if (server->media) {
    kr_media_pool_destroy(server->media);
    server->media = NULL;
  }
  return 0;
}

void krad_ops(const char *callsign) {
  static kr_app_server_setup setup;
  static kr_ops server;
  strncpy(setup.appname, "krad_radio", sizeof(setup.appname) - 1);
  strncpy(setup.sysname, callsign, sizeof(setup.sysname) - 1);
  strncpy(server.info.sysname, callsign, sizeof(server.info.sysname) - 1);
  setup.user = &server;
  setup.startup_handler = handle_startup;
  setup.signal_handler = signal_event;
  setup.shutdown_handler = handle_shutdown;
  krad_app_server(&setup);
}
