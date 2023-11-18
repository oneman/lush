static int receiver_add_source(kr_stream_client *client, void *user);
static int receiver_del_source(kr_stream_client *client, void *user);
static ssize_t receiver_read_source(kr_stream_client *client, uint8_t *buf,
 size_t len, void *user);

static int receiver_add_source(kr_stream_client *client, void *user) {
  int ret;
  kr_radio *radio;
  kr_name name;
  kr_name *new_name;
  char *path;
  kr_studio_path_info info;
  kr_xfer_path_info xf_info;
  radio = (kr_radio *)user;
  printk("Receiver: Add Source CB!");
  memset(&name, 0, sizeof(name));
  memset(&info, 0, sizeof(info));
  kr_xfer_path_info_get(radio->receiver, &xf_info);
  name.len = snprintf(name.buf, sizeof(name.buf), "Source%d",
   xf_info.server.receiver.sources);
  new_name = kr_name_add(radio->api.names, &name);
  client->user = new_name;
  info.type = KR_STUDIO_SOURCE;
  info.source.type = KR_SOURCE_STREAM;
  info.source.stream.type = KR_SOURCE_STREAM_ACCEPT;
  path = (char *)client->mem;
  info.source.stream.len = strlen(path);
  memcpy(info.source.stream.path, path, info.source.stream.len);
  info.source.stream.num = xf_info.server.receiver.sources;
  ret = kr_studio_source_create(radio->studio, &info, new_name);
  if (ret != 0) printke("Receiver: studio_add failed!");
  return ret;
}

static int receiver_del_source(kr_stream_client *client, void *user) {
  printk("Receiver: Del Source CB!");
  int ret;
  kr_radio *radio;
  kr_name *name;
  kr_name_ctx *ctx;
  kr_studio_path *sp;
  radio = (kr_radio *)user;
  if (!client->user) return -1;
  name = (kr_name *)client->user;
  ctx = kr_name_resolve_type(radio->api.names, KR_STUDIO,
   name->buf, name->len);
  if (!ctx) return -2;
  sp = (kr_studio_path *)ctx->user;
  ret = kr_studio_source_destroy(sp);
  if (ret != 0) printke("Receiver: studio_remove failed!");
  return ret;
}

static ssize_t receiver_read_source(kr_stream_client *client, uint8_t *buf,
 size_t len, void *user) {
  ssize_t ret;
  kr_radio *radio;
  kr_name *name;
  kr_name_ctx *ctx;
  kr_studio_path *sp;
  printkd("Receiver: Want to read %zu bytes from Source", len);
  radio = (kr_radio *)user;
  if (!client->user) return -1;
  name = (kr_name *)client->user;
  ctx = kr_name_resolve_type(radio->api.names, KR_STUDIO,
   name->buf, name->len);
  if (!ctx) return -2;
  sp = (kr_studio_path *)ctx->user;
  ret = kr_studio_source_read(sp, buf, len);
  if (ret != len) printke("Receiver: source stream read failed %d", ret);
  return ret;
}
