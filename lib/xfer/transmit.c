#include <krad/app/notice.h>
#include <krad/radio/limits.h>

#define KR_TRANSMIT_BACKBUFFER_DEFAULT 4000

static int transmit_header(kr_xfer_path *xf);
static int transmit_handle(kr_xfer_path *xf);
static int transmit_close(kr_xfer_path *xf);
static int transmit_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr);
static int transmitter_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr);

static int transmit_header(kr_xfer_path *xf) {
  int ret;
  kr_transmit_info *info;
  kr_http_response_type type;
  kr_http_header hdr;
  kr_http_date_header dh;
  kr_http_response res;
  info = &xf->info.client.transmit;
  type = KR_HTTP_OK;
  kr_http_response_init(&res, type);
  kr_http_date_header_now(&dh);
  kr_http_response_add_header(&res, &dh.hdr);
  kr_http_response_add_version_rand(&res);
  kr_http_response_any_origin(&res);
  //hdr.value = ext_to_mime(strrchr(xf->info.client.path, '.') + 1);
  hdr.value = NULL; //ext_to_mime(strrchr(xf->info.client.path, '.') + 1);
  if (hdr.value != NULL) {
    hdr.key = "Content-Type";
    hdr.key_len = strlen(hdr.key);
    hdr.value_len = strlen(hdr.value);
    kr_http_response_add_header(&res, &hdr);
  }
  ret = kr_web_client_pack_response(xf->client, &res);
  info->packed_header = 1;
  return ret;
}

static int transmit_handle(kr_xfer_path *xf) {
  ssize_t in;
  ssize_t out;
  size_t len;
  ssize_t out_space;
  size_t buflen;
  int bytes;
  uint8_t buf[8192 * 8];
  kr_transmit_info *info;
  bytes = 0;
  info = &xf->info.client.transmit;
  if (!info->packed_header &&
    xf->server->info.server.transmitter.type == KR_XFER_TRANSMITTER_HTTP) {
    transmit_header(xf);
  }
  buflen = sizeof(buf);
  for (;;) {
    len = buflen;
    out_space = kr_web_client_space(xf->client);
    if (out_space < 1) {
      kr_web_client_want_space(xf->client);
      break;
    }
    if (out_space < buflen) len = out_space;
    in = xf->server->client_read(&xf->transmit, buf, len);
    if (in < 1) break;
    /*printk("XFER: Transmit packing %d bytes", in);*/
    out = kr_web_client_pack(xf->client, buf, in);
    if (out < in) {
      printke("Transmit: kr_web_client_pack failed %d vs %d", out, in);
    }
    bytes += out;
  }
  return bytes;
}

static int transmit_close(kr_xfer_path *xf) {
  int ret;
  if (xf->transmit.user == NULL) return 0;
  ret = xf->server->del_client(&xf->transmit, xf->server->user);
  if (ret) {
    printke("Transmit: close ret %d", ret);
  } else {
    if (xf->server->info.server.transmitter.clients > 0) {
      xf->server->info.server.transmitter.clients--;
    }
  }
  return ret;
}

static int parse_backbuf(char *str, size_t len) {
  uint val;
  int ret;
  ret = KR_TRANSMIT_BACKBUFFER_DEFAULT;
  if (len < 1 || len > 7) return ret;
  if (!strncmp(str, "all", len)) {
    ret = -1;
  }
  if (!strncmp(str, "none", len)) {
    ret = 0;
  }
  if (str[len - 1] == 's') {
    if (len > 2 && str[len - 2] == 'm') {
      ret = kr_strtou32(&val, str, len - 2);
      if (ret != 0) return KR_TRANSMIT_BACKBUFFER_DEFAULT;
      ret = val;
    } else {
      if (len < 2 || len > 3) return KR_TRANSMIT_BACKBUFFER_DEFAULT;
      ret = kr_strtou32(&val, str, len - 1);
      if (ret != 0) return KR_TRANSMIT_BACKBUFFER_DEFAULT;
      ret = val * 1000;
    }
  }
  return ret;
}

static int transmit_parse_req(kr_xfer_path *xf, kr_http_request *req) {
  char *key;
  char *val;
  size_t key_len;
  size_t val_len;
  /* Just handling one param atm, might need to extend to
   multiple params in the future */
  memset(&xf->transmit, 0, sizeof(xf->transmit));
  xf->transmit.backbuf = KR_TRANSMIT_BACKBUFFER_DEFAULT;
  if (!req->req_line.param_count) return 0;
  key = (char *)req->req_line.param[0].key;
  val = (char *)req->req_line.param[0].value;
  key_len = req->req_line.param[0].key_len;
  val_len = req->req_line.param[0].value_len;
  if (!strncmp(key, "backbuffer", key_len)) {
    xf->transmit.backbuf = parse_backbuf(val, val_len);
  }
  return 0;
}

static int transmit_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int ret;
  kr_transmit_info *info;
  kr_xfer_transmitter_info *tinfo;
  xf->info.client.type = KR_TRANSMIT;
  info = &xf->info.client.transmit;
  tinfo = &xf->server->info.server.transmitter;
  info->packed_header = 0;
  if (tinfo->clients >= KR_TRANSMIT_CLIENTS_MAX) {
    printke("Transmit: max clients limit reached %d!",
     KR_TRANSMIT_CLIENTS_MAX);
    return -1;
  }
  ret = xf->server->add_client(&xf->transmit, xf->server->user);
  if (ret) {
    printke("Transmit: setup ret %d", ret);
  } else {
    printk("XFER: Transmit setup");
    tinfo->clients++;
  }
  return ret;
}

static int transmitter_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int len;
  char *root;
  if (xf->info.server.transmitter.type == KR_XFER_TRANSMITTER_HTTP &&
  m != KR_HTTP_GET) return 0;
  /* FIXME look at share... ? */
  len = kr_path_cur_name(addr, &root);
  if (len == xf->info.server.len) {
    if (memcmp(xf->info.server.path, root, len) == 0) {
      return 1;
    }
  }
  return 0;
}

static int transmit_data(kr_xfer_path *xf) {
  int ret;
  ret = kr_give_notice(xf->fd);
  return ret;
}

static int output_data_event(kr_event *e) {
  int i;
  int ret;
  kr_pool *pool;
  kr_xfer_path *path;
  kr_xfer_path *transmitter_path;
  transmitter_path = (kr_xfer_path *)e->user;
  pool = transmitter_path->xfer->pool;
  ret = kr_take_notice(transmitter_path->fd);
  if (ret <= 0) {
    printke("Transmitter: got output data event! %d", ret);
    return 0;
  }
  i = 0;
  while ((path = kr_pool_iterate_active(pool, &i))) {
    if (path->info.type != KR_XFER_CLIENT) continue;
    if (path->info.client.type != KR_TRANSMIT) continue;
    if (path->server != transmitter_path) continue;
    if (path->transmit.user == NULL) continue;
    transmit_handle(path);
  }
  return 0;
}

static int transmitter_destroy(kr_xfer_path *path) {
  int i;
  kr_pool *pool;
  kr_xfer_path *client;
  kr_web_path *wpath;
  printk("XFER: transmitter destroy");
  i = 0;
  pool = path->xfer->pool;
  while ((client = kr_pool_iterate_active(pool, &i))) {
    if (client->info.type != KR_XFER_CLIENT) continue;
    if (client->info.client.type != KR_TRANSMIT) continue;
    if (client->server != path) continue;
    wpath = kr_web_client_get_path(client->client);
    kr_web_path_remove(wpath);
  }
  kr_notifier_destroy(path->fd);
  kr_loop_del(kr_xfer_loop(path->xfer), path->fd);
  path->info.server.transmitter.clients = 0;
  return 0;
}

static int transmitter_setup(kr_xfer_path *path) {
  int ret;
  kr_loop *loop;
  kr_event ev;
  ev.events = EPOLLIN;
  path->fd = kr_notifier_create();
  ev.fd = path->fd;
  ev.user = path;
  ev.handler = output_data_event;
  loop = kr_xfer_loop(path->xfer);
  ret = kr_loop_add(loop, &ev);
  path->info.server.transmitter.clients = 0;
  return ret;
}
