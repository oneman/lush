#if !defined(_TASKS_QUVI_GEN_TYPES_HELPERS_H)
#define _TASKS_QUVI_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_media_version_init(void *info);
int kr_media_version_valid(void *info);
int kr_media_version_random(void *info);
int kr_media_version_patch_path(kr_patch *patch, kr_path *path);
int kr_media_version_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_quvi_media_info_init(void *info);
int kr_quvi_media_info_valid(void *info);
int kr_quvi_media_info_random(void *info);
int kr_quvi_media_info_patch_path(kr_patch *patch, kr_path *path);
int kr_quvi_media_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_quvi_status_info_init(void *info);
int kr_quvi_status_info_valid(void *info);
int kr_quvi_status_info_random(void *info);
int kr_quvi_status_info_patch_path(kr_patch *patch, kr_path *path);
int kr_quvi_status_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_quvi_params_info_init(void *info);
int kr_quvi_params_info_valid(void *info);
int kr_quvi_params_info_random(void *info);
int kr_quvi_params_info_patch_path(kr_patch *patch, kr_path *path);
int kr_quvi_params_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_quvi_info_init(void *info);
int kr_quvi_info_valid(void *info);
int kr_quvi_info_random(void *info);
int kr_quvi_info_patch_path(kr_patch *patch, kr_path *path);
int kr_quvi_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

