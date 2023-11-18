static void broadcast_unsuppress(kr_radio *r);
static void broadcast_suppress(radio_client *rc, kr_crate *crate);
static radio_client *broadcast_reject(kr_radio *r, char *addr, int len);
static void test_broadcast_raw(kr_radio *radio, radio_client *reject,
 uint8_t *buf, size_t len);
static void test_broadcast_create(kr_radio *radio, kr_crate *crate);
static void test_broadcast_patch(kr_radio *r, char *p1, kr_name *name,
 kr_patchset *ps);
static void test_broadcast_delete(kr_radio *r, char *p1, kr_name *name);

static void broadcast_unsuppress(kr_radio *r) {
  r->api.exclude.client = NULL;
}

static void broadcast_suppress(radio_client *rc, kr_crate *crate) {
  kr_radio *r;
  r = rc->radio;
  printkd("Broadcast: suppress %s %zu", crate->address, crate->addr_len);
  if (crate->method == KR_PUT || crate->method == KR_PATCH
   || crate->method == KR_DELETE) {
    r->api.exclude.address = crate->address;
    r->api.exclude.addr_len = crate->addr_len;
    r->api.exclude.client = rc;
  }
}

static radio_client *broadcast_reject(kr_radio *r, char *addr, int len) {
  radio_client *rc;
  rc = NULL;
  printkd("Broadcast: reject %*s %d", len, addr, len);
  if (r->api.exclude.client && r->api.exclude.addr_len == len) {
    if (memcmp(r->api.exclude.address, addr, len) == 0) {
      rc = r->api.exclude.client;
      broadcast_unsuppress(r);
    }
  }
  return rc;
}

static void test_broadcast_raw(kr_radio *radio, radio_client *reject,
 uint8_t *buf, size_t len) {
  int i;
  ssize_t space;
  radio_client *rc;
  kr_web_client *wc;
  kr_web_client_info *info;
  i = 0;
  while ((rc = kr_pool_iterate_active(radio->clients, &i))) {
    if (rc == reject) continue;
    if ((rc->info.type == KR_RADIO_CLIENT_WEB) &&
     (rc->info.web.protocol == KR_PROTOCOL_WEBSOCKET)) {
      wc = kr_web_path_get_client(rc->path);
      info = kr_web_client_get_info(wc);
      if (info->ws.proto != KR_WS_PROTOCOL_KRAD) continue;
      space = kr_web_client_space(wc);
      if (space < len) {
        printke("API Broadcast Web Client: Not enough frakin room in out buf!");
        continue;
      }
      kr_web_client_pack(wc, buf, len);
    }
  }
}

static void test_broadcast_create(kr_radio *radio, kr_crate *crate) {
  int len;
  char json[8192];
  radio_client *rc;
  len = kr_crate_pack_json(json, crate, sizeof(json));
  rc = broadcast_reject(radio, crate->address, crate->addr_len);
  if (len > 0) test_broadcast_raw(radio, rc, (uint8_t *)json, len);
}

static void test_broadcast_patch(kr_radio *r, char *p1, kr_name *name,
 kr_patchset *ps) {
  int ret;
  char json[2048];
  char str[1024];
  char addr[256];
  int addr_len;
  radio_client *rc;
  if (!name) {
    addr_len = snprintf(addr, sizeof(addr), "/%s", p1);
  } else {
    addr_len = snprintf(addr, sizeof(addr), "/%s/", p1);
    addr_len += kr_name_snprint(name, addr + addr_len, sizeof(addr) - addr_len);
    if (addr_len >= sizeof(addr)) return;
  }
  rc = broadcast_reject(r, addr, addr_len);
  ret = kr_patchset_to_json(json, sizeof(json), ps);
  if (ret < 0) {
    printke("Broadcast PATCH: kr_patchset_to_json failed %d", ret);
    return;
  }
  ret = snprintf(str, sizeof(str), "{\"meth\":\"PATCH\",\"path\":\"%*s\""
   ",\"meta\":{},\"data\":%.*s}", addr_len, addr, ret, json);
  if (ret < sizeof(str)) test_broadcast_raw(r, rc, (uint8_t *)str, ret);
}

static void test_broadcast_patch2(kr_radio *r, kr_broadcast_data *bd) {
  int ret;
  int addr_len;
  char json[2048];
  char str[2048];
  ret = kr_patchset_to_json(json, sizeof(json), &bd->ps);
  if (ret < 0) {
    printke("Broadcast PATCH2: kr_patchset_to_json failed %d", ret);
    return;
  }
  addr_len = bd->addr_len;
  ret = snprintf(str, sizeof(str), "{\"meth\":\"PATCH\",\"path\":\"%*s\""
   ",\"meta\":{},\"data\":%.*s}", addr_len, bd->addr, ret, json);
  if (ret < sizeof(str)) test_broadcast_raw(r, NULL, (uint8_t *)str, ret);
}

static void queue_broadcast_patch(kr_radio *r, char *p1, kr_name *name,
 kr_patchset *ps) {
  int ret;
  kr_queue_item item;
  kr_broadcast_data *bd;
  kr_queue_producer *p;
  bd = kr_pool_atomic_slice(r->broadcasts);
  if (!bd) return;
  if (!name) {
    bd->addr_len = snprintf(bd->addr, sizeof(bd->addr), "/%s", p1);
  } else {
    bd->addr_len = snprintf(bd->addr, sizeof(bd->addr), "/%s/", p1);
    bd->addr_len += kr_name_snprint(name, bd->addr + bd->addr_len,
     sizeof(bd->addr) - bd->addr_len);
    if (bd->addr_len >= sizeof(bd->addr)) {
      kr_pool_atomic_release(r->broadcasts, bd);
      return;
    }
  }
  bd->ps = *ps;
  p = kr_queue_get_producer(r->events, KR_MIXER);
  if (!p) {
    kr_pool_atomic_release(r->broadcasts, bd);
    return;
  }
  item.ptr = bd;
  item.user = KR_MIXER;
  ret = kr_queue_write(p, &item);
  if (ret) printke("Queue broadcast fail");
  //printk("broadcasted: %p", bd);
}

static void test_broadcast_delete(kr_radio *r, char *p1, kr_name *name) {
  int ret;
  char str[256];
  char addr[256];
  int addr_len;
  radio_client *rc;
  addr_len = snprintf(addr, sizeof(addr), "/%s/", p1);
  addr_len += kr_name_snprint(name, addr + addr_len, sizeof(addr) - addr_len);
  if (addr_len >= sizeof(addr)) return;
  rc = broadcast_reject(r, addr, addr_len);
  ret = snprintf(str, sizeof(str), "{\"meth\":\"DELETE\",\"path\":\"%*s\"}",
   addr_len, addr);
  if (ret < sizeof(str)) test_broadcast_raw(r, rc, (uint8_t *)str, ret);
}
