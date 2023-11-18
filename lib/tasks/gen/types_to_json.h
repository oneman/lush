#if !defined(_TASKS_GEN_TYPES_TO_JSON_H)
#define _TASKS_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_fail_info_pack_json(char *json, void *info, int max);
int kr_task_status_pack_json(char *json, void *info, int max);
int kr_task_params_info_pack_json(char *json, void *info, int max);
int kr_task_info_pack_json(char *json, void *info, int max);
#endif

