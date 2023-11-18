#if !defined(_TASKS_VALGRIND_STRESSOR_GEN_TYPES_TO_TEXT_H)
#define _TASKS_VALGRIND_STRESSOR_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_valgrind_stressor_status_info_to_text(char *text, void *info, int max);
int kr_valgrind_stressor_params_info_to_text(char *text, void *info, int max);
int kr_valgrind_stressor_info_to_text(char *text, void *info, int max);
#endif

