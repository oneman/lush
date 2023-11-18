#if !defined(_adapter_sw_wayland_types_H)
# define _adapter_sw_wayland_types_H (1)

typedef enum {
  KR_WL_DISCONNECTED = 1,
  KR_WL_CONNECTED
} kr_wayland_state;

typedef struct {
  char display_name[128];
  kr_wayland_state state;
} kr_wayland_info;

typedef struct {
  int width;
  int height;
  int fullscreen;
} kr_wayland_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
