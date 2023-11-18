#if !defined(_adapter_hw_v4l2_types_H)
# define _adapter_hw_v4l2_types_H (1)

typedef enum {
  KR_V4L2_VOID,
  KR_V4L2_OPEN,
  KR_V4L2_CAPTURE
} kr_v4l2_state;

typedef struct {
  int width;
  int height;
  int num;
  int den;
  int format;
} kr_v4l2_path_info;

typedef struct {
  int dev;
  int priority;
  kr_v4l2_state state;
} kr_v4l2_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
