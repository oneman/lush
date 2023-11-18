#include <math.h>
#include <krad/mem/mem.h>
#include <krad/radio/version.h>
#include "internal.h"
#include "encoder.h"

#define KR_LAVC_HEADER_SZ_MAX 8192 * 4
#define KR_LAVC_NHEADERS 3
#define KR_LAVC_NCHANNELS 2

struct kr_lavc_encoder {
  kr_lavc_encoder_info info;
  AVCodecContext *ctx;
  AVFrame *frm;
  uint8_t header[KR_LAVC_HEADER_SZ_MAX];
  kr_iov headers[KR_LAVC_NHEADERS];
  uint8_t nheaders;
};

size_t kr_lavc_encoder_sizeof() {
  size_t sz;
  sz = 0;
  sz += sizeof(kr_lavc_encoder);
  return sz;
}

kr_lavc_encoder *kr_lavc_encoder_init(kr_lavc_encoder_setup *setup) {
  int ret;
  kr_lavc_encoder *e;
  if (!setup) return NULL;
  e = (kr_lavc_encoder *)setup->mem;
  memset(e, 0, sizeof(*e));
  e->info = setup->info;
  e->nheaders = 0;
  if (kr_lavc_global_init() < 0) return NULL;
  //e->frm = av_frame_alloc();
  //e->ctx = avcodec_alloc_context3(h264);
  //ret = avcodec_open2(e->ctx, h264, NULL);
  ret = -1;
  if (ret < 0) return NULL;
  return e;
}

int kr_lavc_encoder_get_headers(kr_lavc_encoder *e,
 kr_iov *v, size_t len) {
  int i;
  if (!e || !v) return -1;
  if (len < e->nheaders) return -2;
  for (i = 0; i < e->nheaders; i++) {
    v[i].buf = e->headers[i].buf;
    v[i].sz = e->headers[i].sz;
  }
  return i;
}

int kr_lavc_encoder_get_info(kr_lavc_encoder *e,
 kr_lavc_encoder_info *info) {
  if (!e || !info) return -1;
  *info = e->info;
  return 0;
}

int kr_lavc_encode_audio(kr_lavc_encoder *e, kr_coded_audio *out, kr_audio *in) {
  if (!e || !out || !in) return -1;
  return 0;
}

int kr_lavc_encode_video(kr_lavc_encoder *e, kr_frame *out, kr_frame *in) {
  if (!e || !out || !in) return -1;
  return 0;
}

int kr_lavc_encoder_ctl(kr_lavc_encoder *e, kr_patchset *patchset) {
  if (!e || !patchset) return -1;
  return 0;
}

int kr_lavc_encoder_destroy(kr_lavc_encoder *e) {
  if (!e) return -1;
  avcodec_close(e->ctx);
  avcodec_free_context(&e->ctx);
  av_frame_free(&e->frm);
  return 0;
}
