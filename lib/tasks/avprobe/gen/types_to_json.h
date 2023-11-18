#if !defined(_TASKS_AVPROBE_GEN_TYPES_TO_JSON_H)
#define _TASKS_AVPROBE_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_avprobe_format_info_pack_json(char *json, void *info, int max);
int kr_avprobe_stream_info_pack_json(char *json, void *info, int max);
int kr_avprobe_media_info_pack_json(char *json, void *info, int max);
int kr_avprobe_params_info_pack_json(char *json, void *info, int max);
int kr_avprobe_status_info_pack_json(char *json, void *info, int max);
int kr_avprobe_info_pack_json(char *json, void *info, int max);
#endif

