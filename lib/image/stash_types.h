#if !defined(_image_stash_types_H)
# define _image_stash_types_H (1)

#include "types.h"

typedef struct {
  kr_image_type type;
  union {
    kr_coded_image_type enc_fmt;
    kr_pixel_fmt raw_fmt;
  };
  uint32_t width;
  uint32_t height;
  uint64_t sz;
  uint64_t created_on;
} kr_image_stash_item_info;

#include "gen/stash_types_to_json.h"
#include "gen/stash_types_from_json.h"
#include "gen/stash_types_to_ebml.h"
#include "gen/stash_types_from_ebml.h"
#include "gen/stash_types_to_text.h"
#include "gen/stash_types_helpers.h"

#endif
