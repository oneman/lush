#if !defined(_IMAGE_GEN_STASH_TYPES_HELPERS_H)
#define _IMAGE_GEN_STASH_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../stash_types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_image_stash_item_info_init(void *info);
int kr_image_stash_item_info_valid(void *info);
int kr_image_stash_item_info_random(void *info);
int kr_image_stash_item_info_patch_path(kr_patch *patch, kr_path *path);
int kr_image_stash_item_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

