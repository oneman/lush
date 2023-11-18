#if !defined(_image_types_H)
# define _image_types_H (1)

#include <inttypes.h>

typedef enum {
  KR_PIXELS_ARGB = 1,
  KR_PIXELS_YUV422P,
  KR_PIXELS_YVU422P,
  KR_PIXELS_YUV444,
  KR_PIXELS_YUV420,
  KR_PIXELS_YUV422
} kr_pixel_fmt;

typedef enum {
  KR_IMAGE_ENC = 1,
  KR_IMAGE_RAW
} kr_image_type;

typedef struct {
  int x; /* -128000, 128000, 0 */
  int y; /* -128000, 128000, 0 */
  int w; /* 1, 256000, 7680 */
  int h; /* 1, 256000, 4320 */
} kr_rect;

typedef struct {
  uint16_t w; /* 1, 65535, 640 */
  uint16_t h; /* 1, 65535, 360 */
  kr_pixel_fmt fmt;
} kr_image_info;

typedef enum {
  KR_CODED_IMAGE_PNG = 1,
  KR_CODED_IMAGE_JPG,
  KR_CODED_IMAGE_GIF,
  KR_CODED_IMAGE_VP8,
  KR_CODED_IMAGE_VP9,
  KR_CODED_IMAGE_DAALA,
  KR_CODED_IMAGE_THEORA,
  KR_CODED_IMAGE_Y4M,
  KR_CODED_IMAGE_H264,
} kr_coded_image_type;

typedef struct {
  kr_coded_image_type fmt;
  uint64_t sz;
  kr_image_info image;
} kr_coded_image_info;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
