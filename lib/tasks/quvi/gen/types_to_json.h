#if !defined(_TASKS_QUVI_GEN_TYPES_TO_JSON_H)
#define _TASKS_QUVI_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_media_version_pack_json(char *json, void *info, int max);
int kr_quvi_media_info_pack_json(char *json, void *info, int max);
int kr_quvi_status_info_pack_json(char *json, void *info, int max);
int kr_quvi_params_info_pack_json(char *json, void *info, int max);
int kr_quvi_info_pack_json(char *json, void *info, int max);
#endif

