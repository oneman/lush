#if !defined(_ADAPTER_SW_AUX_GEN_TYPES_FROM_JSON_H)
#define _ADAPTER_SW_AUX_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_aux_info_unpack_json(char *json, void *info);
int kr_aux_path_info_unpack_json(char *json, void *info);
#endif

