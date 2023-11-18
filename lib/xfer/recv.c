static int source_handle(kr_xfer_path *xf);
static int source_close(kr_xfer_path *xf);
static int source_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr);
static int receiver_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr);

static int recv_source_data(kr_xfer_path *xf) {
  int ret;
  uint8_t buf[4096];
  for (;;) {
    ret = kr_web_client_unpack(xf->client, buf, sizeof(buf));
    printkd("XFER: recv_source_data %d bytes", ret);
    if (ret < 1) return ret;
    ret = xf->server->client_write(&xf->source, buf, ret, xf->server->user);
    if (ret < 0) break;
  }
  return ret;
}

static int source_handle(kr_xfer_path *xf) {
  int ret;
  printkd("XFER: source_handle");
  ret = recv_source_data(xf);
  return ret;
}

static int source_close(kr_xfer_path *xf) {
  int ret;
  kr_xfer_receiver_info *info;
  info = &xf->server->info.server.receiver;
  printk("XFER: source_close");
  ret = xf->server->del_client(&xf->source, xf->server->user);
  if (ret) {
    printke("Source: del ret %d", ret);
  } else {
    printk("XFER: Source close");
    if (info->sources > 0) info->sources--;
  }
  return 0;
}

static int source_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int ret;
  kr_web_client_info *winfo;
  kr_xfer_receiver_info *info;
  kr_http_request *req;
  kr_http_response res;
  kr_http_header hdr;
  memset(&res, 0, sizeof(res));
  xf->info.client.type = KR_SOURCE;
  winfo = kr_web_client_get_info(xf->client);
  info = &xf->server->info.server.receiver;
  printk("XFER: source_setup %d", info->sources);
  req = kr_web_client_get_request(xf->client);
  if (!req) return -1;
  if (req->req_line.path_len > sizeof(xf->source.mem)) return -2;
  memcpy(xf->source.mem, req->req_line.path, req->req_line.path_len);
  if (winfo->http.input.chunked) {
    printk("XFER: source stream is a chunker");
  }
  ret = xf->server->add_client(&xf->source, xf->server->user);
  if (ret) {
    printke("Source: setup ret %d", ret);
  } else {
    printk("XFER: Source setup");
  }
  ret = kr_http_get_header(&hdr, "Expect", 6, req);
  if (ret == 0) {
    if (hdr.value_len == 12 && !memcmp(hdr.value, "100-continue", 12)) {
      printk("XFER: sending HTTP 100-continue.");
      kr_http_response_init(&res, KR_HTTP_CONTINUE);
      kr_web_client_pack_response(xf->client, &res);
    }
  }
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_web_client_pack_response(xf->client, &res);
  /* ok so if this xfer client matched a receiver then we shall set things up
     to suck up its bytes */
  info->sources++;
  return 0;
}

static int receiver_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int len;
  int cnum;
  char *root;
  if (m != KR_HTTP_POST && m != KR_HTTP_PUT && m != KR_HTTP_SOURCE) return 0;
  /* need to check that incoming header does NOT have a content size */
  len = kr_path_cur_name(addr, &root);
  if (len == xf->info.server.len) {
    if (memcmp(xf->info.server.path, root, len) == 0) {
      cnum = kr_path_steps_ahead(addr);
      /* here we are saying, if the incoming source has a mount name,
         it should be at most one path component.. */
      if (cnum != 1) return 0;
      len = kr_path_next(addr, &root);
      if (len == 0) return 0;
      return 1;
    }
  }
  return 0;
}

static int receiver_setup(kr_xfer_server_info *info) {
  printk("XFER: receiver setup");
  info->receiver.sources = 0;
  return 0;
}
