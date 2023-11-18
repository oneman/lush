#if !defined(_IMAGE_GEN_TYPES_HELPERS_H)
#define _IMAGE_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_pixel_fmt(char *str);
char *kr_pixel_fmt_to_str(int val);
int str_to_kr_image_type(char *str);
char *kr_image_type_to_str(int val);
int kr_rect_init(void *info);
int kr_rect_valid(void *info);
int kr_rect_random(void *info);
int kr_rect_patch_path(kr_patch *patch, kr_path *path);
int kr_rect_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_image_info_init(void *info);
int kr_image_info_valid(void *info);
int kr_image_info_random(void *info);
int kr_image_info_patch_path(kr_patch *patch, kr_path *path);
int kr_image_info_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_coded_image_type(char *str);
char *kr_coded_image_type_to_str(int val);
int kr_coded_image_info_init(void *info);
int kr_coded_image_info_valid(void *info);
int kr_coded_image_info_random(void *info);
int kr_coded_image_info_patch_path(kr_patch *patch, kr_path *path);
int kr_coded_image_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

