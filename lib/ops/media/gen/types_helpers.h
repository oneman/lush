#if !defined(_OPS_MEDIA_GEN_TYPES_HELPERS_H)
#define _OPS_MEDIA_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_media_event_type(char *str);
char *kr_media_event_type_to_str(int val);
int str_to_kr_media_item_type(char *str);
char *kr_media_item_type_to_str(int val);
int kr_media_file_info_init(void *info);
int kr_media_file_info_valid(void *info);
int kr_media_file_info_random(void *info);
int kr_media_file_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_file_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_media_metadata_info_init(void *info);
int kr_media_metadata_info_valid(void *info);
int kr_media_metadata_info_random(void *info);
int kr_media_metadata_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_metadata_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_media_url_info_init(void *info);
int kr_media_url_info_valid(void *info);
int kr_media_url_info_random(void *info);
int kr_media_url_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_url_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_media_item_info_init(void *info);
int kr_media_item_info_valid(void *info);
int kr_media_item_info_random(void *info);
int kr_media_item_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_item_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_media_info_init(void *info);
int kr_media_info_valid(void *info);
int kr_media_info_random(void *info);
int kr_media_info_patch_path(kr_patch *patch, kr_path *path);
int kr_media_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

