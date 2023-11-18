#if !defined(_FEAT_GEN_TYPES_HELPERS_H)
#define _FEAT_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_feature_aspect_type(char *str);
char *kr_feature_aspect_type_to_str(int val);
int kr_feature_aspect_init(void *info);
int kr_feature_aspect_valid(void *info);
int kr_feature_aspect_random(void *info);
int kr_feature_aspect_patch_path(kr_patch *patch, kr_path *path);
int kr_feature_aspect_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_feature_init(void *info);
int kr_feature_valid(void *info);
int kr_feature_random(void *info);
int kr_feature_patch_path(kr_patch *patch, kr_path *path);
int kr_feature_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

