#if !defined(_tasks_curl_types_H)
# define _tasks_curl_types_H (1)

typedef struct {
  char url[2048];
  char dest[2048];
  int percent_complete;
} kr_curl_status_info;

typedef struct {
  char url[2048];
  char dest[2048];
  int percent_complete;
} kr_curl_params_info;

typedef struct {
  char url[2048];
  char dest[2048];
  int percent_complete;
} kr_curl_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
