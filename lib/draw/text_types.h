#ifndef KRAD_TEXT_COMMON_H
#define KRAD_TEXT_COMMON_H

typedef struct kr_text_info kr_text_info;

struct kr_text_info {
  char string[512];
  char font[256];
  float red;
  float green;
  float blue;
};

#include "gen/text_types_to_json.h"
#include "gen/text_types_from_json.h"
#include "gen/text_types_to_ebml.h"
#include "gen/text_types_from_ebml.h"
#include "gen/text_types_to_text.h"
#include "gen/text_types_helpers.h"

#endif
