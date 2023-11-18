//static int accept_data_ranges(kr_xfer_path *xf);
//static int accept_data(kr_xfer_path *xf);
//static int accept_header(kr_xfer_path *xf);
static int accept_handle(kr_xfer_path *xf);
static int accept_close(kr_xfer_path *xf);
static int accept_path(kr_xfer_path *xf, kr_path *addr, char *fp, size_t max);
static int accept_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr);
static int accepting_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr);

static size_t filepath_add_shortid(kr_hashids *hids, char *path,
 size_t sz, size_t len) {
  kr_hash_id hid;
  memset(&hid, 0, sizeof(hid));
  kr_hashids_gen(hids, &hid);
  return kr_hashids_filename(path, sz, len, &hid);
}

static int web_client_pack_ok(kr_web_client *client, char *path, size_t len) {
  int ret;
  kr_http_response res;
  kr_http_header hdr;
  kr_http_header ctype;
  uint8_t hdrbuf[512];
  size_t hdrsz;
  hdrsz = sizeof(hdrbuf);
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_http_length_header_init(&hdr, len, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  ctype.key = "Content-Type";
  ctype.key_len = 12;
  ctype.value = "text/plain";
  ctype.value_len = 10;
  kr_http_response_add_header(&res, &ctype);
  kr_web_client_pack_response(client, &res);
  ret = kr_web_client_pack(client, (uint8_t *)path, len);
  return ret;
}

static int accept_handle(kr_xfer_path *xf) {
  int ret;
  uint8_t *data;
  kr_http_response res;
  kr_accept_info *info;
  kr_web_client_info *winfo;
  kr_http_request *req;
  kr_http_header hdr;
  info = &xf->info.client.accept;
  ret = 0;
  winfo = kr_web_client_get_info(xf->client);
  if (winfo->http.input.chunked) {
    printk("XFER: it's a chunked transfer, aborting.");
    return -1;
  }
  if (xf->accept.sent_continue) {
    data = kr_file2_get_data(xf->accept.file);
    if (!data) return -1;
    ret = kr_web_client_unpack(xf->client, data + info->pos, info->len - info->pos);
    if (ret > 0) {
      info->pos += ret;
      kr_file2_wrote(xf->accept.file, ret);
      if (info->pos == info->len || winfo->http.input.multipart == KR_MULTIPART_DONE) {
        printk("XFER Accept: Fully unpacked");
        kr_file2_truncate(xf->accept.file, info->pos);
        info->len = info->pos;
        if (xf->server->accepted_cb) {
          xf->server->accepted_cb(xf->accept.file, xf->server->user);
        }
        kr_file2_close(xf->accept.file);
        memset(&xf->accept, 0, sizeof(xf->accept));
        web_client_pack_ok(xf->client, info->path, info->path_len);
        xf->done = 1;
      }
    }
  } else {
    req = kr_web_client_get_request(xf->client);
    ret = kr_http_get_header(&hdr, "Expect", 6, req);
    if (ret == 0 && hdr.value_len > 11 && (memcmp("100", hdr.value, 3) == 0)) {
      printk("XFER Accept: Client wants to upload, sending continue.");
      kr_http_response_init(&res, KR_HTTP_CONTINUE);
      ret = kr_web_client_pack_response(xf->client, &res);
    }
    xf->accept.sent_continue = 1;
    return 0;
  }
  return ret;
}

static int accept_close(kr_xfer_path *xf) {
  int ret;
  kr_accept_info *info;
  info = &xf->info.client.accept;
  ret = 0;
  if (xf->accept.file) {
    if (info->pos != info->len) {
      printk("Upload failed: unlinking file.");
      kr_file2_unlink(xf->accept.file);
      return ret;
    } else {
      ret = kr_file2_close(xf->accept.file);
    }
  }
  memset(&xf->accept, 0, sizeof(xf->accept));
  return ret;
}

static int accept_path(kr_xfer_path *xf, kr_path *addr, char *fp, size_t max) {
  int len;
  len = snprintf(fp, max, "%.*s", xf->server->info.server.acceptor.dir.len,
   xf->server->info.server.acceptor.dir.path);
  kr_path_clear_first(addr);
  kr_path_unparse(fp + len, addr, max - len);
  len = strlen(fp);
  return len;
}

static int accept_gen_upload_path(kr_xfer_path *xf, char *path, size_t len) {
  char *p;
  char *up_root;
  size_t ur_len;
  kr_accept_info *info;
  info = &xf->info.client.accept;
  up_root = getenv("KRAD_XFER_INCOMING");
  if (!up_root) return -1;
  ur_len = strlen(up_root);
  if (len <= ur_len) return -2;
  p = memmem(path, len, up_root, ur_len);
  if (!p) return -3;
  p = p + ur_len;
  if (p[0] == '/') {
    p++;
    len--;
  }
  len -= ur_len;
  info->path_len = snprintf(info->path, sizeof(info->path),
    "%.*s", (int)(len), p);
  return 0;
}

static int accept_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  size_t sz;
  kr_accept_info *info;
  kr_web_client_info *winfo;
  char fullpath[512];
  int len;
  memset(fullpath, 0, sizeof(fullpath));
  xf->info.client.type = KR_ACCEPT;
  info = &xf->info.client.accept;
  winfo = kr_web_client_get_info(xf->client);
  printk("accept client setup here");
  if (winfo->http.input.chunked) {
    printk("XFER: it's a chunked transfer, aborting.");
    return -1;
  }
  kr_path_unparse(xf->info.client.path, addr, sizeof(xf->info.client.path));
  len = accept_path(xf, addr, fullpath, sizeof(fullpath));
  if (winfo->http.input.sz <= 0) {
    printk("accept client setup: http content length is 0.");
    return -1;
  }
  sz = winfo->http.input.sz;
  len = filepath_add_shortid(xf->xfer->hids, fullpath, sizeof(fullpath), len);
  accept_gen_upload_path(xf, fullpath, len);
  xf->accept.file = kr_file2_create(xf->xfer->fs, fullpath, len, sz);
  if (!xf->accept.file) {
    printke("XFER: accept handler.. couldn't create the file %.*s",
     len, fullpath);
    return -1;
  }
  printk("XFER: accept handler.. created file %.*s", len, fullpath);
  info->pos = 0;
  info->len = sz;
  return 0;
}

static int accepting_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int len;
  int cnum;
  char *root;
  if (m != KR_HTTP_POST && m != KR_HTTP_PUT) return 0;
  /* need to check that incoming header does have a content size */
  len = kr_path_cur_name(addr, &root);
  if (len == xf->info.server.len) {
    if (memcmp(xf->info.server.path, root, len) == 0) {
      cnum = kr_path_steps_ahead(addr);
      /* Checking here that there is only one more path component at most,
         just the filename (for non multipart), to support uploading into
         subdirs directly we need to ensure they exist... */
      if (cnum > 1) return 0;
      return 1;
    }
  }
  return 0;
}

static int accepting_setup(kr_xfer_server_info *info) {
  if (info->acceptor.type != KR_XFER_ACCEPT_DIR) return 1;
  if (!kr_dir_exists(info->acceptor.dir.path)) {
    printke("XFER: no such dir");
    return 1;
  }
  return 0;
}
