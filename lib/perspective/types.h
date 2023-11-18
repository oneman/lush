#ifndef KRAD_PERSPECTIVE_COMMON_H
#define KRAD_PERSPECTIVE_COMMON_H

#include <inttypes.h>

typedef struct kr_perspective_view kr_perspective_view;
typedef struct kr_pos kr_pos;

struct kr_pos {
  uint32_t x;
  uint32_t y;
};

struct kr_perspective_view {
  kr_pos top_left;
  kr_pos top_right;
  kr_pos bottom_left;
  kr_pos bottom_right;
};

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
