#include "pool.h"

kr_image_pool *kr_image_pool_create(kr_image *image, size_t len) {
  kr_pool_setup setup;
  setup.slices = len;
  setup.overlay = image;
  setup.overlay_sz = sizeof(kr_image);
  setup.shared = 1;
  setup.size = image->info.w * image->info.h * 4;
  return kr_pool_create(&setup);
}

int kr_image_pool_getimage(kr_image_pool *image_pool, kr_image *image) {
  kr_pool_overlay_get_copy(image_pool, image);
  image->px[0] = kr_pool_slice(image_pool);
  if (image->px[0] == NULL) return 0;
  /* FIXME */
  image->px[1] = image->px[0] + (image->ps[0] * image->info.h);
  image->px[2] = image->px[1] + (image->ps[1] * (image->info.h/2));
  image->px[3] = 0;
  return 1;
}

int kr_image_pool_destroy(kr_image_pool *image_pool) {
  return kr_pool_destroy(image_pool);
}
