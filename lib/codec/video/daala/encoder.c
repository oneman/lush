#include <daala/daalaenc.h>
#include <krad/mem/mem.h>
#include "encoder.h"

#define DAALA_NHEADERS 3

struct kr_daala_encoder {
  daala_enc_ctx *ctx;
  daala_info di;
  daala_comment dc;
  kr_daala_encoder_info info;
  kr_codec_header headers;
};

size_t kr_daala_encoder_sizeof() {
  return sizeof(kr_daala_encoder);
}

kr_daala_encoder *kr_daala_encoder_init(kr_daala_encoder_setup *setup) {
  int ret;
  int h;
  daala_packet op;
  kr_daala_encoder *dh;
  if (!setup) return NULL;
  dh = (kr_daala_encoder *)setup->mem;
  memset(dh, 0, sizeof(*dh));
  dh->info = setup->info;
  if (dh->info.key_rate < 1) dh->info.key_rate = 1;
  daala_info_init(&dh->di);
  dh->di.pic_width = dh->info.width;
  dh->di.pic_height = dh->info.height;
  dh->di.timebase_numerator = dh->info.fps_num;
  dh->di.timebase_denominator = dh->info.fps_den;
  dh->di.frame_duration = 1;
  dh->di.pixel_aspect_numerator = 1;
  dh->di.pixel_aspect_denominator = 1;
  dh->di.keyframe_rate = dh->info.key_rate;
  dh->di.nplanes = 3;
  if (dh->info.pixel_fmt == KR_PIXELS_YUV420) {
    dh->di.plane_info[0].xdec = 0;
    dh->di.plane_info[0].ydec = 0;
    dh->di.plane_info[1].xdec = 1;
    dh->di.plane_info[1].ydec = 1;
    dh->di.plane_info[2].xdec = 1;
    dh->di.plane_info[2].ydec = 1;
    dh->di.plane_info[3].xdec = 0;
    dh->di.plane_info[3].ydec = 0;
  } else if (dh->info.pixel_fmt == KR_PIXELS_YUV422) {
    dh->di.plane_info[0].xdec = 0;
    dh->di.plane_info[0].ydec = 0;
    dh->di.plane_info[1].xdec = 0;
    dh->di.plane_info[1].ydec = 1;
    dh->di.plane_info[2].xdec = 0;
    dh->di.plane_info[2].ydec = 1;
    dh->di.plane_info[3].xdec = 0;
    dh->di.plane_info[3].ydec = 0;
  } else if (dh->info.pixel_fmt == KR_PIXELS_YUV444) {
    dh->di.plane_info[0].xdec = 0;
    dh->di.plane_info[0].ydec = 0;
    dh->di.plane_info[1].xdec = 0;
    dh->di.plane_info[1].ydec = 0;
    dh->di.plane_info[2].xdec = 0;
    dh->di.plane_info[2].ydec = 0;
    dh->di.plane_info[3].xdec = 0;
    dh->di.plane_info[3].ydec = 0;
  } else {
    return NULL;
  }
  dh->ctx = daala_encode_create(&dh->di);
  if (!dh->ctx) return NULL;
  daala_comment_init(&dh->dc);
  h = 0;
  while ((ret = daala_encode_flush_header(dh->ctx, &dh->dc, &op))) {
    if (h == DAALA_NHEADERS) {
      kr_daala_encoder_destroy(dh);
      return NULL;
    }
    dh->headers.data[h].buf = kr_alloc(op.bytes);
    memcpy(dh->headers.data[h].buf, op.packet, op.bytes);
    dh->headers.data[h].sz = op.bytes;
    h++;
  }
  dh->headers.len = h;
  return dh;
}

int kr_daala_encoder_get_headers(kr_daala_encoder *dh, kr_codec_header *h) {
  if (!dh || !h) return -1;
  *h = dh->headers;
  return 0;
}

int kr_daala_encoder_get_info(kr_daala_encoder *dh,
 kr_daala_encoder_info *info) {
  if (!dh || !info) return -1;
  *info = dh->info;
  return 0;
}

int kr_daala_encode(kr_daala_encoder *dh, kr_frame *out, kr_frame *in) {
  int i;
  int ret;
  int last;
  od_img img;
  daala_packet op;
  if (!dh || !out || !in) return -1;
  ret = -1;
  last = 0;
  memset(&op, 0, sizeof(op));
  if (in->image.info.fmt != dh->info.pixel_fmt) return -2;
  img.nplanes = 3;
  img.width = in->image.info.w;
  img.height = in->image.info.h;
  for (i = 0; i < img.nplanes; i++) {
    img.planes[i].data = in->image.px[i];
    img.planes[i].xstride = 1;
    img.planes[i].ystride = in->image.ps[i];
    img.planes[i].xdec = dh->di.plane_info[i].xdec;
    img.planes[i].ydec = dh->di.plane_info[i].ydec;
  }
  ret = daala_encode_img_in(dh->ctx, &img, 1);
  if (ret < 0) return -3;
  ret = daala_encode_packet_out(dh->ctx, last, &op);
  if (ret <= 0) return -4;
  out->info.keyframe = daala_packet_iskeyframe(&op);
  out->info.tc = op.granulepos;
  out->type = KR_IMAGE_ENC;
  out->coded.info.image.w = dh->di.pic_width;
  out->coded.info.image.h = dh->di.pic_height;
  out->coded.info.image.fmt = dh->info.pixel_fmt;
  out->coded.info.fmt = KR_CODED_IMAGE_DAALA;
  out->coded.info.sz = op.bytes;
  memcpy(out->coded.data, op.packet, out->coded.info.sz);
  return 0;
}

int kr_daala_encoder_ctl(kr_daala_encoder *dh, kr_patchset *patchset) {
  if (!dh || !patchset) return -1;
  /*daala_encode_ctl(dd, OD_SET_QUANT, &video_q, sizeof(int));
  daala_encode_ctl(dd, OD_SET_MC_USE_CHROMA, &mc_use_chroma, sizeof(int));
  daala_encode_ctl(dd, OD_SET_MV_RES_MIN, &mv_res_min, sizeof(int));
  daala_encode_ctl(dd, OD_SET_MV_LEVEL_MIN, &mv_level_min, sizeof(int));
  daala_encode_ctl(dd, OD_SET_MV_LEVEL_MAX, &mv_level_max, sizeof(int));*/
  return 0;
}

int kr_daala_encoder_destroy(kr_daala_encoder *dh) {
  int i;
  if (!dh) return -1;
  daala_info_clear(&dh->di);
  daala_encode_free(dh->ctx);
  daala_comment_clear(&dh->dc);
  for (i = 0; i < dh->headers.len; i++) {
    if (dh->headers.data[i].buf) free(dh->headers.data[i].buf);
  }
  return 0;
}
