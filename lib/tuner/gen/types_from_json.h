#if !defined(_TUNER_GEN_TYPES_FROM_JSON_H)
#define _TUNER_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../hls_types.h"
int kr_hls_variant_unpack_json(char *json, void *info);
int kr_hls_rendition_unpack_json(char *json, void *info);
int kr_hls_master_playlist_unpack_json(char *json, void *info);
int kr_hls_media_segment_unpack_json(char *json, void *info);
int kr_hls_media_playlist_unpack_json(char *json, void *info);
int kr_hls_playlist_unpack_json(char *json, void *info);
int kr_hls_tuner_info_unpack_json(char *json, void *info);
#endif

