#if !defined(_PLAYLIST_GEN_TYPES_HELPERS_H)
#define _PLAYLIST_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../m3u_types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_m3u_el_type(char *str);
char *m3u_el_type_to_str(int val);
int str_to_kr_m3u_tag_name(char *str);
char *kr_m3u_tag_name_to_str(int val);
int str_to_kr_m3u_tag_val_type(char *str);
char *kr_m3u_tag_val_type_to_str(int val);
int str_to_kr_m3u_tag_attr_name(char *str);
char *kr_m3u_tag_attr_name_to_str(int val);
int kr_m3u_tag_prim_info_init(void *info);
int kr_m3u_tag_prim_info_valid(void *info);
int kr_m3u_tag_prim_info_random(void *info);
int kr_m3u_tag_prim_info_patch_path(kr_patch *patch, kr_path *path);
int kr_m3u_tag_prim_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_m3u_tag_alist_info_init(void *info);
int kr_m3u_tag_alist_info_valid(void *info);
int kr_m3u_tag_alist_info_random(void *info);
int kr_m3u_tag_alist_info_patch_path(kr_patch *patch, kr_path *path);
int kr_m3u_tag_alist_info_diff(kr_diffset *diffset, void *info1, void *info2);
int m3u_tag_info_init(void *info);
int m3u_tag_info_valid(void *info);
int m3u_tag_info_random(void *info);
int m3u_tag_info_patch_path(kr_patch *patch, kr_path *path);
int m3u_tag_info_diff(kr_diffset *diffset, void *info1, void *info2);
int m3u_el_info_init(void *info);
int m3u_el_info_valid(void *info);
int m3u_el_info_random(void *info);
int m3u_el_info_patch_path(kr_patch *patch, kr_path *path);
int m3u_el_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_m3u_info_init(void *info);
int kr_m3u_info_valid(void *info);
int kr_m3u_info_random(void *info);
int kr_m3u_info_patch_path(kr_patch *patch, kr_path *path);
int kr_m3u_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

