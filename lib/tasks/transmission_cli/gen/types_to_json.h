#if !defined(_TASKS_TRANSMISSION_CLI_GEN_TYPES_TO_JSON_H)
#define _TASKS_TRANSMISSION_CLI_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_transmission_cli_status_info_pack_json(char *json, void *info, int max);
int kr_transmission_cli_params_info_pack_json(char *json, void *info, int max);
int kr_transmission_cli_info_pack_json(char *json, void *info, int max);
#endif

