#if !defined(_ADAPTER_HW_V4L2_GEN_TYPES_HELPERS_H)
#define _ADAPTER_HW_V4L2_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_v4l2_state(char *str);
char *kr_v4l2_state_to_str(int val);
int kr_v4l2_path_info_init(void *info);
int kr_v4l2_path_info_valid(void *info);
int kr_v4l2_path_info_random(void *info);
int kr_v4l2_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_v4l2_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_v4l2_info_init(void *info);
int kr_v4l2_info_valid(void *info);
int kr_v4l2_info_random(void *info);
int kr_v4l2_info_patch_path(kr_patch *patch, kr_path *path);
int kr_v4l2_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

