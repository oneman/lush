#include <stdint.h>
#include <time.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <krad/mem/mem.h>
#include "encoder.h"

#define KR_VPX_IMG_ALIGN 2

struct kr_vpx_encoder {
  vpx_codec_ctx_t encoder;
  vpx_codec_enc_cfg_t cfg;
  uint64_t frames;
  kr_vpx_encoder_info info;
};

static void vpx_info_to_cfg(vpx_codec_enc_cfg_t *cfg,
 kr_vpx_encoder_info *info);
static void kr_image_to_vpx_image(vpx_image_t *v, kr_image *k);

static void vpx_info_to_cfg(vpx_codec_enc_cfg_t *cfg,
 kr_vpx_encoder_info *info) {
  cfg->g_w = info->width;
  cfg->g_h = info->height;
  cfg->g_timebase.num = info->fps_den;
  cfg->g_timebase.den = info->fps_num;
  cfg->rc_target_bitrate = info->bit_rate / 1000;
  cfg->g_threads = 2;
  cfg->kf_max_dist = info->key_rate;
}

static void kr_image_to_vpx_image(vpx_image_t *v, kr_image *k) {
  v->fmt = VPX_IMG_FMT_I420;
  v->w = k->info.w + (KR_VPX_IMG_ALIGN - (k->info.w % KR_VPX_IMG_ALIGN));
  v->h = k->info.h + (KR_VPX_IMG_ALIGN - (k->info.h % KR_VPX_IMG_ALIGN));
  v->d_w = k->info.w;
  v->d_h = k->info.h;
  v->x_chroma_shift = 1;
  v->x_chroma_shift = 1;
  v->planes[0] = k->px[0];
  v->planes[1] = k->px[1];
  v->planes[2] = k->px[2];
  v->planes[3] = NULL;
  v->stride[0] = k->ps[0];
  v->stride[1] = k->ps[1];
  v->stride[2] = k->ps[2];
  v->stride[3] = 0;
  v->bps = 12;
  v->user_priv = NULL;
  v->img_data = NULL;
  v->img_data_owner = 0;
  v->self_allocd = 0;
}

size_t kr_vpx_encoder_sizeof() {
  return sizeof(kr_vpx_encoder);
}

int kr_vpx_encoder_get_headers(kr_vpx_encoder *vpx, kr_codec_header *h) {
  if (!vpx || !h) return -1;
  h->len = 0;
  return 0;
}

int kr_vpx_encoder_get_info(kr_vpx_encoder *vpx,
  kr_vpx_encoder_info *info) {
  if (!vpx || !info) return -1;
  *info = vpx->info;
  return 0;
}

int kr_vpx_encoder_ctl(kr_vpx_encoder *vpx, kr_patchset *patchset) {
  kr_patchset_apply(&vpx->info, patchset);
  vpx_info_to_cfg(&vpx->cfg, &vpx->info);
  vpx_codec_enc_config_set(&vpx->encoder, &vpx->cfg);
  return 0;
}

ssize_t kr_vpx_encode(kr_vpx_encoder *vpx, kr_frame *out, kr_frame *in) {
  ssize_t ret;
  int flags;
  const char *err;
  vpx_image_t vimg;
  vpx_codec_iter_t iter;
  const vpx_codec_cx_pkt_t *pkt;
  if (vpx == NULL) return -1;
  if (!out  || !in) return -2;
  if (in->image.info.fmt != KR_PIXELS_YUV420) return -3;
  iter = NULL;
  ret = 0;
  flags = 0;
  memset(&vimg, 0, sizeof(vimg));
  if (in != NULL) {
    kr_image_to_vpx_image(&vimg, &in->image);
    ret = vpx_codec_encode(&vpx->encoder, &vimg, vpx->frames++,
     1, flags, VPX_DL_REALTIME);
    /*printk("tc %"PRIu64"  td  %u", in->tc, in->td);*/
  } else {
    ret = vpx_codec_encode(&vpx->encoder, NULL, 0, 1, flags, 1);
  }
  if (ret != 0) {
    printk("VPX Encoder: encode: %s", vpx_codec_err_to_string(ret));
    printk("VPX Encoder: encode: %s", vpx_codec_error(&vpx->encoder));
    err = vpx_codec_error_detail(&vpx->encoder);
    printk("Details: %s", err);
    return -4;
  }
  pkt = vpx_codec_get_cx_data(&vpx->encoder, &iter);
  if ((pkt == NULL) || (pkt->kind != VPX_CODEC_CX_FRAME_PKT)) {
    return 0;
  }
  kr_frame_copytime(out, in);
  out->type = KR_IMAGE_ENC;
  out->coded.info.sz = pkt->data.frame.sz;
  out->coded.info.image = in->image.info;
  out->info.keyframe = pkt->data.frame.flags & VPX_FRAME_IS_KEY;
  out->info.tc = pkt->data.frame.pts;

  // why oneman why :(
  memcpy(out->coded.data, pkt->data.frame.buf, pkt->data.frame.sz);
  printk("VPX Encoder: encoded frame size %d", pkt->data.frame.sz);
  //return pkt->data.frame.sz;
  return 0;
}

kr_vpx_encoder *kr_vpx_encoder_init(kr_vpx_encoder_setup *setup) {
  kr_vpx_encoder *vpx;
  vpx_codec_err_t res;
  if (!setup) return NULL;
  vpx = (kr_vpx_encoder *)setup->mem;
  memset(vpx, 0, sizeof(*vpx));
  vpx->info = setup->info;
  res = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &vpx->cfg, 0);
  if (res) return NULL;
  vpx_info_to_cfg(&vpx->cfg, &vpx->info);
  res = vpx_codec_enc_init(&vpx->encoder,
   vpx_codec_vp8_cx(), &vpx->cfg, 0);
  if (res) return NULL;
  return vpx;
}

int kr_vpx_encoder_destroy(kr_vpx_encoder *vpx) {
  vpx_codec_destroy(&vpx->encoder);
  return 0;
}
