static int upload_setup(kr_xfer_path *path, kr_upload_info *info) {
  int ret;
  kr_file_set *fs;
  kr_file2_info finfo;
  kr_http_url url;
  kr_web_path_setup_info wp_setup;
  if (!path || !info) return -1;
  if (info->type != KR_UPLOAD_FILE) return -2;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  memset(&finfo, 0, sizeof(finfo));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  ret = kr_http_url_parse(&url, info->file.url, info->file.url_len);
  if (ret != 0) return -3;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  ret = kr_web_path_create(path->xfer->web, &wp_setup, path, web_event);
  if (ret != 0) return -4;
  fs = kr_xfer_get_file_set(path->xfer);
  if (!fs) return -5;
  path->upload.file = kr_file2_open(fs, info->file.path, info->file.path_len);
  if (!path->upload.file) return -6;
  kr_file2_get_info(path->upload.file, &finfo);
  path->upload.type = info->type;
  path->upload.sz = finfo.sz;
  //path->user = user;
  path->info.type = KR_XFER_CLIENT;
  path->info.client.type = KR_UPLOAD;
  path->info.client.upload = *info;
  path->upload.url = url;
  run_event(path, KR_XFER_CREATE);
  return 0;
}

static int upload_close(kr_xfer_path *xf) {
  printk("upload_close");
  if (xf->upload.type == KR_UPLOAD_FILE) kr_file2_close(xf->upload.file);
  memset(&xf->upload, 0, sizeof(xf->upload));
  return 0;
}

static int upload_handle(kr_xfer_path *xf) {
  int ret;
  uint8_t *data;
  //char auth[256];
  char clength[64];
  kr_http_header hdr;
  kr_http_request req;
  if (xf->upload.type != KR_UPLOAD_FILE) return -1;
  if (!xf->upload.headers) {
    memset(&req, 0, sizeof(req));
    memset(&hdr, 0, sizeof(hdr));
    req.req_line.method = KR_HTTP_PUT;
    req.req_line.path = xf->upload.url.path;
    req.req_line.path_len = xf->upload.url.path_len;
    req.req_line.version = KR_HTTP_VERSION_1_1;
    req.req_line.param_count = xf->upload.url.param_count;
    memcpy(req.req_line.param, xf->upload.url.param,
     sizeof(req.req_line.param));
    hdr.key = "Host";
    hdr.key_len = 4;
    hdr.value = xf->upload.url.host;
    hdr.value_len = xf->upload.url.host_len;
    kr_http_request_add_header(&req, &hdr);
    hdr.key = "User-Agent";
    hdr.key_len = 10;
    hdr.value = "KRAD RADIO";
    hdr.value_len = 10;
    kr_http_request_add_header(&req, &hdr);
    hdr.key = "Connection";
    hdr.key_len = 10;
    hdr.value = "close";
    hdr.value_len = 5;
    kr_http_request_add_header(&req, &hdr);
    snprintf(clength, sizeof(clength), "%zu", xf->upload.sz);
    hdr.key = "Content-Length";
    hdr.key_len = 14;
    hdr.value = clength;
    hdr.value_len = strlen(clength);
    kr_http_request_add_header(&req, &hdr);
    ret = kr_web_client_pack_request(xf->client, &req);
    if (ret <= 0) return -1;
    xf->upload.headers = 1;
  }
  data = kr_file2_get_data(xf->upload.file);
  if (!data) return -2;
  ret = kr_web_client_pack(xf->client, data + xf->upload.pos,
   xf->upload.sz - xf->upload.pos);
  if (ret <= 0) return -3;
  xf->upload.pos += ret;
  if (xf->upload.sz == xf->upload.pos) xf->done = 1;
  return 0;
}
