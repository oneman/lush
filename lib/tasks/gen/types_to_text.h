#if !defined(_TASKS_GEN_TYPES_TO_TEXT_H)
#define _TASKS_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_fail_info_to_text(char *text, void *info, int max);
int kr_task_status_to_text(char *text, void *info, int max);
int kr_task_params_info_to_text(char *text, void *info, int max);
int kr_task_info_to_text(char *text, void *info, int max);
#endif

