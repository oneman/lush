static int radio_404(kr_web_client *client);
static int reply_http(kr_web_client *c, kr_response res, size_t len);
static void client_reply(radio_client *rc, kr_response res, kr_crate *crate);

static int tracker_track_list(kr_path_list *list, kr_tracker *tracker) {
  int i;
  kr_track *track;
  kr_track_info info;
  i = 0;
  memset(list, 0, sizeof(*list));
  while ((track = kr_tracker_iter(tracker, &i))) {
    kr_track_info_get(track, &info);
    memcpy(list->path[list->len].name, "/studio/tracker/", 16);
    memcpy(list->path[list->len++].name + 16, info.track.name, info.track.len);
  }
  return 0;
}

static int names_to_path_list(kr_path_list *list,
 kr_names *names, int ctx_type, char *ctx_str) {
  int i;
  size_t max;
  uint len;
  char *str;
  kr_name *result;
  memset(list, 0, sizeof(*list));
  list->len = 0;
  i = 0;
  while ((result = kr_names_iter(names, &i))) {
    if (kr_name_has_ctx_type(result, ctx_type)) {
      max = sizeof(list->path[0].name);
      str = &list->path[list->len].name[0];
      len = strlen(ctx_str);
      if (len) {
        *str++ = '/';
        max--;
        memcpy(str, ctx_str, len);
        max -= len;
        str += len;
      }
      *str++ = '/';
      max--;
      kr_name_snprint(result, str, max);
      list->len++;
    }
    if (list->len == sizeof(list->path) / sizeof(list->path[0])) break;
  }
  return 0;
}

static int radio_404(kr_web_client *client) {
  int ret;
  uint64_t sz;
  kr_http_header hdr;
  kr_http_response res;
  uint8_t hdrbuf[512];
  static const char *not_found;
  not_found = "NO CARRIER\n";
  sz = strlen(not_found);
  kr_http_response_init(&res, KR_HTTP_NOT_FOUND);
  kr_http_length_header_init(&hdr, sz, hdrbuf, sizeof(hdrbuf));
  kr_http_response_add_header(&res, &hdr);
  ret = kr_web_client_pack_response(client, &res);
  if (ret > 0) kr_web_client_pack(client, (uint8_t *)not_found, sz);
  return 0;
}

static int reply_http(kr_web_client *c, kr_response res, size_t len) {
  kr_http_response headers;
  kr_http_header hdr;
  kr_http_header ctype;
  uint8_t buf[512];
  kr_http_response_type type;
  type = kr_res_to_http_res_type(res);
  if (type == KR_HTTP_NOT_FOUND) return radio_404(c);
  printkd("Radio HTTP reply: res is %d and type is %d", res, type);
  kr_http_response_init(&headers, type);
  kr_http_response_any_origin(&headers);
  kr_http_length_header_init(&hdr, len, buf, sizeof(buf));
  ctype.key = "Content-Type";
  ctype.key_len = 12;
  ctype.value = "application/json";
  ctype.value_len = 16;
  kr_http_response_add_header(&headers, &ctype);
  kr_http_response_add_header(&headers, &hdr);
  kr_web_client_pack_response(c, &headers);
  return 0;
}

static size_t kr_path_list_pack_json_array(char *json,
 kr_path_list *list, size_t max) {
  int i;
  int res;
  res = 0;
  if (max < 3) return max;
  res += snprintf(json + res, max - res, "[");
  for (i = 0; i < list->len; i++) {
    res += snprintf(json + res, max - res, "\"%s\"", list->path[i].name);
    if (res >= max) return max;
    if (i != (list->len - 1)){
      res += snprintf(json + res, max - res, ",");
      if (res >= max) return max;
    }
  }
  res += snprintf(json + res, max - res, "]");
  return res;
}

static void client_reply(radio_client *rc, kr_response res, kr_crate *crate) {
  int ret;
  size_t len;
  ssize_t space;
  kr_web_client *wc;
  kr_radio_client_type ct;
  kr_web_client_protocol wp;
  kr_packing_plan plan;
  uint8_t data[8192 * 8];
  ct = rc->info.type;
  wp = rc->info.web.protocol;
  len = 0;
  ret = 0;
  memset(&plan, 0, sizeof(plan));
  plan.sz = sizeof(data);
  if (ct == KR_RADIO_CLIENT_LOCAL) {
    plan.format = KR_FORMAT_EBML;
    plan.buf = data;
  } else {
    plan.json = (char *)data;
    if (wp == KR_PROTOCOL_HTTP) {
      plan.format = KR_FORMAT_JSON_PAYLOAD;
    } else if (wp == KR_PROTOCOL_WEBSOCKET) {
      plan.format = KR_FORMAT_JSON;
    }
  }
  if (res == KR_INFORM) {
    crate->disposition = KR_RESPONSE;
    crate->response = KR_INFORM;
    if ( (crate->type == KR_PATH_LIST)
      && (plan.format == KR_FORMAT_JSON_PAYLOAD) ) {
      len = kr_path_list_pack_json_array((char *)data,
       &crate->info.path_list, sizeof(data));
      if (len == sizeof(data)) {
        printke("Radio: kr_path_list_pack_json_array failed!");
        return;
      }
    } else {
      len = kr_crate_pack(&plan, crate);
      if (len < 1) {
        printke("Radio: couldn't kr_crate_pack");
        return;
      }
    }
  }
  if (ct == KR_RADIO_CLIENT_LOCAL) {
    kr_app_client_write(rc->local_client, data, len);
    printk("Radio: packed %d local bytes", len);
    return;
  } else {
    wc = kr_web_path_get_client(rc->path);
    space = kr_web_client_space(wc);
    if (space < len) {
      printke("API Reply Web Client: Not enough frakin room in out buffer!");
      return;
    }
    if (wp == KR_PROTOCOL_HTTP) {
      reply_http(wc, res, len);
    }
    if (len > 0) ret = kr_web_client_pack(wc, data, len);
  }
  if (ret < 0) {
    printke("Radio: error packing web client crate");
  }
}
