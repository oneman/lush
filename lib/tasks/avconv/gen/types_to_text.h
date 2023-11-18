#if !defined(_TASKS_AVCONV_GEN_TYPES_TO_TEXT_H)
#define _TASKS_AVCONV_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_avconv_status_info_to_text(char *text, void *info, int max);
int kr_avconv_params_info_to_text(char *text, void *info, int max);
int kr_avconv_info_to_text(char *text, void *info, int max);
#endif

