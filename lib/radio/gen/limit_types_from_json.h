#if !defined(_RADIO_GEN_LIMIT_TYPES_FROM_JSON_H)
#define _RADIO_GEN_LIMIT_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../limit_types.h"
int kr_limit_info_unpack_json(char *json, void *info);
int kr_limits_info_unpack_json(char *json, void *info);
#endif

