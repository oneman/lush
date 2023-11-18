#if !defined(_ADAPTER_HW_V4L2_GEN_TYPES_TO_TEXT_H)
#define _ADAPTER_HW_V4L2_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_v4l2_path_info_to_text(char *text, void *info, int max);
int kr_v4l2_info_to_text(char *text, void *info, int max);
#endif

