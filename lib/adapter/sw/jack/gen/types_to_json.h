#if !defined(_ADAPTER_SW_JACK_GEN_TYPES_TO_JSON_H)
#define _ADAPTER_SW_JACK_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_jack_info_pack_json(char *json, void *info, int max);
int kr_jack_path_info_pack_json(char *json, void *info, int max);
#endif

