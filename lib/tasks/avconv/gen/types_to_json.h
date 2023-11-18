#if !defined(_TASKS_AVCONV_GEN_TYPES_TO_JSON_H)
#define _TASKS_AVCONV_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_avconv_status_info_pack_json(char *json, void *info, int max);
int kr_avconv_params_info_pack_json(char *json, void *info, int max);
int kr_avconv_info_pack_json(char *json, void *info, int max);
#endif

