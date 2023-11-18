#if !defined(_TASKS_YDL_GEN_TYPES_FROM_JSON_H)
#define _TASKS_YDL_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_ydl_format_info_unpack_json(char *json, void *info);
int kr_ydl_video_info_unpack_json(char *json, void *info);
int kr_ydl_params_info_unpack_json(char *json, void *info);
int kr_ydl_status_info_unpack_json(char *json, void *info);
int kr_ydl_info_unpack_json(char *json, void *info);
#endif

