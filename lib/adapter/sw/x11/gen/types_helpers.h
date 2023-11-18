#if !defined(_ADAPTER_SW_X11_GEN_TYPES_HELPERS_H)
#define _ADAPTER_SW_X11_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_x11_info_init(void *info);
int kr_x11_info_valid(void *info);
int kr_x11_info_random(void *info);
int kr_x11_info_patch_path(kr_patch *patch, kr_path *path);
int kr_x11_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_x11_path_info_init(void *info);
int kr_x11_path_info_valid(void *info);
int kr_x11_path_info_random(void *info);
int kr_x11_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_x11_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

