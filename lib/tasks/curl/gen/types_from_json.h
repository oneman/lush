#if !defined(_TASKS_CURL_GEN_TYPES_FROM_JSON_H)
#define _TASKS_CURL_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_curl_status_info_unpack_json(char *json, void *info);
int kr_curl_params_info_unpack_json(char *json, void *info);
int kr_curl_info_unpack_json(char *json, void *info);
#endif

