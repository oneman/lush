#if !defined(_image_convert_H)
# define _image_convert_H (1)

#include <string.h>
#include <libswscale/swscale.h>
#include <krad/image/image.h>

typedef struct kr_convert kr_convert;

struct kr_convert {
  struct SwsContext *sws;
  int quality;
};

void kr_image_convert_init(kr_convert *conv);
void kr_image_convert_clear(kr_convert *conv);
int kr_image_convert(kr_convert *conv, kr_image *dst, kr_image *src);

#endif
