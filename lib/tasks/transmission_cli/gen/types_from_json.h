#if !defined(_TASKS_TRANSMISSION_CLI_GEN_TYPES_FROM_JSON_H)
#define _TASKS_TRANSMISSION_CLI_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_transmission_cli_status_info_unpack_json(char *json, void *info);
int kr_transmission_cli_params_info_unpack_json(char *json, void *info);
int kr_transmission_cli_info_unpack_json(char *json, void *info);
#endif

