#if !defined(_TASKS_YDL_GEN_TYPES_TO_JSON_H)
#define _TASKS_YDL_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_ydl_format_info_pack_json(char *json, void *info, int max);
int kr_ydl_video_info_pack_json(char *json, void *info, int max);
int kr_ydl_params_info_pack_json(char *json, void *info, int max);
int kr_ydl_status_info_pack_json(char *json, void *info, int max);
int kr_ydl_info_pack_json(char *json, void *info, int max);
#endif

