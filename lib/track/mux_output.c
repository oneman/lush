static int output_track_event(kr_track_event *e);
static size_t read_header(kr_mux_output *out, uint8_t *buf, size_t sz);
static int need_header(kr_mux_output *out);
static size_t read_block(kr_mux_output *out, uint8_t *buf, size_t sz);

static int need_block(kr_mux_output *out);
static int output_track_event(kr_track_event *e) {
  if (e->type == KR_TRACK_READ) {
    printkd("Muxed output track read event");
  } else if (e->type == KR_TRACK_WRITE) {
    printkd("Muxed output track write event");
  } else if (e->type == KR_TRACK_REMOVE) {
    printkd("Mux Output: Track I was reading from was removed!");
  }
  return 0;
}

static size_t read_header(kr_mux_output *out, uint8_t *buf, size_t sz) {
  size_t len;
  len = sz;
  if (out->mux->header_sz - out->pos < sz) {
    len = out->mux->header_sz - out->pos;
  }
  memcpy(buf, out->mux->header + out->pos, len);
  out->pos += len;
  if (out->pos == out->mux->header_sz) {
    printk("Mux: header read %ld", out->pos);
    out->header_sent = 1;
    out->pos = 0;
  }
  return len;
}

static int need_header(kr_mux_output *out) {
  if (out->header_sent == 1) return 0;
  return 1;
}

static size_t read_block(kr_mux_output *out, uint8_t *buf, size_t sz) {
  kr_track_block *b;
  size_t len;
  b = &out->block;
  len = b->len - out->pos;
  if (sz < len) len = sz;
  memcpy(buf, b->data + out->pos, len);
  out->pos += len;
  if (out->pos == b->len) out->pos = 0;
  return len;
}

static int need_block(kr_mux_output *out) {
  if (out->pos == 0) return 1;
  return 0;
}

static ssize_t mux_read(kr_mux_output *out, uint8_t *buf, size_t sz) {
  ssize_t ret;
  size_t pos;
  kr_mux *m;
  m = out->mux;
  if (!m) return -1;
  ret = 0;
  pos = 0;
  printkd("Muxer: I'm being read for %zu bytes!", sz);
  if (need_header(out)) {
    pos += read_header(out, buf + pos, sz - pos);
  }
  while ((sz - pos) > 0) {
    if (need_block(out)) {
      ret = kr_track_read_lite(out->head, &out->block);
      if (ret != 1) break;
    }
    pos += read_block(out, buf + pos, sz - pos);
  }
  return pos;
}

ssize_t kr_mux_read_n(kr_mux_output *out, uint8_t *buf, size_t sz, int n) {
  if (!out || !buf || sz < 1 || n < 1) return -1;
  if (kr_track_head_offset(out->head) < n) {
    return 0;
  }
  return mux_read(out, buf, sz);
}

ssize_t kr_mux_read(kr_mux_output *out, uint8_t *buf, size_t sz) {
  if (!out || !buf || sz < 1) return -1;
  return mux_read(out, buf, sz);
}

int kr_mux_detach(kr_mux_output *out) {
  int ret;
  if (!out) return -1;
  ret = kr_track_close(out->head);
  if (ret) printke("kr_track_close ret %d", ret);
  memset(out, 0, sizeof(*out));
  printk("Mux: Detachment");
  return 0;
}

kr_mux_output *kr_mux_attach(kr_mux *m, kr_mux_output_setup *s) {
  kr_mux_output *out;
  kr_track_head_setup head_setup;
  if (!m || !s) return NULL;
  out = (kr_mux_output *)s->mem;
  memset(out, 0, sizeof(*out));
  memset(&head_setup, 0, sizeof(head_setup));
  out->mux = m;
  out->user = s->user;
  head_setup.user = out;
  head_setup.type = KR_TRACK_READ_HEAD;
  head_setup.track_ev_cb = output_track_event;
  head_setup.seek = s->backbuf;
  /* handling one track only atm */
  out->head = kr_track_open(m->output.track, &head_setup);
  if (!out->head) return NULL;
  printk("Mux: Attachment");
  return out;
}

size_t kr_mux_output_sizeof() {
  kr_mux_output *out;
  return sizeof(*out);
}
