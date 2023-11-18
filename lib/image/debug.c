#include "debug.h"
#include <krad/app/debug.h>
#include <cairo/cairo.h>

#define FRAME_DEBUG_LINES 6
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

static void packed_yuv_kludge(kr_image *img, uint8_t *data, int stride, int h) {
  int x;
  int y;
  uint32_t val;
  //int bit;
  int s;
  uint32_t mp;
  s = 0;
  //printk("stride is %d width is %d h is %d", stride, img->info.w, h);
  for (y = 0; y < h; y++) {
    for (x = 0; x < img->info.w; x++) {
      val = data[(y * stride) + x * 1];
      mp = (y * 1 * img->ps[0]) + ((x * 4) / 2);
      //bit = x % 32;
      //if ((CHECK_BIT(val, bit)) || s) {
      if (s || val) {
        if (x % 2 == 0) {
          img->px[0][mp + 1] = 0x80;
          img->px[0][mp + 3] = 0x80;
          img->px[0][mp + 0] = 0xFF;
          img->px[0][mp + 2] = 0xFF;
        }
      } else {
        if (x % 2 == 0) {
          img->px[0][mp + 0] = 0x00; //Y0
          img->px[0][mp + 2] = 0x00; //Y1
          img->px[0][mp + 1] = 0xFF; //U
          img->px[0][mp + 3] = 0x00; //V
        }
      }
    }
  }
}

int kr_image_debug_render(kr_image *img, int line, char *text) {
  cairo_t *cr;
  cairo_surface_t *cs;
  cairo_format_t fmt;
  int h;
  size_t sz;
  int stride;
  uint8_t *data;
  //int x;
  return 0;
  switch (img->info.fmt) {
  case KR_PIXELS_ARGB:
    fmt = CAIRO_FORMAT_ARGB32;
    break;
  case KR_PIXELS_YUV422P:
    fmt = CAIRO_FORMAT_A8;
    break;
  case KR_PIXELS_YVU422P:
    fmt = CAIRO_FORMAT_A8;
    break;
  case KR_PIXELS_YUV444:
    fmt = CAIRO_FORMAT_A8;
    break;
  case KR_PIXELS_YUV420:
    fmt = CAIRO_FORMAT_A8;
    break;
  case KR_PIXELS_YUV422:
    fmt = CAIRO_FORMAT_A8;
    break;
  default:
    return -1;
  }
  data = NULL;
  stride = 0;
  h = 40;
  if (line < 0 || line > FRAME_DEBUG_LINES) line = 2;
  if (img->info.fmt == KR_PIXELS_YUV422P
   || img->info.fmt == KR_PIXELS_YVU422P) {
    stride = cairo_format_stride_for_width(fmt, img->info.w);
    sz = stride * h;
    data = alloca(sz);
    memset(data, 0, stride * h);
    cs = cairo_image_surface_create_for_data(data, fmt, img->info.w,
     h, stride);
    if (cairo_surface_status(cs)) return -2;
    cr = cairo_create(cs);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  } else {
    cs = cairo_image_surface_create_for_data(img->px[0], fmt, img->info.w,
     img->info.h, img->ps[0]);
    if (cairo_surface_status(cs)) return -2;
    cr = cairo_create(cs);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
    if (line == 5) {
      cairo_translate(cr, 0, img->info.h - (2 * h));
    } else {
      cairo_translate(cr, 0, line * h);
    }
  }
  cairo_move_to(cr, 32, h - 2);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
  cairo_select_font_face(cr, "VCR OSD Mono", CAIRO_FONT_SLANT_NORMAL,
   CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 40.0f);
  cairo_show_text(cr, text);
  cairo_surface_flush(cs);
  if (img->info.fmt == KR_PIXELS_YUV422P
   || img->info.fmt == KR_PIXELS_YVU422P) {
    packed_yuv_kludge(img, data, stride, h);
  }
  cairo_destroy(cr);
  cairo_surface_destroy(cs);
}
