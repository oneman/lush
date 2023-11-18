#if !defined(_IMAGE_GEN_TYPES_FROM_JSON_H)
#define _IMAGE_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_rect_unpack_json(char *json, void *info);
int kr_image_info_unpack_json(char *json, void *info);
int kr_coded_image_info_unpack_json(char *json, void *info);
#endif

