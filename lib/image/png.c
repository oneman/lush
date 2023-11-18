#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <cairo/cairo.h>
#include <krad/image/image.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>

typedef struct {
  uint8_t *data;
  size_t pos;
  size_t sz;
} png_coding_state;

static cairo_status_t read_cb(void *user, unsigned char *data,
 unsigned int length) {
  png_coding_state *state;
  state = (png_coding_state *)user;
  if (state->pos + length > state->sz) return CAIRO_STATUS_READ_ERROR;
  memcpy(data, state->data + state->pos, length);
  state->pos += length;
  return CAIRO_STATUS_SUCCESS;
}

static cairo_status_t write_cb(void *user, const unsigned char *data,
 unsigned int length) {
  png_coding_state *state;
  state = (png_coding_state *)user;
  if (state->pos + length > state->sz) return CAIRO_STATUS_WRITE_ERROR;
  memcpy(state->data + state->pos, data, length);
  state->pos += length;
  return CAIRO_STATUS_SUCCESS;
}

int kr_image_to_png(kr_coded_image *coded, kr_image *image) {
  int ret;
  cairo_surface_t *cst;
  png_coding_state state;
  if (coded == NULL || image == NULL) return 1;
  cst = cairo_image_surface_create_for_data(image->px[0],
   CAIRO_FORMAT_ARGB32, image->info.w, image->info.h, image->ps[0]);
  if (cairo_surface_status(cst)) return 1;
  state.data = coded->data;
  state.pos = 0;
  state.sz = coded->info.sz;
  ret = cairo_surface_write_to_png_stream(cst, write_cb, &state);
  cairo_surface_destroy(cst);
  if (ret == CAIRO_STATUS_SUCCESS) {
    coded->info.sz = state.pos;
  }
  return ret;
}

int kr_image_from_png(kr_image *image, kr_coded_image *coded) {
  int y;
  uint8_t *cd;
  uint8_t *kd;
  cairo_surface_t *cst;
  png_coding_state state;
  if (coded == NULL || image == NULL) return 1;
  state.data = coded->data;
  state.pos = 0;
  state.sz = coded->info.sz;
  cst = cairo_image_surface_create_from_png_stream(read_cb, &state);
  if (cairo_surface_status(cst)) return 1;
  image->info.fmt = KR_PIXELS_ARGB;
  image->info.w = cairo_image_surface_get_width(cst);
  image->info.h = cairo_image_surface_get_height(cst);
  if (image->px[0] == NULL) kr_image_pixels_alloc(image);
  cairo_surface_flush(cst);
  cd = cairo_image_surface_get_data(cst);
  kd = image->px[0];
  for (y = 0; y < image->info.h; y++) {
    memcpy(kd, cd, image->info.w * 4);
    cd += cairo_image_surface_get_stride(cst);
    kd += image->ps[0];
  }
  cairo_surface_destroy(cst);
  return 0;
}
