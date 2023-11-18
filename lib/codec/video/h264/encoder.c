#include <stdint.h>
#include <time.h>
#include <x264.h>
#include <krad/mem/mem.h>
#include "encoder.h"

struct kr_h264_encoder {
  x264_t *encoder;
  x264_param_t param;
  x264_nal_t *nals;
  int nnals;
  int64_t pts;
  kr_h264_encoder_info info;
};

static void h264_info_to_params(x264_param_t *param, kr_h264_encoder_info *info) {
  // For streaming
  x264_param_default_preset(param, "ultrafast", "zerolatency");
  param->i_width = info->width;
  param->i_height = info->height;
  param->rc.i_bitrate = info->bit_rate;
  param->rc.i_rc_method = X264_RC_ABR;
  param->i_fps_num = info->fps_num;
  param->i_fps_den = info->fps_den;
  param->i_timebase_num = info->fps_num;
  param->i_timebase_den = info->fps_den;
  param->i_keyint_max = info->key_rate;
  switch (info->pixel_fmt) {
    case KR_PIXELS_YUV420:
      param->i_csp = X264_CSP_I420;
      break;
    case KR_PIXELS_YUV422:
      param->i_csp = X264_CSP_I422;
      break;
    case KR_PIXELS_YUV444:
      param->i_csp = X264_CSP_I444;
      break;
    default: break;
  }
}

size_t kr_h264_encoder_sizeof() {
  return sizeof(kr_h264_encoder);
}

int kr_h264_encoder_get_info(kr_h264_encoder *h264,
  kr_h264_encoder_info *info) {
  if (!h264 || !info) return -1;
  *info = h264->info;
  return 0;
}

int kr_h264_encoder_get_headers(kr_h264_encoder *h264, kr_codec_header *h) {
  int i;
  int ret;
  if (!h264 || !h) return -1;
  ret = x264_encoder_headers(h264->encoder, &h264->nals, &h264->nnals);
  if (ret < 0) return -2;
  h->len = h264->nnals;
  for (i = 0; i < h->len; i++) {
    h->data[i].buf = h264->nals[i].p_payload;
    h->data[i].sz = h264->nals[i].i_payload;
  }
  return 0;
}

ssize_t kr_h264_encode(kr_h264_encoder *h264, kr_frame *out, kr_frame *in) {
  ssize_t ret;
  x264_picture_t in_pic;
  x264_picture_t out_pic;
  if (!h264 || !out || !in) return -1;
  x264_picture_init(&in_pic);
  x264_picture_init(&out_pic);
  out_pic.param = &h264->param;
  in_pic.param = &h264->param;
  in_pic.img.i_csp = h264->param.i_csp;
  in_pic.img.i_plane = 3;
  in_pic.img.i_stride[0] = in->image.ps[0];
  in_pic.img.i_stride[1] = in->image.ps[1];
  in_pic.img.i_stride[2] = in->image.ps[2];
  in_pic.img.plane[0] = in->image.px[0];
  in_pic.img.plane[1] = in->image.px[1];
  in_pic.img.plane[2] = in->image.px[2];
  do {
    in_pic.i_pts = h264->pts++;
    ret = x264_encoder_encode(h264->encoder,
      &h264->nals, &h264->nnals, &in_pic, &out_pic);
  } while(ret == 0);
  if (ret < 0) {
    printk("x264 encoding failed!");
    return -1;
  }
  out->coded.data = h264->nals[0].p_payload;
  out->coded.info.sz = ret;
  out->info.keyframe = out_pic.b_keyframe;
  out->info.tc = out_pic.i_pts;
  return ret;
}

kr_h264_encoder *kr_h264_encoder_init(kr_h264_encoder_setup *setup) {
  kr_h264_encoder *h264;
  if (!setup) return NULL;
  h264 = (kr_h264_encoder *)setup->mem;
  memset(h264, 0, sizeof(*h264));
  h264->info = setup->info;
  h264->pts = time(NULL);
  x264_param_default(&h264->param);
  h264_info_to_params(&h264->param, &h264->info);
  h264->encoder = x264_encoder_open(&h264->param);
  if (!h264->encoder) return NULL;
  return h264;
}

int kr_h264_encoder_destroy(kr_h264_encoder *h264) {
  x264_encoder_close(h264->encoder);
  return 0;
}
