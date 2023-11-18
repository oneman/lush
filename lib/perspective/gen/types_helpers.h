#if !defined(_PERSPECTIVE_GEN_TYPES_HELPERS_H)
#define _PERSPECTIVE_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_pos_init(void *info);
int kr_pos_valid(void *info);
int kr_pos_random(void *info);
int kr_pos_patch_path(kr_patch *patch, kr_path *path);
int kr_pos_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_perspective_view_init(void *info);
int kr_perspective_view_valid(void *info);
int kr_perspective_view_random(void *info);
int kr_perspective_view_patch_path(kr_patch *patch, kr_path *path);
int kr_perspective_view_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

