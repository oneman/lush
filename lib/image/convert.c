#include <krad/app/debug.h>
#include "convert.h"

void kr_image_convert_init(kr_convert *conv) {
  if (conv == NULL) return;
  memset(conv, 0, sizeof(kr_convert));
  conv->quality = SWS_BILINEAR;
}

void kr_image_convert_clear(kr_convert *conv) {
  if (conv == NULL) return;
  if (conv->sws != NULL) {
    sws_freeContext(conv->sws);
  }
  kr_image_convert_init(conv);
}

int kr_pxfmt_to_libav_pxfmt(kr_pixel_fmt fmt) {
  switch (fmt) {
    case KR_PIXELS_ARGB: return AV_PIX_FMT_RGB32;
    /*case KR_PIXELS_XRGB: return AV_PIX_FMT_0RGB32;*/
    case KR_PIXELS_YUV422P: return AV_PIX_FMT_YUYV422;
    case KR_PIXELS_YVU422P: return AV_PIX_FMT_UYVY422;
    case KR_PIXELS_YUV444: return AV_PIX_FMT_YUV444P;
    case KR_PIXELS_YUV420: return AV_PIX_FMT_YUV420P;
    case KR_PIXELS_YUV422: return AV_PIX_FMT_YUV422P;
    default: printke("Krad Converter: wrong pix_fmt %s",
     kr_pixel_fmt_to_str(fmt)); return -1;
  }
}

int kr_image_convert(kr_convert *conv, kr_image *dst, kr_image *src) {
  int ret;
  int src_fmt;
  int dst_fmt;
  if (conv == NULL) return -2;
  if (src == NULL) return -3;
  if (dst == NULL) return -4;
  src_fmt = kr_pxfmt_to_libav_pxfmt(src->info.fmt);
  dst_fmt = kr_pxfmt_to_libav_pxfmt(dst->info.fmt);
  if (src_fmt < 0 || dst_fmt < 0) return -1;
  conv->sws = sws_getCachedContext(conv->sws, src->info.w, src->info.h,
   src_fmt, dst->info.w, dst->info.h, dst_fmt, conv->quality, NULL, NULL,
   NULL);
  if (conv->sws == NULL) {
    printke("Image convert: could not sws_getCachedContext");
    printke("source: %dx%d--%d dest:  %dx%d--%d", src->info.w, src->info.h,
     src->ps[0], dst->info.w, dst->info.h, dst->ps[0]);
    return -5;
  }
  ret = sws_scale(conv->sws, (const uint8_t * const*)src->px, src->ps, 0,
   src->info.h, dst->px, dst->ps);
  if (ret == dst->info.h) {
    return 0;
  }
  return ret;
}
