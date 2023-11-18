#include <krad/app/debug.h>
#include "crop.h"

int kr_image_crop(kr_image *dst, kr_image *src, kr_rect *crop) {
  if (dst == NULL || src == NULL || crop == NULL) return -1;
  if (crop->x < 0 || crop->y < 0) return -2;
  if (crop->w > src->info.w || crop->h > src->info.h) return -3;
  dst->info.fmt = src->info.fmt;
  dst->info.w = crop->w;
  dst->info.h = crop->h;
  dst->ps[0] = src->ps[0];
  dst->ps[1] = src->ps[1];
  dst->ps[2] = src->ps[2];
  dst->ps[3] = src->ps[3];
  dst->px[0] = src->px[0];
  dst->px[1] = src->px[1];
  dst->px[2] = src->px[2];
  dst->px[3] = src->px[3];
  dst->px[0] += crop->y * src->ps[0];
  if (dst->info.fmt == KR_PIXELS_YUV422P
   || dst->info.fmt == KR_PIXELS_YVU422P) {
    dst->px[0] += (crop->x - (crop->x % 2)) * 2;
  } else if (dst->info.fmt == KR_PIXELS_ARGB) {
    dst->px[0] += crop->x * 4;
  } else {
    dst->px[0] += crop->x * 1;
    dst->px[1] += crop->y * src->ps[1];
    dst->px[1] += crop->x * 1;
    dst->px[2] += crop->y * src->ps[2];
    dst->px[2] += crop->x * 1;
  }
  return 0;
}
