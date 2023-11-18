#if !defined(_adapter_sw_x11_types_H)
# define _adapter_sw_x11_types_H (1)

typedef struct {
  char display[64];
} kr_x11_info;

typedef struct {
  char display[64];
  uint32_t width;
  uint32_t height;
  uint32_t num;
  uint32_t den;
  int x;
  int y;
} kr_x11_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
