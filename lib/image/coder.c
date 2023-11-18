#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/app/debug.h>

int kr_image_decode(kr_image *image, kr_coded_image *coded_image) {
  switch (coded_image->info.image.fmt) {
    case KR_CODED_IMAGE_PNG:
      return kr_image_from_png(image, coded_image);
    case KR_CODED_IMAGE_JPG:
      return kr_image_from_jpeg(image, coded_image);
    default: return 1;
  }
}

int kr_image_encode(kr_coded_image *coded_image, kr_image *image) {
  switch (coded_image->info.image.fmt) {
    case KR_CODED_IMAGE_PNG:
      return kr_image_to_png(coded_image, image);
    case KR_CODED_IMAGE_JPG:
      return kr_image_to_jpeg(coded_image, image);
    default: return 1;
  }
}

int kr_image_pixels_alloc(kr_image *image) {
  int ret;
  size_t sz;
  kr_image_setup setup;
  if (image == NULL) return -1;
  sz = kr_image_sizeof(&image->info);
  setup.mem = kr_alloc(sz);
  setup.info = image->info;
  setup.img = image;
  ret = kr_image_init2(&setup);
  if (ret) printke("Image: init of pixels was a fail!");
  return 0;
}

int kr_image_pixels_free(kr_image *image) {
  if (image == NULL) return -1;
  if (image->px[0]) free(image->px[0]);
  image->px[0] = NULL;
  return 0;
}

int kr_image_pixels_clear(kr_image *image) {
  if (image == NULL || image->px[0] == NULL) return -1;
  kr_image_fill(image, 0, 0, 0);
  return 0;
}

int kr_image_same_pixels(kr_image *img1, kr_image *img2) {
  size_t sz;
  if (img1 == NULL || img2 == NULL) return 0;
  if (memcmp(&img1->info, &img2->info, sizeof(kr_image_info))) return 0;
  sz = kr_image_sizeof(&img1->info);
  if (sz <= 0) return 0;
  return !memcmp(img1->px[0], img2->px[0], sz);
}

int kr_coded_image_data_alloc(kr_coded_image *coded_image, size_t size) {
  coded_image->data = malloc(size);
  coded_image->info.sz = size;
  return 0;
}

int kr_coded_image_data_free(kr_coded_image *coded_image) {
  if (coded_image == NULL) return -1;
  if (coded_image->data)
    free(coded_image->data);
  coded_image->data = NULL;
  return 0;
}

int kr_coded_image_data_clear(kr_coded_image *coded_image) {
  if (coded_image == NULL || coded_image->data == NULL) return -1;
  memset(coded_image->data, 0, coded_image->info.sz);
  return 0;
}
