#if !defined(_OPS_MEDIA_GEN_TYPES_FROM_JSON_H)
#define _OPS_MEDIA_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_media_file_info_unpack_json(char *json, void *info);
int kr_media_metadata_info_unpack_json(char *json, void *info);
int kr_media_url_info_unpack_json(char *json, void *info);
int kr_media_item_info_unpack_json(char *json, void *info);
int kr_media_info_unpack_json(char *json, void *info);
#endif

