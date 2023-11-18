#ifndef KRAD_VECTOR_COMMON_H
#define KRAD_VECTOR_COMMON_H

typedef struct kr_vector_info kr_vector_info;

typedef enum {
  NOTHING,
  HEX,
  CIRCLE,
  RECT,
  TRIANGLE,
  VIPER,
  METER,
  GRID,
  CURVE,
  ARROW,
  CLOCK,
  SHADOW
} kr_vector_type;

struct kr_vector_info {
  kr_vector_type type;
  float red;
  float green;
  float blue;
};

#include "gen/vector_types_to_json.h"
#include "gen/vector_types_from_json.h"
#include "gen/vector_types_to_ebml.h"
#include "gen/vector_types_from_ebml.h"
#include "gen/vector_types_to_text.h"
#include "gen/vector_types_helpers.h"

#endif
