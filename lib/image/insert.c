#include <cairo.h>
#include <math.h>
#include <krad/app/debug.h>
#include "coder.h"
#include "insert.h"

static double aspect_scale(int sw, int sh, int dw, int dh, int *w, int *h);
static void clamp_rect(kr_rect *r, int w, int h);
static int rect_image_match(kr_rect *r, kr_image_info *im);
static int get_midpoint(int len1, int len2);
static int rect_clipped_out(kr_rect *r, int w, int h);

static double aspect_scale(int sw, int sh, int dw, int dh, int *w, int *h) {
  double scale_x, scale_y, scale;
  scale_x = (float)dw  / sw;
  scale_y = (float)dh / sh;
  scale = MIN(scale_x, scale_y);
  *w = sw * scale;
  *h = sh * scale;
  return scale;
}

static void clamp_rect(kr_rect *r, int w, int h) {
  if (r->x < 0) {
    r->x = 0;
  } else if (r->x > w - 1) {
    r->x = w - 1;
  }
  if (r->y < 0) {
    r->y = 0;
  } else if (r->y > h - 1) {
    r->y = h - 1;
  }
  if (r->w < 1) {
    r->w = 1;
  } else if (r->x + r->w > w) {
    r->w = w - r->x;
  }
  if (r->h < 1) {
    r->h = 1;
  } else if (r->y + r->h > h) {
    r->h = h - r->y;
  }
}

static int rect_image_match(kr_rect *r, kr_image_info *im) {
  if (r->x || r->y || r->w != im->w || r->h != im->h) return 0;
  return 1;
}

static int get_midpoint(int len1, int len2) {
  int x;
  x = (len2 / 2) - (len1 / 2);
  return x;
}

static int rect_clipped_out(kr_rect *r, int w, int h) {
  if (w < 1 || h < 1 || r->w < 1 || r->h < 1) return 1;
  if (r->x >= w || r->y >= h) return 1;
  if ((r->x + r->w < 0) || (r->y + r->h < 0)) return 1;
  return 0;
}

void kr_rect_scale(kr_rect *r, double s) {
  r->x = round(r->x * s);
  r->y = round(r->y * s);
  r->w = round(r->w * s);
  r->h = round(r->h * s);
}

double kr_rect_scaler(int max_w, int max_h, int w, int h) {
  double s;
  double xs;
  double ys;
  xs = (float) max_w / w;
  ys = (float) max_h / h;
  s = MIN(xs, ys);
  return s;
}

int kr_rect_center(kr_rect *out, kr_rect *container) {
  int rel_x;
  int rel_y;
  rel_x = get_midpoint(out->w, container->w);
  rel_y = get_midpoint(out->h, container->h);
  out->x = rel_x + container->x;
  out->y = rel_y + container->y;
  return 0;
}

int kr_image_insert(kr_image *dst, kr_image *src, kr_rect *area,
 kr_rect *crop) {
  int ret;
  kr_image dst_area;
  kr_convert convert;
  if (!dst || !src) return -1;
  ret = kr_image_crop(&dst_area, dst, area);
  if (ret) return ret;
  /*printk("src: %dx%d -> dst: %dx%d area: %d,%d %dx%d :: dst image area: %dx%d",
   src->info.w, src->info.h, dst->info.w, dst->info.h, area->x, area->y,
   area->w, area->h, dst_area.info.w, dst_area.info.h);*/
  kr_image_convert_init(&convert);
  kr_image_convert(&convert, &dst_area, src);
  kr_image_convert_clear(&convert);
  return 0;
  /*
  if (crop && !rect_image_match(crop, &src->info)) {
    src_crop = *crop;
    ret = kr_image_crop(&src_copy, src, &src_crop);
    if (ret) return ret;
  } else {
    src_crop.x = 0;
    src_crop.y = 0;
    src_crop.w = src->info.w;
    src_crop.h = src->info.h;
    src_copy = *src;
  }
  src = &src_copy;
  if (area && !rect_image_match(area, &dst->info)) {
    dst_area = *area;
  } else {
    dst_area.x = 0;
    dst_area.y = 0;
    dst_area.w = dst->info.w;
    dst_area.h = dst->info.h;
  }
  kr_rect final_area;
  final_area.x = 0;
  final_area.y = 0;
  aspect_scale(src_crop.w, src_crop.h, dst_area.w, dst_area.h, &final_area.w,
   &final_area.h);
  kr_rect_center(&final_area, &dst_area);
  ret = kr_image_crop(&dst_copy, dst, &final_area);
  if (ret) return ret;
  dst = &dst_copy;
  */
}

static int cairo_insert(kr_image *dst, kr_image *src, kr_insert *insert);
static int handle_input_format(kr_image *conv, kr_image *dst, kr_image *src);
static int handle_output_format(kr_image *conv, kr_image *dst, kr_image *src);

static int cairo_insert(kr_image *dst, kr_image *src, kr_insert *insert) {
  double sf;
  kr_image src_cropped;
  cairo_t *cr;
  cairo_surface_t *dst_cst;
  cairo_surface_t *src_cst;
  kr_rect final_area;
  int ret;
  dst_cst = cairo_image_surface_create_for_data(dst->px[0],
   CAIRO_FORMAT_ARGB32, dst->info.w, dst->info.h, dst->ps[0]);
  if (cairo_surface_status(dst_cst)) {
    printke("Cairo Insert: Bad dst surface context");
    return -2;
  }
  if (!rect_image_match(&insert->src_crop, &src->info)) {
    ret = kr_image_crop(&src_cropped, src, &insert->src_crop);
    if (ret != 0) {
      printke("Image: crop ret %d", ret);
      return -1;
    }
    src = &src_cropped;
  }
  src_cst = cairo_image_surface_create_for_data(src->px[0],
   CAIRO_FORMAT_ARGB32, src->info.w, src->info.h, src->ps[0]);
  if (cairo_surface_status(src_cst)) {
    printke("Cairo Insert: Bad src surface context");
    return -3;
  }
  final_area.x = 0;
  final_area.y = 0;
  sf = aspect_scale(src->info.w, src->info.h, insert->dst_area.w,
   insert->dst_area.h, &final_area.w, &final_area.h);
  kr_rect_center(&final_area, &insert->dst_area);
  cr = cairo_create(dst_cst);
  cairo_translate(cr, final_area.x, final_area.y);
  cairo_translate(cr, final_area.w / 2, final_area.h / 2);
  cairo_rotate(cr, insert->rotation * M_PI/180);
  cairo_scale(cr, sf, sf);
  cairo_translate(cr, src->info.w / -2, src->info.h / -2);
  cairo_set_source_surface(cr, src_cst, 0, 0);
  cairo_paint_with_alpha(cr, insert->opacity);
  cairo_surface_destroy(dst_cst);
  cairo_surface_destroy(src_cst);
  cairo_destroy(cr);
  return 0;
}

static int handle_input_format(kr_image *conv, kr_image *dst, kr_image *src) {
  kr_convert convert;
  int ret;
  if (src->info.fmt != KR_PIXELS_ARGB) {
    conv->info.w = src->info.w;
    conv->info.h = src->info.h;
    conv->info.fmt = KR_PIXELS_ARGB;
    conv->ps[0] = src->info.w * 4;
    kr_image_pixels_alloc(conv);
    kr_image_convert_init(&convert);
    ret = kr_image_convert(&convert, conv, src);
    if (ret != 0) {
       kr_image_pixels_free(conv);
       return -1;
    }
    kr_image_convert_clear(&convert);
    return 1;
  }
  return 0;
}

static int handle_output_format(kr_image *conv, kr_image *dst, kr_image *src) {
  kr_convert convert;
  int ret;
  if (dst->info.fmt != KR_PIXELS_ARGB) {
    kr_image_convert_init(&convert);
    ret = kr_image_convert(&convert, dst, conv);
    if (ret != 0) {
      kr_image_pixels_free(conv);
      return -1;
    }
    kr_image_convert_clear(&convert);
  }
  if (src->info.fmt != KR_PIXELS_ARGB) {
    kr_image_pixels_free(conv);
  }
  return 0;
}

int kr_image_insert_adv(kr_image *dst, kr_image *src, kr_insert *insert) {
  int ret;
  kr_insert ins;
  kr_image conv_src;
  memset(&conv_src, 0, sizeof(conv_src));
  if (dst == NULL || src == NULL || insert == NULL) {
    printke("Image insert called with a null argument");
    return -1;
  }
  if (rect_clipped_out(&insert->dst_area, dst->info.w, dst->info.h)) return -2;
  ins = *insert;
  clamp_rect(&ins.src_crop, src->info.w, src->info.h);
  clamp_rect(&ins.dst_area, dst->info.w, dst->info.h);
  if ((ins.rotation == 0.0f) && (ins.opacity == 1.0f)) {
    return kr_image_insert(dst, src, &ins.dst_area, &ins.src_crop);
  }
  ret = handle_input_format(&conv_src, dst, src);
  if (ret == -1) return ret;
  if (ret == 0) {
    ret = cairo_insert(dst, src, &ins);
  } else {
    ret = cairo_insert(dst, &conv_src, &ins);
    if (ret != 0) {
      printke("Image insert adv: cairo insert failed");
      return -4;
    }
    ret = handle_output_format(&conv_src, dst, src);
    if (ret != 0) {
      printke("Image insert adv: Handle output format failed");
      return -5;
    }
  }
  return 0;
}
