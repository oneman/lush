#if !defined(_ADAPTER_HW_DECKLINK_GEN_TYPES_FROM_JSON_H)
#define _ADAPTER_HW_DECKLINK_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_decklink_info_unpack_json(char *json, void *info);
int kr_decklink_path_info_unpack_json(char *json, void *info);
#endif

