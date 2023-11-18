#include <theora/theoraenc.h>
#include <krad/mem/mem.h>
#include "encoder.h"

#define THEORA_NHEADERS 3

struct kr_theora_encoder {
  th_enc_ctx *ctx;
  th_info ti;
  th_comment comment;
  kr_theora_encoder_info info;
  kr_codec_header headers;
};

size_t kr_theora_encoder_sizeof() {
  return sizeof(kr_theora_encoder);
}

kr_theora_encoder *kr_theora_encoder_init(kr_theora_encoder_setup *setup) {
  int ret;
  int h;
  ogg_packet op;
  kr_theora_encoder *th;
  if (!setup) return NULL;
  th = (kr_theora_encoder *)setup->mem;
  memset(th, 0, sizeof(*th));
  th->info = setup->info;
  th_info_init(&th->ti);
  th_comment_init(&th->comment);
  th->ti.pic_width = th->info.width;
  th->ti.pic_height = th->info.height;
  th->ti.pic_x = 0;
  th->ti.pic_y = 0;
  th->ti.frame_width = kr_image_axis_padded(th->info.width);
  th->ti.frame_height = kr_image_axis_padded(th->info.height);
  th->ti.fps_numerator = th->info.fps_num;
  th->ti.fps_denominator = th->info.fps_den;
  th->ti.aspect_denominator = 1;
  th->ti.aspect_numerator = 1;
  th->ti.target_bitrate = 0;
  th->ti.keyframe_granule_shift = 12;
  th->ti.quality = th->info.quality;
  if (th->info.pixel_fmt == KR_PIXELS_YUV420)
   th->ti.pixel_fmt = TH_PF_420;
  else if (th->info.pixel_fmt == KR_PIXELS_YUV422)
   th->ti.pixel_fmt = TH_PF_422;
  else if (th->info.pixel_fmt == KR_PIXELS_YUV444)
   th->ti.pixel_fmt = TH_PF_444;
  else return NULL;
  th->ctx = th_encode_alloc(&th->ti);
  if (th->ctx == NULL) return NULL;
  h = 0;
  while ((ret = th_encode_flushheader(th->ctx, &th->comment, &op))) {
    if (h == THEORA_NHEADERS) {
      kr_theora_encoder_destroy(th);
      return NULL;
    }
    th->headers.data[h].buf = kr_alloc(op.bytes);
    memcpy(th->headers.data[h].buf, op.packet, op.bytes);
    th->headers.data[h].sz = op.bytes;
    h++;
  }
  th->headers.len = h;
  uint32_t key_rate;
  key_rate = th->info.key_rate;
  if (key_rate) {
    th_encode_ctl(th->ctx, TH_ENCCTL_SET_KEYFRAME_FREQUENCY_FORCE, &key_rate,
     sizeof(key_rate));
  }
  return th;
}

int kr_theora_encoder_get_headers(kr_theora_encoder *th, kr_codec_header *h) {
  if (!th || !h) return -1;
  *h = th->headers;
  return 0;
}

int kr_theora_encoder_get_info(kr_theora_encoder *th,
 kr_theora_encoder_info *info) {
  if (!th || !info) return -1;
  *info = th->info;
  return 0;
}

int kr_theora_encode(kr_theora_encoder *th, kr_frame *out, kr_frame *in) {
  int ret;
  int w;
  int h;
  th_ycbcr_buffer ycbcr;
  ogg_packet op;
  if (!th || !out || !in) return -1;
  memset(&op, 0, sizeof(op));
  if (in->image.info.fmt != th->info.pixel_fmt) return -2;
  ycbcr[0].data = in->image.px[0];
  ycbcr[0].stride = in->image.ps[0];
  w = kr_image_axis_padded(in->image.info.w);
  h = kr_image_axis_padded(in->image.info.h);
  ycbcr[0].width = w;
  ycbcr[0].height = h;
  ycbcr[1].data = in->image.px[1];
  ycbcr[1].stride = in->image.ps[1];
  ycbcr[2].data = in->image.px[2];
  ycbcr[2].stride = in->image.ps[2];
  if (th->info.pixel_fmt == KR_PIXELS_YUV420) {
    ycbcr[1].width = w / 2;
    ycbcr[1].height = h / 2;
    ycbcr[2].width = w / 2;
    ycbcr[2].height = h / 2;
  }
  if (th->info.pixel_fmt == KR_PIXELS_YUV422) {
    ycbcr[1].width = w / 2;
    ycbcr[1].height = h;
    ycbcr[2].width = w / 2;
    ycbcr[2].height = h;
  }
  if (th->info.pixel_fmt == KR_PIXELS_YUV444) {
    ycbcr[1].width = w;
    ycbcr[1].height = h;
    ycbcr[2].width = w;
    ycbcr[2].height = h;
  }
  ret = th_encode_ycbcr_in(th->ctx, ycbcr);
  if (ret < 0) return -3;
  ret = th_encode_packetout(th->ctx, th->info.eos, &op);
  if (ret <= 0) return -4;
  ret = th_packet_iskeyframe(&op);
  if (ret < 0) return -5;
  out->info.keyframe = ret;
  out->info.tc = op.granulepos;
  out->type = KR_IMAGE_ENC;
  out->coded.info.image.w = th->ti.frame_width;
  out->coded.info.image.h = th->ti.frame_height;
  out->coded.info.image.fmt = th->info.pixel_fmt;
  out->coded.info.fmt = KR_CODED_IMAGE_THEORA;
  out->coded.info.sz = op.bytes;
  memcpy(out->coded.data, op.packet, out->coded.info.sz);
  return 0;
}

int kr_theora_encoder_ctl(kr_theora_encoder *th, kr_patchset *patchset) {
  if (!th || !patchset) return -1;
  //th_encode_ctl(th->theora_encoder, TH_ENCCTL_SET_QUALITY,
  // &->quality, sizeof(int));
  return 0;
}

int kr_theora_encoder_destroy(kr_theora_encoder *th) {
  int i;
  if (!th) return -1;
  th_info_clear(&th->ti);
  th_comment_clear(&th->comment);
  th_encode_free(th->ctx);
  for (i = 0; i < th->headers.len; i++) {
    if (th->headers.data[i].buf) free(th->headers.data[i].buf);
  }
  return 0;
}
