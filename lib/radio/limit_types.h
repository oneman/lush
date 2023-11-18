#if !defined(_radio_limit_types_H)
# define _radio_limit_types_H (1)

typedef struct kr_limits_info kr_limits_info;
typedef struct kr_limit_info kr_limit_info;

struct kr_limit_info {
  char label[32];
  int value;
};

struct kr_limits_info {
  kr_limit_info limits[256]; /* not null */
  int len;
};

#include "gen/limit_types_to_ebml.h"
#include "gen/limit_types_from_ebml.h"
#include "gen/limit_types_to_json.h"
#include "gen/limit_types_from_json.h"
#include "gen/limit_types_to_text.h"
#include "gen/limit_types_helpers.h"

#endif
