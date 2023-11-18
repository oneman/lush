#if !defined(_TASKS_XMMS_GEN_TYPES_FROM_JSON_H)
#define _TASKS_XMMS_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_xmms_params_info_unpack_json(char *json, void *info);
int kr_xmms_status_info_unpack_json(char *json, void *info);
int kr_xmms_info_unpack_json(char *json, void *info);
#endif

