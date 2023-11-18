static int uplink_data(kr_xfer_path *xf);
static int uplink_handle(kr_xfer_path *xf);
static int uplink_close(kr_xfer_path *xf);

static int uplink_reconnect_event(kr_event *e) {
  int ret;
  kr_http_url url;
  kr_xfer_path *xf;
  kr_event ev;
  kr_web_path_setup_info wp_setup;
  memset(&ev, 0, sizeof(ev));
  xf = (kr_xfer_path *)e->user;
  kr_interval_update(xf->reconnect.clock);
  printk("Xfer Uplink: reconnect event");
  if (xf->reconnect.timeout) {
    printk("Xfer Uplink: time to try to reconnect again.");
    xf->reconnect.ms = xf->info.client.reconnect.ms;
    xf->reconnect.timeout = 0;
    xf->reconnect.tries = 1;
  }
  if (xf->reconnect.tries >= xf->info.client.reconnect.max_tries) {
    printk("Xfer Uplink: Max reconnection attempts exceeded,"
     " waiting now for 15 mins before trying again");
    xf->reconnect.tries = 0;
    xf->reconnect.ms = 0;
    xf->reconnect.timeout = 1;
    kr_interval_set(xf->reconnect.clock, 15 * 60 * 1000);
    return 0;
  }
  xf->uplink.headers = 0;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  ret = kr_http_url_parse(&url, xf->info.client.uplink.url,
   xf->info.client.uplink.len);
  if (ret != 0) return -2;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  ret = kr_web_path_create(xf->xfer->web, &wp_setup, xf, web_event);
  if (ret != 0) return -3;
  return 0;
}

static int uplink_data(kr_xfer_path *xf) {
  int ret;
  ret = kr_give_notice(xf->uplink.fd);
  return ret;
}

static int uplink_data_event(kr_event *e) {
  int ret;
  kr_xfer_path *path;
  path = (kr_xfer_path *)e->user;
  ret = kr_take_notice(path->uplink.fd);
  ret = uplink_handle(path);
  return ret;
}

static int uplink_setup(kr_xfer_path *path, kr_uplink_setup *setup) {
  int ret;
  kr_http_url url;
  kr_event ev;
  kr_loop *loop;
  kr_web_path_setup_info wp_setup;
  if (!path || !setup) return -1;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  memset(&ev, 0, sizeof(ev));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  ret = kr_http_url_parse(&url, setup->info.url, setup->info.len);
  if (ret != 0) return -2;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  path->uplink.fd = kr_notifier_create();
  if (path->uplink.fd < 0) return -3;
  ev.events = EPOLLIN;
  ev.fd = path->uplink.fd;
  ev.user = path;
  ev.handler = uplink_data_event;
  loop = kr_xfer_loop(path->xfer);
  ret = kr_loop_add(loop, &ev);
  ret = kr_web_path_create(path->xfer->web, &wp_setup, path, web_event);
  if (ret != 0) return -4;
  path->info.type = KR_XFER_CLIENT;
  path->info.client.type = KR_UPLINK;
  path->info.client.uplink = setup->info;
  path->uplink.url = url;
  path->uplink.user = setup->user;
  path->uplink.read_cb = setup->read_cb;
  path->reconnect.connect_cb = setup->reconnect.connect_cb;
  path->reconnect.disconnect_cb = setup->reconnect.disconnect_cb;
  path->reconnect.cb = uplink_reconnect_event;
  path->reconnect.clock = NULL;
  path->info.client.reconnect.max_tries = setup->reconnect.max_tries;
  path->info.client.reconnect.ms = setup->reconnect.duration;
  return 0;
}

static int uplink_close(kr_xfer_path *xf) {
  kr_web_path *wpath;
  printk("uplink close");
  memset(&xf->uplink, 0, sizeof(xf->uplink));
  memset(&xf->reconnect, 0, sizeof(xf->reconnect));
  if (xf->uplink.fd > 0) {
    kr_notifier_destroy(xf->uplink.fd);
  }
  xf->uplink.fd = -1;
  if (xf->client) {
    wpath = kr_web_client_get_path(xf->client);
    xf->client = NULL;
    kr_web_path_remove(wpath);
  }
  return 0;
}

static int uplink_handle(kr_xfer_path *xf) {
  int ret;
  kr_http_header hdr;
  kr_http_request req;
  uint8_t buf[1024];
  char auth[256];
  char auth_hdr[256];
  size_t auth_len;
  char *e;
  char ext[8];
  char *mime;
  size_t bytes;
  size_t len;
  /*printk("uplink handle");*/
  if (!xf->client) return -1;
  if (!xf->uplink.headers) {
    ret = kr_http_url_parse(&xf->uplink.url,
     xf->info.client.uplink.url, xf->info.client.uplink.len);
    if (ret != 0) return -1;
    memset(&req, 0, sizeof(req));
    memset(&hdr, 0, sizeof(hdr));
    req.req_line.method = KR_HTTP_PUT;
    req.req_line.path = xf->uplink.url.path;
    req.req_line.path_len = xf->uplink.url.path_len;
    req.req_line.version = KR_HTTP_VERSION_1_1;
    req.req_line.param_count = xf->uplink.url.param_count;
    memcpy(req.req_line.param, xf->uplink.url.param,
     sizeof(req.req_line.param));
    hdr.key = "Host";
    hdr.key_len = 4;
    hdr.value = xf->uplink.url.host;
    hdr.value_len = xf->uplink.url.host_len;
    kr_http_request_add_header(&req, &hdr);
    hdr.key = "User-Agent";
    hdr.key_len = 10;
    hdr.value = "KRAD RADIO";
    hdr.value_len = 10;
    kr_http_request_add_header(&req, &hdr);
    if (!xf->info.client.uplink.legacy) {
      hdr.key = "Transfer-Encoding";
      hdr.key_len = 17;
      hdr.value = "chunked";
      hdr.value_len = 7;
      kr_http_request_add_header(&req, &hdr);
    }
    e = memchr(xf->uplink.url.path, '.', xf->uplink.url.path_len);
    if (e) {
      ret = snprintf(ext, sizeof(ext), "%.*s",
       (int)(xf->uplink.url.path_len - (e - xf->uplink.url.path) - 1) , e + 1);
      mime = ext_to_mime(ext);
      if (mime) {
        hdr.key = "Content-Type";
        hdr.key_len = 12;
        hdr.value = mime;
        hdr.value_len = strlen(mime);
        kr_http_request_add_header(&req, &hdr);
      }
    }
    if (xf->uplink.url.user && xf->uplink.url.pwd) {
      ret = kr_base64((uint8_t *)auth,
        (uint8_t *)xf->uplink.url.user, xf->uplink.url.user_len +
         xf->uplink.url.pwd_len + 1, sizeof(auth));
      if (ret <= 0) return -1;
      auth_len = snprintf(auth_hdr, sizeof(auth_hdr),
        "Basic %.*s", ret, auth);
      if (auth_len >= sizeof(auth_hdr)) return -1;
      hdr.key = "Authorization";
      hdr.key_len = 13;
      hdr.value = auth_hdr;
      hdr.value_len = auth_len;
      kr_http_request_add_header(&req, &hdr);
    }
    ret = kr_web_client_pack_request(xf->client, &req);
    if (ret <= 0) return -1;
    xf->uplink.headers = 1;
  }
  bytes = 0;
  for (;;) {
    len = sizeof(buf);
    ret = kr_web_client_space(xf->client);
    if (ret < 0) return -1;
    if (ret < 1) {
      kr_web_client_want_space(xf->client);
      break;
    }
    if (ret < len) len = ret;
    if (!xf->uplink.read_cb) return -1;
    ret = xf->uplink.read_cb(xf->uplink.user, buf, len);
    if (ret <= 0) break;
    if (xf->info.client.uplink.legacy) {
      ret = kr_web_client_pack(xf->client, buf, ret);
    } else {
      ret = kr_web_client_pack_chunk(xf->client, buf, ret);
    }
    if (ret < 0) return -1;
    bytes += ret;
  }
  return bytes;
}
