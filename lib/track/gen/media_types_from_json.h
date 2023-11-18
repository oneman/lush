#if !defined(_TRACK_GEN_MEDIA_TYPES_FROM_JSON_H)
#define _TRACK_GEN_MEDIA_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../media_types.h"
int kr_media_source_info_unpack_json(char *json, void *info);
int kr_media_packet_unpack_json(char *json, void *info);
#endif

