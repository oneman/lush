#if !defined(_ADAPTER_HW_V4L2_GEN_TYPES_TO_JSON_H)
#define _ADAPTER_HW_V4L2_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_v4l2_path_info_pack_json(char *json, void *info, int max);
int kr_v4l2_info_pack_json(char *json, void *info, int max);
#endif

