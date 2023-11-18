#if !defined(_TUNER_GEN_TYPES_TO_TEXT_H)
#define _TUNER_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../hls_types.h"
int kr_hls_variant_to_text(char *text, void *info, int max);
int kr_hls_rendition_to_text(char *text, void *info, int max);
int kr_hls_master_playlist_to_text(char *text, void *info, int max);
int kr_hls_media_segment_to_text(char *text, void *info, int max);
int kr_hls_media_playlist_to_text(char *text, void *info, int max);
int kr_hls_playlist_to_text(char *text, void *info, int max);
int kr_hls_tuner_info_to_text(char *text, void *info, int max);
#endif

