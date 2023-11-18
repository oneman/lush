#include <string.h>
#include <time.h>
#include <krad/mem/pool.h>
#include <krad/app/debug.h>
#include <krad/io/dir.h>
#include <krad/hash/hashids.h>
#include <krad/util/strton.h>
#include <krad/time/interval.h>
#include <krad/web/encoding/gzip.h>
#include "xfer.h"
#include "error.h"

typedef struct {
  int sent_header;
  kr_file2 *file;
  kr_dir dir;
  int gzip;
} xfer_serve;

typedef struct {
  int sent_continue;
  kr_file2 *file;
} xfer_accept;

typedef struct {
  kr_upload_type type;
  union {
    kr_file2 *file;
    uint8_t *buf;
  };
  size_t sz;
  size_t pos;
  kr_http_url url;
  uint8_t headers;
} xfer_upload;

typedef struct {
  kr_download_type type;
  union {
    kr_file2 *file;
    uint8_t *buf;
  };
  size_t sz;
  size_t pos;
  kr_http_url url;
  uint8_t headers;
} xfer_download;

typedef struct {
  kr_http_url url;
  uint8_t *buf;
  size_t len;
  size_t pos;
  uint8_t headers;
  kr_uplink_read_cb *read_cb;
  void *user;
  int fd;
} xfer_uplink;

typedef struct {
  kr_http_url url;
  size_t bytes;
  uint8_t headers;
  kr_downlink_write_cb *write_cb;
  void *user;
} xfer_downlink;

typedef struct {
  kr_handler *cb;
  kr_interval *clock;
  kr_amem(mem, 128)
  uint64_t ms;
  uint64_t tries;
  int timeout;
  kr_xf_connect_cb *connect_cb;
  kr_xf_disconnect_cb *disconnect_cb;
} xfer_reconnect;

typedef struct {
  kr_gzip *gz;
  uint8_t *encoded;
  size_t len;
  size_t sz;
} xfer_encoding;

struct kr_xfer_path {
  union {
    struct {
      kr_web_client *client;
      kr_xfer_path *server;
    };
    struct {
      void *coconut;
    };
  };
  union {
    xfer_serve serve;
    xfer_accept accept;
    kr_source_client source;
    kr_transmit_client transmit;
    struct {
      int fd;
      kr_stream_client_add_cb *add_client;
      kr_stream_client_del_cb *del_client;
      kr_stream_client_read_cb *client_read;
      kr_stream_client_write_cb *client_write;
      kr_xmit_destroy_cb *destroy_cb;
      kr_accepted_cb *accepted_cb;
    };
    xfer_upload upload;
    xfer_download download;
    xfer_uplink uplink;
    xfer_downlink downlink;
  };
  kr_xfer *xfer;
  void *user;
  kr_xfer_path_info info;
  int done;
  xfer_reconnect reconnect;
  xfer_encoding encoding;
};

struct kr_xfer {
  kr_pool *pool;
  kr_web *web;
  void *user;
  kr_hashids *hids;
  kr_file_set *fs;
  kr_xfer_event_cb *event_cb;
};

static int file_event(kr_file_event *event);
static int web_event(kr_web_event *ev);
static void run_event(kr_xfer_path *path, kr_xfer_event_type type);
static int client_check_method(kr_web_client *client);
static kr_xfer_path *xfer_path(kr_xfer *xfer);
static kr_xfer_path *find_server(kr_xfer *x, kr_http_method m, kr_path *a);
static int xfer_handle(kr_xfer_path *xf);
static int xfer_finalize(kr_xfer_path *xf);
static int xfer_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr);

#include "accept.c"
#include "share.c"
#include "recv.c"
#include "transmit.c"
#include "upload.c"
#include "download.c"
#include "uplink.c"
#include "downlink.c"

static int file_event(kr_file_event *event) {
  int ret;
  kr_xfer *xfer;
  kr_file2 *file;
  ret = 0;
  xfer = (kr_xfer *)event->user;
  file = (kr_file2 *)event->file;
  if (!xfer || !file) return -1;
  switch (event->type) {
    case KR_FILE_OPEN:
      printk("XFER: File open event");
      break;
    case KR_FILE_CREATE:
      printk("XFER: File create event");
      break;
    case KR_FILE_CLOSE:
      printk("XFER: File close event");
      break;
    case KR_FILE_DATA:
      /*printk("XFER: File data event");*/
      break;
    case KR_FILE_WROTE:
      /*printk("XFER: File wrote event");*/
      break;
    default:
      ret = -1;
      printke("XFER: Unknown file event");
      break;
  }
  return ret;
}

static int reconnect_clock_setup(kr_xfer_path *xf, uint64_t ms) {
  kr_interval_setup setup;
  kr_loop *loop;
  kr_event ev;
  memset(&setup, 0, sizeof(setup));
  memset(&ev, 0, sizeof(ev));
  if (sizeof(xf->reconnect.mem) < kr_interval_sizeof()) return -1;
  setup.mem = xf->reconnect.mem;
  setup.ms = ms;
  xf->reconnect.clock = kr_interval_create(&setup);
  ev.events = EPOLLIN;
  ev.fd = kr_interval_get_fd(xf->reconnect.clock);
  ev.user = xf;
  ev.handler = xf->reconnect.cb;
  loop = kr_xfer_loop(xf->xfer);
  kr_loop_add(loop, &ev);
  return 0;
}

static int reconnect_init(kr_xfer_path *xf) {
  kr_xfer_reconnect_info *info;
  info = &xf->info.client.reconnect;
  xf->reconnect.ms += info->ms;
  xf->reconnect.tries++;
  printk("Xfer: reconnection attempt %d, waiting %d ms",
   xf->reconnect.tries, xf->reconnect.ms);
  if (!xf->reconnect.clock) {
    reconnect_clock_setup(xf, xf->reconnect.ms);
  } else {
    kr_interval_set(xf->reconnect.clock, xf->reconnect.ms);
  }
  return 0;
}

static void reconnect_destroy(kr_xfer_path *xf) {
  kr_loop *loop;
  if (!xf->reconnect.clock) return;
  loop = kr_xfer_loop(xf->xfer);
  kr_loop_del(loop, kr_interval_get_fd(xf->reconnect.clock));
  kr_interval_destroy(xf->reconnect.clock);
}

static int xf_reconnect(kr_xfer_path *xf) {
  int ret;
  kr_xfer_client_info *info;
  if (!xf) return -1;
  if (xf->done) return 1;
  if (!xf->reconnect.cb) return -2;
  info = &xf->info.client;
  if (!info->reconnect.ms || !info->reconnect.max_tries) return -3;
  ret = reconnect_init(xf);
  return ret;
}

static int web_event(kr_web_event *ev) {
  int ret;
  kr_xfer_path *xf;
  kr_http_response *res;
  kr_web_path_info *info;
  printkd("Xfer: web event");
  info = kr_web_path_info_get(ev->path);
  xf = (kr_xfer_path *)ev->user_path;
  if (!xf) return -1;
  if (info->type != KR_WEB_CLIENT) return -2;
  ret = 0;
  switch (ev->type) {
    case KR_WEB_OPEN:
      printk("xfer: web open");
      xf->client = kr_web_path_get_client(ev->path);
      ret = kr_xfer_data(xf);
      if (ret < 0) break;
      if (kr_xfer_done(xf)) {
        ret = kr_xfer_remove(xf);
      }
      break;
    case KR_WEB_CLOSE:
      printk("xfer: web close");
      kr_web_path_user_set(ev->path, NULL);
      xf->client = NULL;
      if (xf->reconnect.disconnect_cb) {
        ret = xf->reconnect.disconnect_cb(xf->user);
        if (ret != 0) {
          printke("Xfer: disconnect callback failed.");
          break;
        }
      }
      ret = xf_reconnect(xf);
      if (ret < 0) {
        printke("Xfer: xf_reconnect failed %d", ret);
      }
      break;
    case KR_WEB_HEADERS:
      printk("xfer: web headers");
      res = kr_web_client_get_response(xf->client);
      if (res->type == KR_HTTP_OK && xf->info.client.type == KR_DOWNLINK) {
        xf->reconnect.ms = 0;
        xf->reconnect.tries = 0;
        ret = xf->reconnect.connect_cb(xf->user);
        if (ret != 0) {
          downlink_close(xf);
          printke("Xfer: downlink connect callback failed.");
          break;
        }
      }
      if (res->type == KR_HTTP_CONTINUE && xf->info.client.type == KR_UPLINK) {
        printk("Xfer: uplink starting!");
        xf->reconnect.ms = 0;
        xf->reconnect.tries = 0;
        if (xf->reconnect.connect_cb) {
          ret = xf->reconnect.connect_cb(xf->user);
          if (ret != 0) {
            printke("Xfer: uplink connect callback failed.");
            break;
          }
        }
        break;
      }
      if (!res || res->type != KR_HTTP_OK) return -1;
      break;
    case KR_WEB_DATA:
      printkd("xfer: web data");
      ret = kr_xfer_data(xf);
      if (ret < 0) break;
      if (kr_xfer_done(xf)) {
        ret = kr_xfer_remove(xf);
      }
      break;
  }
  return ret;
}

static void run_event(kr_xfer_path *path, kr_xfer_event_type type) {
  kr_xfer_event event;
  event.user = path->xfer->user;
  event.user_path = path->user;
  event.path = path;
  event.type = type;
  kr_xfer_path_info_get(path, &event.info);
  path->xfer->event_cb(&event);
  path->user = event.user_path;
}

static int client_check_method(kr_web_client *client) {
  kr_http_request *req;
  kr_http_method m;
  req = kr_web_client_get_request(client);
  if (!req) return 0;
  m = req->req_line.method;
  if (m == KR_HTTP_GET || m == KR_HTTP_SOURCE || m == KR_HTTP_PUT
   || m == KR_HTTP_POST || m == KR_HTTP_HEAD || m == KR_HTTP_OPTIONS) return 1;
  return 0;
}

static kr_xfer_path *xfer_path(kr_xfer *xfer) {
  kr_xfer_path *path;
  path = kr_pool_slice(xfer->pool);
  if (!path) return NULL;
  memset(path, 0, sizeof(*path));
  path->xfer = xfer;
  return path;
}

static kr_xfer_path *find_server(kr_xfer *x, kr_http_method m, kr_path *addr) {
  kr_xfer_path *server;
  int ret;
  int i;
  ret = 0;
  if (kr_path_steps(addr) < 1) return NULL;
  i = 0;
  while ((server = kr_pool_iterate_active(x->pool, &i))) {
    if (server->info.type != KR_XFER_SERVER) continue;
    switch (server->info.server.type) {
      case KR_XFER_ACCEPTING:
        ret = accepting_match(server, m, addr);
        break;
      case KR_XFER_SHARING:
        ret = sharing_match(server, m, addr);
        break;
      case KR_XFER_RECEIVER:
        ret = receiver_match(server, m, addr);
        break;
      case KR_XFER_TRANSMITTER:
        ret = transmitter_match(server, m, addr);
        break;
      default:
        printke("XFER find server unknown type");
        break;
    }
    if (ret) return server;
  }
  return NULL;
}

static int xfer_handle(kr_xfer_path *xf) {
  int ret;
  ret = -1;
  switch (xf->info.client.type) {
    case KR_ACCEPT:
      ret = accept_handle(xf);
      break;
    case KR_SERVE:
      ret = serve_handle(xf);
      break;
    case KR_SOURCE:
      ret = source_handle(xf);
      break;
    case KR_TRANSMIT:
      //ret = transmit_data(xf);
      ret = 0;
      break;
    case KR_UPLOAD:
      ret = upload_handle(xf);
      break;
    case KR_DOWNLOAD:
      ret = download_handle(xf);
      break;
    case KR_UPLINK:
      ret = uplink_data(xf);
      break;
    case KR_DOWNLINK:
      ret = downlink_handle(xf);
      break;
    default:
      printke("XFER handle unknown client type");
      break;
  }
  return ret;
}

static int xfer_finalize(kr_xfer_path *xf) {
  int ret;
  ret = -1;
  switch (xf->info.client.type) {
    case KR_ACCEPT:
      ret = accept_close(xf);
      break;
    case KR_SERVE:
      ret = serve_close(xf);
      break;
    case KR_SOURCE:
      ret = source_close(xf);
      break;
    case KR_TRANSMIT:
      ret = transmit_close(xf);
      break;
    case KR_UPLOAD:
      ret = upload_close(xf);
      break;
    case KR_DOWNLOAD:
      ret = download_close(xf);
      break;
    case KR_UPLINK:
      ret = uplink_close(xf);
      break;
    case KR_DOWNLINK:
      ret = downlink_close(xf);
      break;
    default:
      printke("XFER close unknown client type");
      break;
  }
  return ret;
}

static int xfer_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int ret;
  ret = -1;
  switch (xf->server->info.server.type) {
    case KR_XFER_ACCEPTING:
      ret = accept_setup(xf, m, addr);
      break;
    case KR_XFER_SHARING:
      ret = serve_setup(xf, m, addr);
      break;
    case KR_XFER_RECEIVER:
      ret = source_setup(xf, m, addr);
      break;
    case KR_XFER_TRANSMITTER:
      ret = transmit_setup(xf, m, addr);
      break;
    default:
      printke("XFER setup unknown client type");
      break;
  }
  return ret;
}

static int xfer_parse_req(kr_xfer_path *xf, kr_http_request *req) {
  int ret;
  ret = -1;
  switch (xf->server->info.server.type) {
    case KR_XFER_ACCEPTING:
      ret = 0;
      break;
    case KR_XFER_SHARING:
      ret = 0;
      break;
    case KR_XFER_RECEIVER:
      ret = 0;
      break;
    case KR_XFER_TRANSMITTER:
      ret = transmit_parse_req(xf, req);
      break;
    default:
      printke("XFER parse req unknown client type");
      break;
  }
  return ret;
}

int kr_xfer_path_web_client_set(kr_xfer_path *path, kr_web_client *client) {
  if (!path) return -1;
  path->client = client;
  return 0;
}

kr_loop *kr_xfer_loop(kr_xfer *xfer) {
  if (!xfer) return NULL;
  return kr_web_loop(xfer->web);
}

int kr_xfer_path_info_get(kr_xfer_path *path, kr_xfer_path_info *info) {
  if (!path || !info) return -1;
  *info = path->info;
  return 0;
}

int kr_xfer_ctl(kr_xfer_path *path, kr_patchset *patchset) {
  if (!path || !patchset) return -1;
  kr_patchset_apply(&path->info, patchset);
  run_event(path, KR_XFER_PATCH);
  return 0;
}

int kr_xfer_done(kr_xfer_path *path) {
  if (!path) return -1;
  return path->done;
}

int kr_xfer_data(kr_xfer_path *path) {
  int ret;
  if (!path) return -1;
  ret = -2;
  if (path->info.type == KR_XFER_SERVER) {
    if (path->info.server.type == KR_XFER_TRANSMITTER) {
      ret = transmit_data(path);
    }
  }
  if (path->info.type == KR_XFER_CLIENT) {
    ret = xfer_handle(path);
  }
  return ret;
}

int kr_xfer_match(kr_xfer *x, kr_http_method m, kr_path *addr) {
  kr_xfer_path *path;
  if (!x || !m || !addr) return 0;
  path = find_server(x, m, addr);
  if (!path) return 0;
  return 1;
}

int kr_xfer_headers(kr_xfer_path *path) {
  int ret;
  int fail;
  kr_xfer *xfer;
  kr_http_request *req;
  kr_path *addr;
  kr_http_method meth;
  size_t path_len;
  fail = 0;
  if (!path) return -1;
  if (path->info.type != KR_XFER_CLIENT) return -2;
  xfer = path->xfer;
  ret = client_check_method(path->client);
  if (!ret) return -3;
  req = kr_web_client_get_request(path->client);
  meth = req->req_line.method;
  kr_path_alloca(addr);
  path_len = req->req_line.path_len;
  if ((req->req_line.path_len > 0)
   && (req->req_line.path[req->req_line.path_len - 1] == '/')) {
    path_len--;
  }
  kr_path_parse(addr, req->req_line.path, path_len);
  if (meth == KR_HTTP_GET && !kr_path_steps(addr)) {
    char *def_path = "/rack/index.html";
    kr_path_parse(addr, def_path, strlen(def_path));
  }
  path->server = find_server(xfer, meth, addr);
  if (!path->server) fail = 1;
  if (!fail) {
    ret = xfer_parse_req(path, req);
    if (ret != 0) fail = 1;
    ret = xfer_setup(path, meth, addr);
    if (ret != 0) fail = 1;
  }
  if (!fail) {
    xfer_handle(path);
    return 0;
  }
  printk("XFER: Can't handle client");
  return -1;
}

int kr_xfer_remove(kr_xfer_path *path) {
  int ret;
  kr_xfer *xfer;
  if (!path) return -1;
  xfer = path->xfer;
  if (xfer == NULL) {
    printke("xfer: run_event XFER shouldn't be null here!");
    return -2;
  }
  run_event(path, KR_XFER_DESTROY);
  if (path->info.type == KR_XFER_CLIENT) {
    xfer_finalize(path);
    reconnect_destroy(path);
  } else if (path->info.type == KR_XFER_SERVER) {
    if (path->info.server.type == KR_XFER_TRANSMITTER) {
      transmitter_destroy(path);
    } else if (path->info.server.type == KR_XFER_SHARING) {
      sharing_destroy(path);
    }
  }
  ret = kr_pool_release(xfer->pool, path);
  return ret;
}

int kr_xfer_accept(kr_xfer *xfer, kr_web_client *client, void *user) {
  kr_xfer_path *path;
  if (!xfer || !client || !user) return -1;
  path = xfer_path(xfer);
  if (!path) return -3;
  path->user = user;
  path->client = client;
  path->info.type = KR_XFER_CLIENT;
  run_event(path, KR_XFER_CREATE);
  return 0;
}

kr_xfer_path *kr_xfer_connect(kr_xfer *xfer, kr_xfer_client_setup *setup) {
  kr_xfer_path *path;
  int ret;
  int fail;
  if (!xfer || !setup || !setup->user) return NULL;
  ret = -1;
  path = xfer_path(xfer);
  if (!path) return NULL;
  fail = 0;
  path->user = setup->user;
  path->info.type = KR_XFER_CLIENT;
  path->info.client = setup->info;
  switch (path->info.client.type) {
    case KR_UPLOAD:
      ret = upload_setup(path, &setup->upload);
      if (ret != 0) fail = 1;
      break;
    case KR_DOWNLOAD:
      ret = download_setup(path, &setup->download);
      if (ret != 0) fail = 1;
      break;
    case KR_UPLINK:
      ret = uplink_setup(path, &setup->uplink);
      if (ret != 0) fail = 1;
      break;
    case KR_DOWNLINK:
      ret = downlink_setup(path, &setup->downlink);
      if (ret != 0) fail = 1;
      break;
    default:
      fail = 1;
      break;
  }
  if (fail) {
    kr_pool_release(xfer->pool, path);
    return NULL;
  }
  run_event(path, KR_XFER_CREATE);
  return path;
}

kr_xfer_path *kr_xfer_server(kr_xfer *xfer, kr_xfer_server_setup *setup) {
  kr_xfer_path *path;
  int ret;
  if (!xfer || !setup || !setup->user) return NULL;
  ret = -1;
  path = xfer_path(xfer);
  if (!path) return NULL;
  path->user = setup->user;
  path->info.type = KR_XFER_SERVER;
  path->info.server = setup->info;
  switch(path->info.server.type) {
    case KR_XFER_SHARING:
      ret = sharing_setup(path);
      break;
    case KR_XFER_ACCEPTING:
      path->user = setup->acptr.user;
      path->accepted_cb = setup->acptr.accepted_cb;
      ret = accepting_setup(&path->info.server);
      break;
    case KR_XFER_RECEIVER:
      path->user = setup->rcvr.user;
      path->add_client = setup->rcvr.add_client;
      path->del_client = setup->rcvr.del_client;
      path->client_write = setup->rcvr.client_write;
      ret = receiver_setup(&path->info.server);
      break;
    case KR_XFER_TRANSMITTER:
      path->user = setup->txmtr.user;
      path->add_client = setup->txmtr.add_client;
      path->del_client = setup->txmtr.del_client;
      path->client_read = setup->txmtr.client_read;
      path->destroy_cb = setup->txmtr.destroy_cb;
      ret = transmitter_setup(path);
      break;
    default:
      ret = -1;
      break;
  }
  if (ret) {
    kr_xfer_remove(path);
    return NULL;
  }
  run_event(path, KR_XFER_CREATE);
  printk("XFER: server setup");
  return path;
}

kr_file_set *kr_xfer_get_file_set(kr_xfer *xfer) {
  if (!xfer) return NULL;
  return xfer->fs;
}

int kr_xfer_destroy(kr_xfer *xfer) {
  int i;
  kr_xfer_path *path;
  if (!xfer) return -1;
  printk("XFER: Destroying");
  i = 0;
  while ((path = kr_pool_iterate_active(xfer->pool, &i))) {
    if (path->info.type == KR_XFER_SERVER &&
      path->info.server.type == KR_XFER_TRANSMITTER) {
      path->destroy_cb(path->user);
    }
    kr_xfer_remove(path);
  }
  free(xfer->hids);
  kr_pool_destroy(xfer->pool);
  printk("XFER: Destroyed");
  return 0;
}

kr_xfer *kr_xfer_create(kr_xfer_setup *setup) {
  kr_xfer *xfer;
  kr_pool *pool;
  kr_fs_setup fs_setup;
  kr_pool_setup pool_setup;
  kr_hashids_setup hids_setup;
  if (!setup) return NULL;
  printk("XFER: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*xfer);
  pool_setup.size = sizeof(kr_xfer_path);
  pool_setup.slices = setup->npaths;
  pool = kr_pool_create(&pool_setup);
  if (!pool) nullfail(KR_ERR_XFER_FRAKERSNACKED);
  xfer = kr_pool_overlay_get(pool);
  memset(xfer, 0, sizeof(*xfer));
  xfer->pool = pool;
  xfer->user = setup->user;
  xfer->web = setup->web;
  xfer->event_cb = setup->event_cb;
  hids_setup.mem = kr_allocz(1, kr_hashids_sizeof());
  memcpy(hids_setup.salt, "kr_xfer", 7);
  hids_setup.len = 7;
  xfer->hids = kr_hashids_init(&hids_setup);
  fs_setup.nfiles = 64;
  fs_setup.event_cb = file_event;
  fs_setup.user = xfer;
  xfer->fs = kr_file_set_create(&fs_setup);
  printk("XFER: Created");
  return xfer;
}
