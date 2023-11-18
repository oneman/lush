#if !defined(_TIME_GEN_TYPES_TO_JSON_H)
#define _TIME_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_timestamp_info_pack_json(char *json, void *info, int max);
#endif

