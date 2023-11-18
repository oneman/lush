#if !defined(_OPS_MEDIA_GEN_TYPES_TO_JSON_H)
#define _OPS_MEDIA_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_media_file_info_pack_json(char *json, void *info, int max);
int kr_media_metadata_info_pack_json(char *json, void *info, int max);
int kr_media_url_info_pack_json(char *json, void *info, int max);
int kr_media_item_info_pack_json(char *json, void *info, int max);
int kr_media_info_pack_json(char *json, void *info, int max);
#endif

