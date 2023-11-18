#if !defined(_RADIO_GEN_LIMIT_TYPES_TO_JSON_H)
#define _RADIO_GEN_LIMIT_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../limit_types.h"
int kr_limit_info_pack_json(char *json, void *info, int max);
int kr_limits_info_pack_json(char *json, void *info, int max);
#endif

