#if !defined(_TASKS_LIVESTREAMER_GEN_TYPES_TO_JSON_H)
#define _TASKS_LIVESTREAMER_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_livestreamer_params_info_pack_json(char *json, void *info, int max);
int kr_livestreamer_status_info_pack_json(char *json, void *info, int max);
int kr_livestreamer_info_pack_json(char *json, void *info, int max);
#endif

