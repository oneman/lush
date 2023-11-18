#if !defined(_IMAGE_GEN_TYPES_TO_JSON_H)
#define _IMAGE_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_rect_pack_json(char *json, void *info, int max);
int kr_image_info_pack_json(char *json, void *info, int max);
int kr_coded_image_info_pack_json(char *json, void *info, int max);
#endif

