#if !defined(_TASKS_VALGRIND_STRESSOR_GEN_TYPES_FROM_JSON_H)
#define _TASKS_VALGRIND_STRESSOR_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_valgrind_stressor_status_info_unpack_json(char *json, void *info);
int kr_valgrind_stressor_params_info_unpack_json(char *json, void *info);
int kr_valgrind_stressor_info_unpack_json(char *json, void *info);
#endif

