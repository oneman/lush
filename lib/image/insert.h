#if !defined(_image_insert_H)
# define _image_insert_H (1)

#include "image.h"
#include "crop.h"
#include "convert.h"

typedef struct {
  kr_rect src_crop;
  kr_rect dst_area;
  float opacity;
  float rotation;
} kr_insert;

void kr_rect_scale(kr_rect *r, double s);
double kr_rect_scaler(int max_w, int max_h, int w, int h);
int kr_rect_center(kr_rect *out, kr_rect *container);
int kr_image_insert(kr_image *dst, kr_image *src, kr_rect *area, kr_rect *crop);
int kr_image_insert_adv(kr_image *dst, kr_image *src, kr_insert *insert);

#endif
