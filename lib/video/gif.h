#if !defined(_image_gif_H)
# define _image_gif_H (1)

#include <inttypes.h>
#include <krad/image/image.h>

typedef struct {
  size_t sz;
  /* type ? meta? */
} kr_data_info;

typedef struct {
  uint8_t *buf;
  kr_data_info info;
} kr_data;

typedef struct kr_gif kr_gif;

typedef struct {
  int w;
  int h;
} kr_gif_info;

int kr_gif_skip_frame(kr_gif *kg);
int kr_gif_get_frame(kr_gif *kg, kr_frame *frame);
int kr_gif_get_info(kr_gif *kg, kr_gif_info *info);
int kr_gif_close(kr_gif *kg);
kr_gif *kr_gif_open(kr_data *data);

#endif
