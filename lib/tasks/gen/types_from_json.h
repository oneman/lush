#if !defined(_TASKS_GEN_TYPES_FROM_JSON_H)
#define _TASKS_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_fail_info_unpack_json(char *json, void *info);
int kr_task_status_unpack_json(char *json, void *info);
int kr_task_params_info_unpack_json(char *json, void *info);
int kr_task_info_unpack_json(char *json, void *info);
#endif

