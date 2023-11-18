#if !defined(_tasks_livestreamer_types_H)
# define _tasks_livestreamer_types_H (1)

typedef struct {
  char save_file[1024];
  char url[2048];
} kr_livestreamer_params_info;

typedef struct {
  char save_file[1024];
  char url[2048];
} kr_livestreamer_status_info;

typedef struct {
  char save_file[1024];
  char url[2048];
} kr_livestreamer_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
