#if !defined(_TUNER_GEN_TYPES_TO_JSON_H)
#define _TUNER_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../hls_types.h"
int kr_hls_variant_pack_json(char *json, void *info, int max);
int kr_hls_rendition_pack_json(char *json, void *info, int max);
int kr_hls_master_playlist_pack_json(char *json, void *info, int max);
int kr_hls_media_segment_pack_json(char *json, void *info, int max);
int kr_hls_media_playlist_pack_json(char *json, void *info, int max);
int kr_hls_playlist_pack_json(char *json, void *info, int max);
int kr_hls_tuner_info_pack_json(char *json, void *info, int max);
#endif

