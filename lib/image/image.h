#include <stdlib.h>
#include <sys/uio.h>
#include "types.h"

typedef struct kr_image kr_image;
typedef struct kr_coded_image kr_coded_image;

#include <krad/xpdr/event.h>

#if !defined(_image_image_H)
# define _image_image_H (1)

struct kr_coded_image {
  uint8_t *data;
  kr_coded_image_info info;
};

struct kr_image {
  uint8_t *px[4];
  int ps[4];
  kr_image_info info;
  void *owner;
  int32_t refs;
  kr_av_release_cb *release;
  kr_av_handler *ready;
};

typedef struct {
  uint8_t *mem;
  kr_image_info info;
  kr_image *img;
} kr_image_setup;

int kr_image_no_signal(kr_image *img);
int kr_image_get_iovec(struct iovec *v, size_t n, kr_image *img, size_t os);
int kr_image_fill(kr_image *img, uint8_t r, uint8_t g, uint8_t b);
int kr_image_ref(kr_image *image);
int kr_image_unref(kr_image *image);
int kr_image_axis_padded(int len);
ssize_t kr_image_sizeof_pixels(kr_image_info *info);
ssize_t kr_image_sizeof(kr_image_info *info);
int kr_image_init2(kr_image_setup *setup);

#endif
