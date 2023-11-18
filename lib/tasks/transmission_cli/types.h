#if !defined(_tasks_transmission_cli_types_H)
# define _tasks_transmission_cli_types_H (1)

typedef struct {
  char file[1024];
} kr_transmission_cli_status_info;

typedef struct {
  char file[1024];
} kr_transmission_cli_params_info;

typedef struct {
  char file[1024];
} kr_transmission_cli_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
