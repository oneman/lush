#if !defined(_ops_types_H)
# define _ops_types_H (1)

#include <krad/time/types.h>
#include "media/types.h"

typedef struct {
  char sysname[16];
} kr_ops_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
