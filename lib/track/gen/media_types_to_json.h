#if !defined(_TRACK_GEN_MEDIA_TYPES_TO_JSON_H)
#define _TRACK_GEN_MEDIA_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../media_types.h"
int kr_media_source_info_pack_json(char *json, void *info, int max);
int kr_media_packet_pack_json(char *json, void *info, int max);
#endif

