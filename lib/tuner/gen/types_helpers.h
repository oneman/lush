#if !defined(_TUNER_GEN_TYPES_HELPERS_H)
#define _TUNER_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../hls_types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_hls_playlist_type(char *str);
char *kr_hls_playlist_type_to_str(int val);
int str_to_kr_hls_rendition_type(char *str);
char *kr_hls_rendition_type_to_str(int val);
int str_to_kr_hls_media_playlist_type(char *str);
char *kr_hls_media_playlist_type_to_str(int val);
int kr_hls_variant_init(void *info);
int kr_hls_variant_valid(void *info);
int kr_hls_variant_random(void *info);
int kr_hls_variant_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_variant_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_rendition_init(void *info);
int kr_hls_rendition_valid(void *info);
int kr_hls_rendition_random(void *info);
int kr_hls_rendition_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_rendition_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_master_playlist_init(void *info);
int kr_hls_master_playlist_valid(void *info);
int kr_hls_master_playlist_random(void *info);
int kr_hls_master_playlist_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_master_playlist_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_media_segment_init(void *info);
int kr_hls_media_segment_valid(void *info);
int kr_hls_media_segment_random(void *info);
int kr_hls_media_segment_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_media_segment_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_media_playlist_init(void *info);
int kr_hls_media_playlist_valid(void *info);
int kr_hls_media_playlist_random(void *info);
int kr_hls_media_playlist_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_media_playlist_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_playlist_init(void *info);
int kr_hls_playlist_valid(void *info);
int kr_hls_playlist_random(void *info);
int kr_hls_playlist_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_playlist_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_hls_tuner_info_init(void *info);
int kr_hls_tuner_info_valid(void *info);
int kr_hls_tuner_info_random(void *info);
int kr_hls_tuner_info_patch_path(kr_patch *patch, kr_path *path);
int kr_hls_tuner_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

