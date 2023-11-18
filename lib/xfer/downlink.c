static int downlink_reconnect_event(kr_event *e) {
  int ret;
  kr_http_url url;
  kr_xfer_path *xf;
  kr_event ev;
  kr_web_path_setup_info wp_setup;
  memset(&ev, 0, sizeof(ev));
  xf = (kr_xfer_path *)e->user;
  kr_interval_update(xf->reconnect.clock);
  if (xf->reconnect.timeout) {
    printk("Xfer Downlink: time to try to reconnect again.");
    xf->reconnect.ms = xf->info.client.reconnect.ms;
    xf->reconnect.timeout = 0;
    xf->reconnect.tries = 1;
  }
  if (xf->reconnect.tries >= xf->info.client.reconnect.max_tries) {
    printk("Xfer Downlink: Max reconnection attempts exceeded,"
     " waiting now for 15 mins before trying again");
    xf->reconnect.tries = 0;
    xf->reconnect.ms = 0;
    xf->reconnect.timeout = 1;
    kr_interval_set(xf->reconnect.clock, 15 * 60 * 1000);
    return 0;
  }
  xf->downlink.headers = 0;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  ret = kr_http_url_parse(&url, xf->info.client.downlink.url,
   xf->info.client.downlink.len);
  if (ret != 0) return -2;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  ret = kr_web_path_create(xf->xfer->web, &wp_setup, xf, web_event);
  if (ret != 0) return -3;
  return 0;
}

static int downlink_setup(kr_xfer_path *path, kr_downlink_setup *setup) {
  int ret;
  kr_http_url url;
  kr_downlink_info info;
  kr_web_path_setup_info wp_setup;
  if (!path || !setup) return -1;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  memset(&info, 0, sizeof(info));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  memcpy(info.url, setup->url, sizeof(info.url));
  info.len = setup->len;
  ret = kr_http_url_parse(&url, info.url, info.len);
  if (ret != 0) return -2;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  ret = kr_web_path_create(path->xfer->web, &wp_setup, path, web_event);
  if (ret != 0) return -3;
  path->info.type = KR_XFER_CLIENT;
  path->info.client.type = KR_DOWNLINK;
  path->info.client.downlink = info;
  path->downlink.url = url;
  path->downlink.user = setup->user;
  path->downlink.write_cb = setup->write_cb;
  path->reconnect.connect_cb = setup->reconnect.connect_cb;
  path->reconnect.disconnect_cb = setup->reconnect.disconnect_cb;
  path->reconnect.cb = downlink_reconnect_event;
  path->reconnect.clock = NULL;
  path->info.client.reconnect.max_tries = setup->reconnect.max_tries;
  path->info.client.reconnect.ms = setup->reconnect.duration;
  return 0;
}

static int downlink_close(kr_xfer_path *xf) {
  kr_web_path *wpath;
  printk("downlink close");
  memset(&xf->downlink, 0, sizeof(xf->downlink));
  memset(&xf->reconnect, 0, sizeof(xf->reconnect));
  if (xf->client) {
    wpath = kr_web_client_get_path(xf->client);
    xf->client = NULL;
    kr_web_path_remove(wpath);
  }
  return 0;
}

static int downlink_handle(kr_xfer_path *xf) {
  int ret;
  kr_http_header hdr;
  kr_http_request req;
  uint8_t buffer[512];
  char auth[256];
  char auth_hdr[256];
  size_t auth_len;
  printkd("downlink handle");
  if (!xf->downlink.headers) {
    ret = kr_http_url_parse(&xf->downlink.url,
     xf->info.client.downlink.url, xf->info.client.downlink.len);
    if (ret != 0) return -1;
    memset(&req, 0, sizeof(req));
    memset(&hdr, 0, sizeof(hdr));
    req.req_line.method = KR_HTTP_GET;
    req.req_line.path = xf->downlink.url.path;
    req.req_line.path_len = xf->downlink.url.path_len;
    req.req_line.version = KR_HTTP_VERSION_1_1;
    req.req_line.param_count = xf->downlink.url.param_count;
    memcpy(req.req_line.param, xf->downlink.url.param,
     sizeof(req.req_line.param));
    hdr.key = "Host";
    hdr.key_len = 4;
    hdr.value = xf->downlink.url.host;
    hdr.value_len = xf->downlink.url.host_len;
    kr_http_request_add_header(&req, &hdr);
    hdr.key = "User-Agent";
    hdr.key_len = 10;
    hdr.value = "KRAD RADIO";
    hdr.value_len = 10;
    kr_http_request_add_header(&req, &hdr);
    hdr.key = "Accept";
    hdr.key_len = 6;
    hdr.value = "*/*";
    hdr.value_len = 3;
    kr_http_request_add_header(&req, &hdr);
    if (xf->downlink.url.user && xf->downlink.url.pwd) {
      ret = kr_base64((uint8_t *)auth,
        (uint8_t *)xf->downlink.url.user, xf->downlink.url.user_len +
         xf->downlink.url.pwd_len + 1, sizeof(auth));
      if (ret <= 0) return -2;
      auth_len = snprintf(auth_hdr, sizeof(auth_hdr),
        "Basic %.*s", ret, auth);
      if (auth_len >= sizeof(auth_hdr)) return -3;
      hdr.key = "Authorization";
      hdr.key_len = 13;
      hdr.value = auth_hdr;
      hdr.value_len = auth_len;
      kr_http_request_add_header(&req, &hdr);
    }
    ret = kr_web_client_pack_request(xf->client, &req);
    if (ret <= 0) return -4;
    xf->downlink.headers = 1;
  }
  for (;;) {
    ret = kr_web_client_unpack(xf->client, buffer, sizeof(buffer));
    if (ret < 0) return -5;
    xf->downlink.bytes += ret;
    if (xf->downlink.write_cb) {
      ret = xf->downlink.write_cb(xf->downlink.user, buffer, ret);
      if (ret < 0) return -6;
    }
    if (ret == 0) break;
  }
  return 0;
}
