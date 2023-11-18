#include "pack.h"

typedef struct {
  uint64_t nblocks;
  kr_codec_header header;
  kr_track_info info;
} txt_track;

struct kr_txt_muxer {
  uint64_t nblocks;
  int have_video_track;
  int tracks;
  txt_track track[KR_TXT_MUXER_NTRACKS];
  char comment[KR_TXT_COMMENT_SZ];
  int comment_len;
};

static size_t header_sz(kr_codec_header *hdr) {
  int i;
  size_t sz;
  sz = 0;
  for (i = 0; i < hdr->len; i++) {
    sz += hdr->data[i].sz;
  }
  return sz;
}

ssize_t kr_txt_mux(kr_txt_muxer *m, kr_txt_muxer_data *data) {
  ssize_t res;
  size_t off;
  char *s;
  ssize_t sz;
  if (!m || !data) return -1;
  if (data->out.space < 1 || !data->out.framing) return -2;
  if (data->track >= m->tracks) return -3;
  data->out.sync = 0;
  off = 0;
  sz = data->out.space;
  s = (char *)data->out.framing;
  if (m->track[data->track].info.codec.type == KR_CODEC_VIDEO &&
    data->in.keyframe) {
    data->out.sync = 1;
    res = snprintf(s + off, sz - off, "K ");
    if (res > (sz - off)) return -3;
    off += res;
  } else {
    res = snprintf(s + off, sz - off, "  ");
    if (res > (sz - off)) return -3;
    off += res;
  }
  res = snprintf(s + off, sz - off, "[%d] #%08zu :: %zu bytes",
   data->track, m->track[data->track].nblocks++, data->in.sz);
  if (res > (sz - off)) return -3;
  off += res;
  if (m->track[data->track].info.codec.type == KR_CODEC_AUDIO) {
    res = snprintf(s + off, sz - off, " %d frames",
     data->in.nframes);
    if (res > (sz - off)) return -3;
    off += res;
  }
  res = snprintf(s + off, sz - off, "\n");
  if (res > (sz - off)) return -3;
  off += res;
  data->out.used = off;
  m->nblocks++;
  if (!m->have_video_track && ((m->nblocks % 6) == 0)) {
    data->out.sync = 1;
  }
  return data->out.used;
}

ssize_t kr_txt_muxer_pack_header(kr_txt_muxer *m, uint8_t *buf, size_t sz) {
  int i;
  int r;
  int ret;
  char *s;
  if (!m || !buf || sz < 1) return -1;
  s = (char *)buf;
  r = sz;
  ret = snprintf(s, r, "KRAD MUXTEXT\nVERSION %d\nCOMMENT: %.*s\n%d TRACKS\n",
   KR_TXT_VERSION, m->comment_len, m->comment, m->tracks);
  if (ret > r) return -1;
  r -= ret;
  for (i = 0; i < m->tracks; i++) {
    ret = snprintf(s + (sz - r), r, "%d: ", i);
    if (ret > r) return -1;
    r -= ret;
    ret = snprintf(s + (sz - r), r, "%.*s ",
      m->track[i].info.track.len, m->track[i].info.track.name);
    if (ret > r) return -1;
    r -= ret;
    ret = snprintf(s + (sz - r), r, " :: %s ",
     kr_codec_type_to_str(m->track[i].info.codec.type));
    if (ret > r) return -1;
    r -= ret;
    if (m->track[i].info.codec.type == KR_CODEC_AUDIO) {
      ret = snprintf(s + (sz - r), r,
        "%s %dch %dbit %dkhz",
        kr_audio_codec_to_str(m->track[i].info.codec.audio),
        m->track[i].info.audio.channels,
        m->track[i].info.audio.bit_depth,
        m->track[i].info.audio.sample_rate);
    } else if (m->track[i].info.codec.type == KR_CODEC_VIDEO) {
      ret = snprintf(s + (sz - r), r, "%s %dx%d %d/%d fps",
        kr_video_codec_to_str(m->track[i].info.codec.video),
        m->track[i].info.video.width,
        m->track[i].info.video.height,
        m->track[i].info.video.fps_num,
        m->track[i].info.video.fps_den);
    }
    if (ret > r) return -1;
    r -= ret;
    if (m->track[i].header.len > 0) {
      ret = snprintf(s + (sz - r), r, " :: hdr %zu bytes",
        header_sz(&m->track[i].header));
      if (ret > r) return -1;
      r -= ret;
    }
    ret = snprintf(s + (sz - r), r, "\n");
    if (ret > r) return -1;
    r -= ret;
  }
  return sz - r;
}

size_t kr_txt_muxer_sizeof(void) {
  kr_txt_muxer *m;
  return sizeof(*m);
}

kr_txt_muxer *kr_txt_muxer_init(kr_txt_muxer_setup *setup) {
  int i;
  kr_txt_muxer *m;
  if (!setup || !setup->mem) return NULL;
  if (setup->tracks < 1 || setup->tracks > KR_TXT_MUXER_NTRACKS) return NULL;
  m = (kr_txt_muxer *)setup->mem;
  memset(m, 0, sizeof(*m));
  m->comment_len = setup->comment_len;
  memcpy(m->comment, setup->comment, m->comment_len);
  m->tracks = setup->tracks;
  m->nblocks = 0;
  for (i = 0; i < m->tracks; i++) {
    m->track[i].nblocks = 0;
    m->track[i].header = setup->track[i].header;
    m->track[i].info = setup->track[i].info;
    if (m->track[i].info.codec.type == KR_CODEC_VIDEO) m->have_video_track = 1;
  }
  return m;
}
