#if !defined(_PERSPECTIVE_GEN_TYPES_FROM_JSON_H)
#define _PERSPECTIVE_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_pos_unpack_json(char *json, void *info);
int kr_perspective_view_unpack_json(char *json, void *info);
#endif

