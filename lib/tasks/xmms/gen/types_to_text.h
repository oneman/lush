#if !defined(_TASKS_XMMS_GEN_TYPES_TO_TEXT_H)
#define _TASKS_XMMS_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_xmms_params_info_to_text(char *text, void *info, int max);
int kr_xmms_status_info_to_text(char *text, void *info, int max);
int kr_xmms_info_to_text(char *text, void *info, int max);
#endif

