#include <krad/app/debug.h>
#include "image.h"

#define KR_IMAGE_ALIGN 64

typedef struct {
  int offset[4];
  int stride[4];
  int width[2];
  int height[2];
} kr_image_alignment;

static void rgb_to_yuv(uint8_t *y, uint8_t *u, uint8_t *v, uint8_t r,
 uint8_t g, uint8_t b) {
  *y  = (0.257 * r) + (0.504 * g) + (0.098 * b) + 16;
  *u = -(0.148 * r) - (0.291 * g) + (0.439 * b) + 128;
  *v =  (0.439 * r) - (0.368 * g) - (0.071 * b) + 128;
}

static ssize_t kr_image_pixel_pos(int *plane, int *line, int *col, int *row_sz,
 kr_image_info *info, size_t pos) {
  size_t plane_sz[3];
  if (info->fmt == KR_PIXELS_ARGB) {
    *plane = 0;
    *line = pos / info->w;
    *col = pos % info->w;
    *row_sz = info->w - *col * 4;
    return 0;
  } else if (info->fmt == KR_PIXELS_YUV444) {
    plane_sz[0] = info->w * info->h; 
    plane_sz[1] = info->w * info->h; 
    plane_sz[2] = info->w * info->h;
    *plane = 0;
    if (pos > plane_sz[0] + plane_sz[1]) {
      *plane = 2;
      pos -= plane_sz[0] + plane_sz[1];
    } else if (pos > plane_sz[0]) {
      *plane = 1;
      pos -= plane_sz[0];
    }
    *line = pos / info->w;
    *col = pos % info->w;
    *row_sz = info->w - *col * 4;
    return 0;
  } else if (info->fmt == KR_PIXELS_YUV422) {
  } else if (info->fmt == KR_PIXELS_YUV420) {
  }
  return -1;
}

int kr_image_no_signal(kr_image *img) {
  int j;
  int s;
  int end;
  int offset;
  if (!img) return -1;
  offset = rand() >> 4;
  for (s = 0; s < 4; s++) {
    end = img->ps[s] * img->info.h;
    for (j = 0; j < end; j++) {
      img->px[s][j] = (j + offset) * 0x0080401;
    }
  }
  return 0;
}

int kr_image_get_iovec(struct iovec *v, size_t n, kr_image *img, size_t os) {
  int ret;
  int i;
  int p;
  int e;
  int col_start;
  int row_sz;
  ret = 0;
  p = 0;
  e = 0;
  n--;
  ret = kr_image_pixel_pos(&p, &i, &col_start, &row_sz, &img->info, os);
  if (ret) exit(1);
  for (; p < 3; p++) {
    for (; i < img->info.h; i++) {
      if (col_start && row_sz) {
        v[e].iov_base = img->px[p] + (i * img->ps[p]) + col_start;
        v[e].iov_len = row_sz;
        col_start = 0;
        row_sz = 0;
      } else {
        v[e].iov_base = img->px[p] + (i * img->ps[p]);
        v[e].iov_len = img->info.w;
      }
      if (++e == n) return e;
    }
    i = 0;
  } 
  return e;
}

int kr_image_fill(kr_image *img, uint8_t r, uint8_t g, uint8_t b) {
  int x;
  int y;
  int p;
  char subpixel;
  union {
    uint32_t argb;
    struct {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
    };
    struct {
      uint8_t y;
      uint8_t u;
      uint8_t v;
      uint8_t a1;
    };
  } pixel;
  if (!img) return -1;
  pixel.a = 255;
  pixel.r = r;
  pixel.g = g;
  pixel.b = b;
  switch (img->info.fmt) {
  case KR_PIXELS_ARGB:
    for (y = 0; y < img->info.h; y++) {
      for (x = 0; x < img->info.w; x++) {
        memcpy(img->px[0] + (y * img->ps[0]) + (x * 4), &pixel.argb, 4);
      }
    }
    break;
  case KR_PIXELS_YUV422P:
    rgb_to_yuv(&pixel.y, &pixel.u, &pixel.v, r, g, b);
    /* FIXME */
    break;
  case KR_PIXELS_YVU422P:
    rgb_to_yuv(&pixel.y, &pixel.u, &pixel.v, r, g, b);
    /* FIXME */
  case KR_PIXELS_YUV444:
  case KR_PIXELS_YUV422:
  case KR_PIXELS_YUV420:
    rgb_to_yuv(&pixel.y, &pixel.u, &pixel.v, r, g, b);
    for (p = 0; p < 3; p++) {
      if (p == 0) subpixel = pixel.y;
      if (p == 1) subpixel = pixel.u;
      if (p == 2) subpixel = pixel.v;
      for (y = 0; y < img->info.h; y++) {
        memset(img->px[p] + (img->ps[p] * y), subpixel, img->ps[p]);
      }
    }
    break;
  default:
    printke("Image fill: unknown pixel format");
    return -1;
  }
  return 0;
}

int kr_image_ref(kr_image *image) {
  if (image == NULL) return -1;
  /* FIXME */
  return 0;
}

int kr_image_unref(kr_image *image) {
  if (image == NULL) return -1;
  /* FIXME */
  if ((image->owner != NULL) && (image->release != NULL)) {
   image->release(image->owner);
  }
  return 0;
}

int kr_image_axis_padded(int len) {
  if (len % KR_IMAGE_ALIGN == 0) return len;
  return len + (KR_IMAGE_ALIGN - (len % KR_IMAGE_ALIGN));
}

static int image_align(kr_image_alignment *a, kr_image_info *info) {
  int offset[4];
  int stride[4];
  int width[2];
  int height[2];
  if (!info) return -1;
  memset(&offset, 0, sizeof(offset));
  height[0] = kr_image_axis_padded(info->h);
  width[0] = info->w;
  stride[0] = kr_image_axis_padded(width[0]);
  switch (info->fmt) {
    case KR_PIXELS_ARGB:
      stride[0] *= 4;
      stride[1] = 0;
      stride[2] = 0;
      stride[3] = 0;
      width[1] = 0;
      height[1] = 0;
      break;
    case KR_PIXELS_YUV422P:
      stride[0] *= 2;
      stride[1] = 0;
      stride[2] = 0;
      stride[3] = 0;
      width[1] = 0;
      height[1] = 0;
      break;
    case KR_PIXELS_YVU422P:
      stride[0] *= 2;
      stride[1] = 0;
      stride[2] = 0;
      stride[3] = 0;
      width[1] = 0;
      height[1] = 0;
      break;
    case KR_PIXELS_YUV444:
      stride[1] = stride[0];
      stride[2] = stride[0];
      stride[3] = 0;
      width[1] = width[0];
      height[1] = height[0];
      offset[1] = stride[0] * height[0];
      offset[2] = offset[1] * 2;
      break;
    case KR_PIXELS_YUV422:
      stride[1] = stride[0] / 2;
      stride[2] = stride[1];
      stride[3] = 0;
      width[1] = width[0] / 2;
      height[1] = height[0];
      offset[1] = stride[0] * height[0];
      offset[2] = offset[1] * 2;
      break;
    case KR_PIXELS_YUV420:
      stride[1] = stride[0] / 2;
      stride[2] = stride[1];
      stride[3] = 0;
      width[1] = width[0] / 2;
      height[1] = height[0] / 2;
      offset[1] = stride[0] * height[0];
      offset[2] = offset[1] + (stride[1] * height[1]);
      break;
    default:
      printke("Krad Converter: wrong pix_fmt");
      return -1;
  }
  a->width[0] = width[0];
  a->width[1] = width[1];
  a->height[0] = height[0];
  a->height[1] = height[1];
  a->stride[0] = stride[0];
  a->stride[1] = stride[1];
  a->stride[2] = stride[2];
  a->stride[3] = stride[3];
  a->offset[0] = offset[0];
  a->offset[1] = offset[1];
  a->offset[2] = offset[2];
  a->offset[3] = offset[3];
  return 0;
}

ssize_t kr_image_sizeof_pixels(kr_image_info *info) {
  ssize_t sz;
  if (!info) return -1;
  if (info->w < 1 || info->h < 1) return -2;
  if (info->fmt == KR_PIXELS_ARGB) {
    sz = info->w * info->h * 4;
    return sz;
  } else if (info->fmt == KR_PIXELS_YUV444) {
    sz = info->w * info->h * 3;
    return sz;
  } else if (info->fmt == KR_PIXELS_YUV422) {
    sz = info->w * info->h;
    sz += (info->w/2) * info->h * 2;
    return sz;
  } else if (info->fmt == KR_PIXELS_YUV420) {
    sz = info->w * info->h;
    sz += (info->w/2) * (info->h/2) * 2;
    return sz;
  }
  return -1;
}

ssize_t kr_image_sizeof(kr_image_info *info) {
  size_t sz;
  kr_image_alignment a;
  if (!info) return -1;
  sz = 0;
  image_align(&a, info);
  sz += a.stride[0] * a.height[0];
  sz += a.stride[1] * a.height[1];
  sz += a.stride[2] * a.height[1];
  sz += a.stride[3] * a.height[1];
  return sz;
}

int kr_image_init2(kr_image_setup *s) {
  kr_image_alignment a;
  if (!s || !s->img || !s->mem) return -1;
  image_align(&a, &s->info);
  s->img->info = s->info;
  s->img->ps[0] = a.stride[0];
  s->img->ps[1] = a.stride[1];
  s->img->ps[2] = a.stride[2];
  s->img->ps[3] = a.stride[3];
  s->img->px[0] = s->mem;
  if (a.offset[1]) s->img->px[1] = s->img->px[0] + a.offset[1];
  if (a.offset[2]) s->img->px[2] = s->img->px[0] + a.offset[2];
  if (a.offset[3]) s->img->px[3] = s->img->px[0] + a.offset[3];
  return 0;
}
