#if !defined(_TASKS_TRANSMISSION_CLI_GEN_TYPES_TO_TEXT_H)
#define _TASKS_TRANSMISSION_CLI_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_transmission_cli_status_info_to_text(char *text, void *info, int max);
int kr_transmission_cli_params_info_to_text(char *text, void *info, int max);
int kr_transmission_cli_info_to_text(char *text, void *info, int max);
#endif

