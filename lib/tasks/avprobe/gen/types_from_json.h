#if !defined(_TASKS_AVPROBE_GEN_TYPES_FROM_JSON_H)
#define _TASKS_AVPROBE_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_avprobe_format_info_unpack_json(char *json, void *info);
int kr_avprobe_stream_info_unpack_json(char *json, void *info);
int kr_avprobe_media_info_unpack_json(char *json, void *info);
int kr_avprobe_params_info_unpack_json(char *json, void *info);
int kr_avprobe_status_info_unpack_json(char *json, void *info);
int kr_avprobe_info_unpack_json(char *json, void *info);
#endif

