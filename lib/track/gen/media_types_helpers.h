#if !defined(_TRACK_GEN_MEDIA_TYPES_HELPERS_H)
#define _TRACK_GEN_MEDIA_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../media_types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_media_source_format(char *str);
char *kr_media_source_format_to_str(int val);
int kr_media_source_info_init(void *info);
int kr_media_source_info_valid(void *info);
int kr_media_source_info_random(void *info);
int kr_media_source_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_source_info_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_media_packet_type(char *str);
char *kr_media_packet_type_to_str(int val);
int kr_media_packet_init(void *info);
int kr_media_packet_valid(void *info);
int kr_media_packet_random(void *info);
int kr_media_packet_patch_path(kr_patch *patch, kr_path *path);
int kr_media_packet_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

