#if !defined(_tasks_xmms_types_H)
# define _tasks_xmms_types_H (1)

#include <stdint.h>
#include <limits.h>

typedef struct {
  char filename[PATH_MAX];
  char env[32];
} kr_xmms_params_info;

typedef struct {
  int coconut;
} kr_xmms_status_info;

typedef struct {
  kr_xmms_params_info params;
  kr_xmms_status_info status;
} kr_xmms_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
