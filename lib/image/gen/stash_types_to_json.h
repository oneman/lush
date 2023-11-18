#if !defined(_IMAGE_GEN_STASH_TYPES_TO_JSON_H)
#define _IMAGE_GEN_STASH_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../stash_types.h"
int kr_image_stash_item_info_pack_json(char *json, void *info, int max);
#endif

