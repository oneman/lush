#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>
#include <krad/util/strton.h>
#include "proxy.h"

#define KR_PROXY_NCONN 32

typedef struct {
  kr_web_path *dst;
  kr_web_path *src;
  kr_proxy *proxy;
  kr_web_remote_info downstream;
} kr_proxy_conn;

struct kr_proxy {
  kr_web *web;
  kr_proxy_info info;
  kr_proxy_conn conn[KR_PROXY_NCONN];
};

static int upstream_drop(kr_proxy_conn *conn) {
  kr_web_path *cpy;
  if (conn->src) {
    if (conn->dst) printk("Proxy: Upstream dropping connection");
    cpy = conn->src;
    conn->src = NULL;
    kr_web_path_remove(cpy);
  }
  return 0;
}

static int downstream_drop(kr_proxy_conn *conn) {
  kr_web_path *cpy;
  if (conn->dst) {
    if (conn->src) printk("Proxy: Downstream dropping connection");
    cpy = conn->dst;
    conn->dst = NULL;
    kr_web_path_remove(cpy);
  }
  return 0;
}

static int proxy_request(kr_proxy_conn *conn) {
  int ret;
  kr_web_client *src_client;
  kr_web_client *dst_client;
  kr_http_request *req;
  dst_client = kr_web_path_get_client(conn->dst);
  src_client = kr_web_path_get_client(conn->src);
  if (!src_client) {
    printke("Proxy: Failed to get downstream client");
    return -1;
  }
  if (!dst_client) {
    printke("Proxy: Failed to get upstream client");
    return -1;
  }
  req = kr_web_client_get_request(src_client);
  if (!req) {
    printke("Proxy: Failed to get downstream client request");
    return -1;
  }
  ret = kr_web_client_pack_request(dst_client, req);
  if (ret < 0) {
    printke("Proxy: Failed to pack request upstream");
    return -1;
  }
  return ret;
}

static int proxy_data(kr_web_client *dst, kr_web_client *src) {
  int ret;
  uint8_t data[4096];
  for (;;) {
    ret = kr_web_client_unpack(src, data, sizeof(data));
    if (ret < 1) break;
    ret = kr_web_client_pack(dst, data, ret);
    if (ret < 1) break;
  }
  if (ret) {
    printke("Proxy: data ret %d", ret);
  }
  return ret;
}

static int upstream_event(kr_web_event *ev) {
  /*printk("upstream event start");*/
  int ret;
  kr_http_response *res;
  kr_proxy_conn *conn;
  kr_web_client *src_client;
  kr_web_client *dst_client;
  conn = (kr_proxy_conn *)ev->user_path;
  /*usleep(100 * 1000);*/
  dst_client = kr_web_path_get_client(conn->dst);
  src_client = kr_web_path_get_client(conn->src);
  if (ev->type == KR_WEB_HEADERS || ev->type == KR_WEB_DATA) {
    if (!src_client) {
      printke("Proxy: Failed to get downstream client");
      return -1;
    }
    if (!dst_client) {
      printke("Proxy: Failed to get upstream client");
      return -1;
    }
  }
  switch (ev->type) {
  case KR_WEB_OPEN:
    printk("Proxy: upstream open");
    conn->dst = ev->path;
    if (conn->proxy->info.type != KR_PROXY_WEBSOCKET_TO_TCP) {
      proxy_request(conn);
    }
    break;
  case KR_WEB_CLOSE:
    upstream_drop(conn);
    break;
  case KR_WEB_HEADERS:
    printk("Proxy: upstream headers");
    if (conn->proxy->info.type != KR_PROXY_WEBSOCKET_TO_TCP) {
      res = kr_web_client_get_response(dst_client);
      if (!res) {
        printke("Proxy: could not get upstream response");
        return -1;
      }
      ret = kr_web_client_pack_response(src_client, res);
      if (ret) {
        printk("Proxy: packing response for downstream client ret %d", ret);
      }
    }
    break;
    printk("Proxied response downstream");
  case KR_WEB_DATA:
    /*printk("Proxy: Upstream data");*/
    proxy_data(dst_client, src_client);
    proxy_data(src_client, dst_client);
    break;
  default:
    return -2;
  }
  /*printk("upstream event stop");*/
  return 0;
}

static int downstream_event(kr_web_event *ev) {
  /*printk("downstream event start");*/
  kr_proxy_conn *conn;
  kr_web_client *src_client;
  kr_web_client *dst_client;
  /*usleep(100 * 1000);*/
  conn = (kr_proxy_conn *)ev->user_path;
  dst_client = kr_web_path_get_client(conn->dst);
  src_client = kr_web_path_get_client(conn->src);
  if (ev->type == KR_WEB_HEADERS || ev->type == KR_WEB_DATA) {
    if (!src_client) {
      printke("Proxy: Failed to get downstream client");
      return -1;
    }
    if (!dst_client) {
      printke("Proxy: Failed to get upstream client");
      return -1;
    }
  }
  switch (ev->type) {
  case KR_WEB_OPEN:
    printke("Proxy: Downstream open?");
    break;
  case KR_WEB_CLOSE:
    downstream_drop(conn);
    break;
  case KR_WEB_HEADERS:
    printk("Proxy: Downstream headers");
    if (conn->proxy->info.type != KR_PROXY_WEBSOCKET_TO_TCP) {
      proxy_request(conn);
      printk("Proxied request upstream");
    }
    break;
  case KR_WEB_DATA:
    /*printk("Proxy: Downstream data");*/
    proxy_data(dst_client, src_client);
    proxy_data(src_client, dst_client);
    break;
  default:
    return -1;
  }
  /*printk("downstream event stop");*/
  return 0;
}

static int connect_upstream(kr_proxy_conn *conn) {
  int ret;
  kr_web_path_setup_info setup;
  memset(&setup, 0, sizeof(setup));
  setup.type = KR_WEB_CLIENT;
  /*setup.client.protocol = KR_PROTOCOL_HTTP;*/
  setup.client.protocol = KR_PROTOCOL_TCP;
  setup.client.remote = conn->proxy->info.dst;
  ret = kr_web_path_create(conn->proxy->web, &setup, conn, upstream_event);
  if (ret) printk("Proxy: connect upstream ret %d", ret);
  return ret;
}

static int add_conn(kr_proxy *p, kr_web_path *path) {
  int i;
  kr_proxy_conn *conn;
  for (i = 0; i < KR_PROXY_NCONN; i++) {
    if (p->conn[i].src == NULL) {
      conn = &p->conn[i];
      conn->proxy = p;
      conn->src = path;  
      kr_web_path_user_set(path, conn);
      kr_web_path_handler_set(path, downstream_event);
      connect_upstream(conn);
      printk("Proxy: Added connection");
      return 1;
    }
  }
  return 0;
}

int kr_proxy_accept(kr_proxy *p, kr_web_path *path, kr_path *addr) {
  int len;
  char *root;
  /*if (addr == NULL && p->info.type == KR_PROXY_WEBSOCKET_TO_TCP) {
    if (add_conn(p, path)) return 1;
  }*/
  len = kr_path_cur_name(addr, &root);
  if (len == strlen(p->info.path)) {
    if (memcmp(p->info.path, root, len) == 0) {
      if (add_conn(p, path)) return 1;
      return 0;
    }
  }
  return 0;
}

int kr_proxy_destroy(kr_proxy *p) {
  int i;
  if (!p) return -1;
  for (i = 0; i < KR_PROXY_NCONN; i++) {
    if (p->conn[i].src) upstream_drop(&p->conn[i]);
    /* downstream? */
  }
  printk("Proxy: Destroyed %s -> %.*s:%u", p->info.path, p->info.dst.host_len,
   p->info.dst.host, p->info.dst.port); 
  return 0;
}

kr_proxy *kr_proxy_create(kr_proxy_setup *setup) {
  kr_proxy *p;
  if (!setup || !setup->mem || !setup->web) return NULL;
  p = (kr_proxy *)setup->mem;
  p->info = setup->info;
  p->web = setup->web;
  memset(p->conn, 0, sizeof(p->conn));
  printk("Proxy: created %s -> %.*s:%u", p->info.path, p->info.dst.host_len,
   p->info.dst.host, p->info.dst.port); 
  return p;
}

size_t kr_proxy_sizeof() {
  return sizeof(kr_proxy);
}
