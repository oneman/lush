#if !defined(_time_types_H)
#define _time_types_H (1)

#include <inttypes.h>

typedef struct {
  uint32_t seconds;
  uint32_t nanoseconds;
} kr_timestamp_info;

typedef enum {
  LINEAR = 33999,
  EASEINSINE,
  EASEOUTSINE,
  EASEINOUTSINE,
  EASEINCUBIC,
  EASEOUTCUBIC,
  EASEINOUTCUBIC,
  EASEINOUTELASTIC,
/*  EASEINCIRC,
  EASEOUTCIRC,
  EASEINOUTCIRC,
  EASEINBACK,
  EASEOUTBACK,
  EASEINOUTBACK,
  EASEINBOUNCE,
  EASEOUTBOUNCE,
  EASEINOUTBOUNCE,
  EASEINEXPO,
  EASEOUTEXPO,
  EASEINOUTEXPO,
  EASEINELASTIC,
  EASEOUTELASTIC,
  EASEINOUTELASTIC,
  EASEINQUAD,
  EASEOUTQUAD,
  EASEINOUTQUAD,
  EASEINQUART,
  EASEOUTQUART,
  EASEINOUTQUART,
  EASEINQUINT,
  EASEOUTQUINT,
  EASEINOUTQUINT,
*/
} kr_easing;

#define FIRSTEASING LINEAR
#define LASTEASING EASEINOUTELASTIC

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_helpers.h"
#include "gen/types_to_text.h"

#endif
