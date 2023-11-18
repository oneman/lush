#if !defined(_compositor_types_H)
# define _compositor_types_H (1)

#define KR_COMPOSITOR_WIDTH 7680
#define KR_COMPOSITOR_HEIGHT 4320
#define KR_COMP_W KR_COMPOSITOR_WIDTH
#define KR_COMP_H KR_COMPOSITOR_HEIGHT

#include <krad/perspective/types.h>
#include <krad/image/types.h>

typedef enum {
  KR_COM_SOURCE = 1,
  KR_COM_OVERLAY,
  KR_COM_INPUT,
  KR_COM_BUS,
  KR_COM_OUTPUT
} kr_compositor_path_type;

typedef enum {
  KR_TEXT = 1,
  KR_VECTOR
} kr_compositor_overlay_type;

typedef struct kr_compositor_source_info kr_compositor_source_info;
typedef struct kr_compositor_input_info kr_compositor_input_info;
typedef struct kr_compositor_bus_info kr_compositor_bus_info;
typedef struct kr_compositor_output_info kr_compositor_output_info;
typedef struct kr_compositor_path_info kr_compositor_path_info;
typedef struct kr_compositor_overlay_info kr_compositor_overlay_info;

struct kr_compositor_output_info {
  float opacity; /* 0.0, 1.0, 1.0 */
};

struct kr_compositor_bus_info {
  float opacity; /* 0.0, 1.0, 1.0 */
};

struct kr_compositor_input_info {
  float opacity; /* 0.0, 1.0, 1.0 */
  kr_rect pos;
  kr_rect crop;
  kr_perspective_view view;
  uint32_t z;
  float rotation; /* -360000.0, 360000.0, 0.0 */
};

struct kr_compositor_source_info {
  float opacity; /* 0.0, 1.0, 1.0 */
  kr_rect crop;
  kr_perspective_view view;
};

#include <krad/draw/vector_types.h>
#include <krad/draw/text_types.h>

struct kr_compositor_overlay_info {
  kr_compositor_overlay_type type;
  union {
    kr_text_info text;
    kr_vector_info vector;
  };
};

struct kr_compositor_path_info {
  kr_compositor_path_type type;
  union {
    kr_compositor_source_info source;
    kr_compositor_overlay_info overlay;
    kr_compositor_input_info input;
    kr_compositor_bus_info bus;
    kr_compositor_output_info output;
  };
};

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
