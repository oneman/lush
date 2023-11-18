static int download_setup(kr_xfer_path *path, kr_download_info *info) {
  int ret;
  kr_http_url url;
  kr_file_set *fs;
  kr_web_path_setup_info wp_setup;
  if (!path || !info) return -1;
  if (info->type != KR_DOWNLOAD_FILE) return -2;
  memset(&wp_setup, 0, sizeof(wp_setup));
  memset(&url, 0, sizeof(url));
  wp_setup.type = KR_WEB_CLIENT;
  wp_setup.client.protocol = KR_PROTOCOL_HTTP;
  ret = kr_http_url_parse(&url, info->file.url, info->file.url_len);
  if (ret != 0) return -3;
  memcpy(wp_setup.client.remote.host, url.host, url.host_len);
  wp_setup.client.remote.port = url.port;
  fs = kr_xfer_get_file_set(path->xfer);
  if (!fs) return -4;
  ret = kr_file2_exists(info->file.path, info->file.path_len);
  if (ret < 0) return -5;
  if (ret == 1) {
    path->download.file = kr_file2_open(fs, info->file.path,
     info->file.path_len);
    if (!path->download.file) return -6;
  }
  ret = kr_web_path_create(path->xfer->web, &wp_setup, path, web_event);
  if (ret != 0) return -7;
  path->info.type = KR_XFER_CLIENT;
  path->info.client.type = KR_DOWNLOAD;
  path->info.client.download = *info;
  path->download.url = url;
  path->download.type = info->type;
  run_event(path, KR_XFER_CREATE);
  return 0;
}

static int download_close(kr_xfer_path *xf) {
  printk("download_close");
  if (xf->download.type == KR_DOWNLOAD_FILE) kr_file2_close(xf->download.file);
  memset(&xf->download, 0, sizeof(xf->download));
  return 0;
}

static int download_handle(kr_xfer_path *xf) {
  int ret;
  uint8_t *data;
  kr_file_set *fs;
  kr_http_response *res;
  kr_http_request req;
  kr_http_header hdr;
  printk("download_handle");
  if (xf->download.type != KR_DOWNLOAD_FILE) return -1;
  if (!xf->download.headers) {
    memset(&req, 0, sizeof(req));
    memset(&hdr, 0, sizeof(hdr));
    req.req_line.method = KR_HTTP_GET;
    req.req_line.path = xf->download.url.path;
    req.req_line.path_len = xf->download.url.path_len;
    req.req_line.version = KR_HTTP_VERSION_1_1;
    req.req_line.param_count = xf->download.url.param_count;
    memcpy(req.req_line.param, xf->download.url.param,
     sizeof(req.req_line.param));
    hdr.key = "Host";
    hdr.key_len = 4;
    hdr.value = xf->download.url.host;
    hdr.value_len = xf->download.url.host_len;
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
    hdr.key = "Connection";
    hdr.key_len = 10;
    hdr.value = "close";
    hdr.value_len = 5;
    kr_http_request_add_header(&req, &hdr);
    ret = kr_web_client_pack_request(xf->client, &req);
    if (ret <= 0) return -2;
    xf->download.headers = 1;
  } else {
    if (xf->download.sz == 0) {
      res = kr_web_client_get_response(xf->client);
      if (!res) return -3;
      ret = kr_http_response_get_header(&hdr, "Content-Length", 14, res);
      if (ret == 0) {
        printk("Found content length of %.*s", hdr.value_len, hdr.value);
        ret = kr_strtou64(&xf->download.sz, hdr.value, hdr.value_len);
        if (ret != 0) return -4;
      } else {
        printk("download_handle: No content length found.");
        return -5;
      }
      if (!xf->download.file) {
        fs = kr_xfer_get_file_set(xf->xfer);
        if (!fs) return -6;
        xf->download.file = kr_file2_create(fs,
         xf->info.client.download.file.path,
          xf->info.client.download.file.path_len, xf->download.sz);
        if (!xf->download.file) return -7;
      }
    }
    data = kr_file2_get_data(xf->download.file);
    if (!data) return -8;
    ret = kr_web_client_unpack(xf->client, data + xf->download.pos,
     xf->download.sz - xf->download.pos);
    if (ret < 0) return -9;
    xf->download.pos += ret;
    if (xf->download.sz == xf->download.pos) xf->done = 1;
  }
  return 0;
}
