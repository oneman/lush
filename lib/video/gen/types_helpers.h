#if !defined(_VIDEO_GEN_TYPES_HELPERS_H)
#define _VIDEO_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_fps_info_init(void *info);
int kr_fps_info_valid(void *info);
int kr_fps_info_random(void *info);
int kr_fps_info_patch_path(kr_patch *patch, kr_path *path);
int kr_fps_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

