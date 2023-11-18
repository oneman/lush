#if !defined(_video_types_H)
# define _video_types_H (1)

#include <inttypes.h>

typedef struct {
  int32_t num; /* 1, 2400, 30 */
  int32_t den; /* 1, 2400, 1 */
} kr_fps_info;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
