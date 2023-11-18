#if !defined(_tasks_avconv_types_H)
# define _tasks_avconv_types_H (1)

typedef struct {
  char input[1024];
  char output[1024];
} kr_avconv_status_info;

typedef struct {
  char input[1024];
  char output[1024];
} kr_avconv_params_info;

typedef struct {
  char input[1024];
  char output[1024];
} kr_avconv_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
