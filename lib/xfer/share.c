static int serve_data_ranges(kr_xfer_path *xf);
static int serve_data(kr_xfer_path *xf);
static int serve_index(kr_xfer_path *xf);
static int serve_header(kr_xfer_path *xf);
static int serve_handle(kr_xfer_path *xf);
static int serve_close(kr_xfer_path *xf);
static int serve_path(kr_xfer_path *xf, kr_path *addr, char *fp, size_t max);
static int serve_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr);
static int sharing_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr);

#include <krad/gen/mime.h>

static int gzip_encode(kr_xfer_path *xf, uint8_t *buf, size_t sz) {
  int ret;
  kr_gzip_data data;
  memset(&data, 0, sizeof(data));
  if (!buf || !sz) return -1;
  data.in = buf;
  data.in_sz = sz;
  data.out = xf->server->encoding.encoded;
  data.out_sz = xf->server->encoding.sz;
  ret = kr_gzip_deflate(xf->server->encoding.gz, &data, 1);
  if (ret != 0) {
    printke("Xfer Serve: gzip deflating failed! %d", ret);
    return -1;
  }
  printk("Xfer Serve: gzipped %0.2f KBs --> %0.2f KBs",
   (double)data.in_sz / 1024.00f, (double)data.wrote / 1024.00f);
  xf->server->encoding.len = data.wrote;
  xf->info.client.serve.full.len = data.wrote;
  xf->info.client.serve.full.pos = 0;
  data.in_sz = 0;
  return xf->server->encoding.len;
}

static int serve_data_ranges(kr_xfer_path *xf) {
  ssize_t packed;
  size_t pos;
  size_t len;
  ssize_t ret;
  int i;
  uint8_t *buf;
  kr_serve_info *info;
  kr_serve_range_info *xr;
  info = &xf->info.client.serve;
  packed = 0;
  ret = 0;
  buf = kr_file2_get_data(xf->serve.file);
  if (!buf) return -1;
  for (i = 0; i < info->ranges.nranges; i++) {
    xr = &info->ranges.range[i];
    pos = info->ranges.pos;
    if (pos >= xr->start && pos <= xr->end) {
      len = 1 + (xr->end - pos);
      ret = kr_web_client_pack(xf->client, buf + pos, len);
      if (ret < 1) return ret;
      packed += ret;
      if (ret == len) {
        if (i + 1 == info->ranges.nranges) {
          serve_close(xf);
          break;
        }
        info->ranges.pos = info->ranges.range[i + 1].start;
      } else {
        info->ranges.pos += ret;
        break;
      }
    }
  }
  return packed;
}

static int serve_data(kr_xfer_path *xf) {
  size_t pos;
  size_t len;
  ssize_t ret;
  uint8_t *buf;
  kr_serve_info *info;
  ret = 0;
  info = &xf->info.client.serve;
  len = info->full.len;
  pos = info->full.pos;
  if (xf->serve.gzip) {
    buf = xf->server->encoding.encoded;
  } else {
    buf = kr_file2_get_data(xf->serve.file);
  }
  if (!buf) return -1;
  ret = kr_web_client_pack(xf->client, buf + pos, len - pos);
  if (ret > 0) {
    info->full.pos += ret;
    if (info->full.pos == info->full.len) {
      printk("XFER Serve: Fully packed");
      if (xf->serve.gzip) {
        xf->server->encoding.len = 0;
        xf->serve.gzip = 0;
      }
      serve_close(xf);
    }
  }
  return ret;
}

static int serve_index(kr_xfer_path *xf) {
  int ret;
  int res;
  int pos;
  int wrote;
  size_t len;
  kr_dir_entry entry;
  kr_gzip_data data;
  char str[1024];
  snprintf(str, sizeof(str), "<head><meta charset='utf-8'></head>"
    "<h1>Index of %s\n</h1>", xf->info.client.path);
  len = strlen(str);
  if (xf->serve.gzip) {
    memset(&data, 0, sizeof(data));
    data.in = (uint8_t *)str;
    data.in_sz = len;
    data.out = xf->server->encoding.encoded;
    data.out_sz = xf->server->encoding.sz;
    ret = kr_gzip_deflate(xf->server->encoding.gz, &data, 0);
    if (ret < 0) return -1;
    len = data.wrote;
    ret = kr_web_client_pack_chunk(xf->client, xf->server->encoding.encoded,
     data.wrote);
  } else {
    ret = kr_web_client_pack_chunk(xf->client, (uint8_t *)str, len);
  }
  if (ret < len) {
    serve_close(xf);
    return ret;
  }
  wrote = ret;
  for (;;) {
    pos = 0;
    ret = kr_dir_get_entry(&xf->serve.dir, &entry);
    if (ret == 0) {
      if (!xf->serve.gzip) {
        kr_web_client_pack_chunking_trailer(xf->client);
        serve_close(xf);
      }
      break;
    }
    if (ret != 1) {
      fprintf(stderr, "kr_dir_get_entry ret %d\n", ret);
      kr_dir_iter(&xf->serve.dir);
      continue;
    }
    if (kr_dir_entry_is_file(&entry)) {
      pos = snprintf(str, sizeof(str), "<li><a href=\"%s\">%s</a></li>",
       entry.name, entry.name);
      /*printf("\t\t\t%zd bytes", entry.sz);*/
    } else if (kr_dir_entry_is_dir(&entry)) {
      pos = snprintf(str, sizeof(str), "<li><a href=\"%s/\">%s/</a></li>",
       entry.name, entry.name);
    }
    snprintf(str + pos, sizeof(str) - pos, "\n");
    len = strlen(str);
    if (xf->serve.gzip) {
      memset(&data, 0, sizeof(data));
      data.in = (uint8_t *)str;
      data.in_sz = len;
      data.out = xf->server->encoding.encoded;
      data.out_sz = xf->server->encoding.sz;
      ret = kr_gzip_deflate(xf->server->encoding.gz, &data, 0);
      if (ret < 0) return -1;
      if (data.wrote > 0) {
        ret = kr_web_client_pack_chunk(xf->client, xf->server->encoding.encoded,
          data.wrote);
        if (ret < 0) return -1;
        wrote += ret;
      }
      kr_dir_iter(&xf->serve.dir);
      continue;
    } else {
      ret = kr_web_client_pack_chunk(xf->client, (uint8_t *)str, len);
    }
    if (ret < len) {
      serve_close(xf);
      return wrote;
    }
    wrote += ret;
    kr_dir_iter(&xf->serve.dir);
  }
  if (xf->serve.gzip) {
    do {
      data.in_sz = 0;
      ret = kr_gzip_deflate(xf->server->encoding.gz, &data, 1);
      if (ret < 0) break;
      res = kr_web_client_pack_chunk(xf->client,
       xf->server->encoding.encoded, data.wrote);
      if (res < 0) break;
    } while (ret == 1);
    if (ret < 0) return -1;
    xf->serve.gzip = 0;
    kr_web_client_pack_chunking_trailer(xf->client);
    serve_close(xf);
  }
  return wrote;
}

static int serve_redirect(kr_xfer_path *xf) {
  int ret;
  char path[512];
  size_t len;
  kr_http_response res;
  kr_http_request *req;
  kr_http_header hdr;
  printk("serve_redirect");
  memset(&res, 0, sizeof(res));
  req = kr_web_client_get_request(xf->client);
  kr_http_response_init(&res, KR_HTTP_MOVED);
  len = req->req_line.path_len;
  if (len >= sizeof(path)) return -1;
  memcpy(path, req->req_line.path, len);
  path[len++] = '/';
  hdr.key = "Location";
  hdr.key_len = 8;
  hdr.value = path;
  hdr.value_len = len;
  kr_http_response_add_header(&res, &hdr);
  kr_http_response_zero_content(&res);
  ret = kr_web_client_pack_response(xf->client, &res);
  if (ret < 0) return -2;
  return ret;
}

static int serve_header(kr_xfer_path *xf) {
  int ret;
  kr_serve_info *info;
  kr_http_response_type type;
  kr_http_header hdr;
  kr_http_date_header dh;
  kr_http_date_header lmh;
  kr_http_response res;
  kr_file2_info file_info;
  char *ext;
  uint8_t lbuf[128];
  uint8_t rbuf[128];
  info = &xf->info.client.serve;
  if (info->type == KR_SERVE_REDIRECT) {
    ret = serve_redirect(xf);
    return ret;
  }
  if (info->type == KR_SERVE_RANGES) {
    type = KR_HTTP_PARTIAL;
  } else {
    type = KR_HTTP_OK;
  }
  kr_http_response_init(&res, type);
  kr_http_date_header_now(&dh);
  kr_http_response_add_header(&res, &dh.hdr);
  kr_http_response_add_version_rand(&res);
  kr_http_response_any_origin(&res);
  kr_http_response_accept_ranges(&res);
  if (info->type != KR_SERVE_INDEX) {
    kr_file2_get_info(xf->serve.file, &file_info);
    kr_http_lastmod_header_at(&lmh, &file_info.lastmod);
    kr_http_response_add_header(&res, &lmh.hdr);
    ext = strrchr(xf->info.client.path, '.');
    if (ext) {
      hdr.value = ext_to_mime(ext + 1);
      if (hdr.value != NULL) {
        hdr.key = "Content-Type";
        hdr.key_len = strlen(hdr.key);
        hdr.value_len = strlen(hdr.value);
        kr_http_response_add_header(&res, &hdr);
      }
    }
  }
  if (info->type == KR_SERVE_FULL) {
    if (xf->serve.gzip) {
      ret = gzip_encode(xf, kr_file2_get_data(xf->serve.file), info->full.len);
      if (ret < 0) {
        xf->serve.gzip = 0;
        kr_http_length_header_init(&hdr, info->full.len, lbuf, sizeof(lbuf));
      } else {
        kr_http_response_content_encoding(&res);
        kr_http_length_header_init(&hdr, ret, lbuf, sizeof(lbuf));
      }
    } else {
      kr_http_length_header_init(&hdr, info->full.len, lbuf, sizeof(lbuf));
    }
    kr_http_response_add_header(&res, &hdr);
  } else if (info->type == KR_SERVE_RANGES) {
    if (info->ranges.nranges == 1) {
      kr_http_range_header_init(&hdr, info->ranges.range[0].start,
       info->ranges.range[0].end, info->ranges.sz, rbuf, sizeof(rbuf));
      kr_http_response_add_header(&res, &hdr);
      kr_http_length_header_init(&hdr, info->ranges.len, lbuf, sizeof(lbuf));
      kr_http_response_add_header(&res, &hdr);
    } else {
      printke("XFER Serve: multipart ranges not implemented");
      return -1;
    }
  } else if (info->type == KR_SERVE_INDEX) {
    hdr.key = "Content-Type";
    hdr.key_len = strlen(hdr.key);
    hdr.value = "text/html";
    hdr.value_len = strlen(hdr.value);
    kr_http_response_add_header(&res, &hdr);
    kr_http_response_transfer_chunked(&res);
    if (xf->serve.gzip) kr_http_response_content_encoding(&res);
  }
  ret = kr_web_client_pack_response(xf->client, &res);
  return ret;
}

static int serve_handle(kr_xfer_path *xf) {
  ssize_t ret;
  kr_serve_info *info;
  info = &xf->info.client.serve;
  if (!xf->serve.sent_header) {
    ret = serve_header(xf);
    if (ret > 0) {
      xf->info.client.tx += ret;
      xf->serve.sent_header = 1;
    } else {
      return -1;
    }
  }
  ret = 0;
  if (info->type == KR_SERVE_FULL) {
    ret = serve_data(xf);
  } else if (info->type == KR_SERVE_RANGES) {
    ret = serve_data_ranges(xf);
  } else if (info->type == KR_SERVE_INDEX) {
    ret = serve_index(xf);
  } else if (info->type == KR_SERVE_REDIRECT) {
    serve_close(xf);
    return 0;
  }
  if (ret > 0) {
    xf->info.client.tx += ret;
  } else {
    return -1;
  }
  return 0;
}

static int serve_close(kr_xfer_path *xf) {
  int ret;
  ret = 0;
  if (xf->serve.file) {
    ret = kr_file2_close(xf->serve.file);
  } else if (kr_dir_is_open(&xf->serve.dir)) {
    ret = kr_dir_close(&xf->serve.dir);
  }
  memset(&xf->serve, 0, sizeof(xf->serve));
  xf->done = 1;
  return ret;
}

static int serve_path(kr_xfer_path *xf, kr_path *addr, char *fp, size_t max) {
  int len;
  len = snprintf(fp, max, "%.*s", xf->server->info.server.share.dir.len,
   xf->server->info.server.share.dir.path);
  kr_path_clear_first(addr);
  kr_path_unparse(fp + len, addr, max - len);
  len = strlen(fp);
  return len;
}

static int resolve_ranges(kr_xfer_path *xf, kr_http_range *ranges, int cnt) {
  int i;
  int64_t sz;
  kr_serve_info *info;
  kr_file2_info file;
  kr_http_range *hr;
  kr_serve_range_info *xr;
  if (cnt > 1) {
    printke("XFER: multiple ranges unimplemented");
    return -1;
  }
  info = &xf->info.client.serve;
  kr_file2_get_info(xf->serve.file, &file);
  sz = file.sz;
  for (i = 0; i < cnt; i++) {
    hr = &ranges[i];
    xr = &xf->info.client.serve.ranges.range[i];
    if (hr->type == KR_HTTP_RANGE_START_FROM) {
      if (hr->start == 0) {
        xr->start = 0;
      } else if (hr->start > 0) {
        if (hr->start >= sz) return -1;
        xr->start = hr->start;
      } else {
        if ((hr->start + sz) < 0) return -1;
        xr->start = sz + hr->start;
      }
      xr->end = sz -1;
    } else {
      if (hr->end >= sz) return -1;
      xr->start = hr->start;
      xr->end = hr->end;
    }
    info->ranges.len += 1 + (xr->end - xr->start);
  }
  info->type = KR_SERVE_RANGES;
  info->ranges.nranges = cnt;
  info->ranges.pos = info->ranges.range[0].start;
  info->ranges.sz = sz;
  return 0;
}

static int serve_setup(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int ret;
  kr_serve_info *info;
  kr_http_request *req;
  kr_http_header hdr;
  kr_file2_info file_info;
  char fullpath[512];
  int len;
  int gzip;
  kr_http_range ranges[KR_XFER_NRANGES];
  xf->info.client.type = KR_SERVE;
  info = &xf->info.client.serve;
  req = kr_web_client_get_request(xf->client);
  gzip = 0;
  memset(&hdr, 0, sizeof(hdr));
  kr_http_get_header(&hdr, "Accept-Encoding", 15, req);
  if (hdr.value_len) {
    if (memmem((uint8_t *)hdr.value, hdr.value_len, (uint8_t *)"gzip", 4)) {
      gzip = 1;
    }
  }
  kr_path_unparse(xf->info.client.path, addr, sizeof(xf->info.client.path));
  len = serve_path(xf, addr, fullpath, sizeof(fullpath));
  if (kr_dir_exists(fullpath)) {
    if (req->req_line.path[req->req_line.path_len - 1] != '/') {
      info->type = KR_SERVE_REDIRECT;
      return 0;
    }
    ret = kr_dir_open(&xf->serve.dir, fullpath, len);
    if (ret) return -1;
    info->type = KR_SERVE_INDEX;
    info->index.cnt = 0;
    info->index.format = KR_SERVE_INDEX_HTML;
    xf->serve.gzip = gzip;
    return 0;
  }
  xf->serve.file = kr_file2_open(xf->xfer->fs, fullpath, len);
  if (!xf->serve.file) {
    printke("XFER: share handler.. didnt find the file! %.*s", len, fullpath);
    return -1;
  }
  kr_file2_get_info(xf->serve.file, &file_info);
  ret = kr_http_request_get_ranges(req, ranges, KR_XFER_NRANGES);
  if (ret > 0) {
    ret = resolve_ranges(xf, ranges, ret);
    if (ret) {
      serve_close(xf);
      return -1;
    }
  } else {
    xf->serve.gzip = gzip;
    info->type = KR_SERVE_FULL;
    info->full.pos = 0;
    info->full.len = file_info.sz;
  }
  printk("XFER: share handler.. we found the file! %.*s", len, fullpath);
  return 0;
}

static int sharing_match(kr_xfer_path *xf, kr_http_method m, kr_path *addr) {
  int len;
  char *root;
  if (m != KR_HTTP_GET) return 0;
  len = kr_path_cur_name(addr, &root);
  if (len == xf->info.server.len) {
    if (memcmp(xf->info.server.path, root, len) == 0) {
      return 1;
    }
  }
  return 0;
}

static int sharing_destroy(kr_xfer_path *xf) {
  if (xf->encoding.encoded) free(xf->encoding.encoded);
  if (xf->encoding.gz) free(xf->encoding.gz);
  return 0;
}

static int sharing_setup(kr_xfer_path *xf) {
  kr_gzip_setup gzip_setup;
  if (xf->info.server.share.type != KR_XFER_SHARE_DIR) return 1;
  if (!kr_dir_exists(xf->info.server.share.dir.path)) {
    printke("XFER: no such dir");
    return 1;
  }
  memset(&gzip_setup, 0, sizeof(gzip_setup));
  gzip_setup.mem = kr_alloc(kr_gzip_sizeof());
  gzip_setup.type = KR_GZIP_DEFLATE;
  xf->encoding.gz = kr_gzip_init(&gzip_setup);
  if (!xf->encoding.gz) return 1;
  xf->encoding.sz = 1024 * 1024 * 2;
  xf->encoding.encoded = kr_alloc(xf->encoding.sz);
  if (!xf->encoding.encoded) return 1;
  return 0;
}
