#if !defined(_PERSPECTIVE_GEN_TYPES_TO_JSON_H)
#define _PERSPECTIVE_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_pos_pack_json(char *json, void *info, int max);
int kr_perspective_view_pack_json(char *json, void *info, int max);
#endif

