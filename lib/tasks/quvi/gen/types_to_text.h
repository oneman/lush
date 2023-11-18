#if !defined(_TASKS_QUVI_GEN_TYPES_TO_TEXT_H)
#define _TASKS_QUVI_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_media_version_to_text(char *text, void *info, int max);
int kr_quvi_media_info_to_text(char *text, void *info, int max);
int kr_quvi_status_info_to_text(char *text, void *info, int max);
int kr_quvi_params_info_to_text(char *text, void *info, int max);
int kr_quvi_info_to_text(char *text, void *info, int max);
#endif

