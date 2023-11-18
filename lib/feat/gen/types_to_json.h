#if !defined(_FEAT_GEN_TYPES_TO_JSON_H)
#define _FEAT_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_feature_aspect_pack_json(char *json, void *info, int max);
int kr_feature_pack_json(char *json, void *info, int max);
#endif

