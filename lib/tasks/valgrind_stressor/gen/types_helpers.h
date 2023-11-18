#if !defined(_TASKS_VALGRIND_STRESSOR_GEN_TYPES_HELPERS_H)
#define _TASKS_VALGRIND_STRESSOR_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int kr_valgrind_stressor_status_info_init(void *info);
int kr_valgrind_stressor_status_info_valid(void *info);
int kr_valgrind_stressor_status_info_random(void *info);
int kr_valgrind_stressor_status_info_patch_path(kr_patch *patch, kr_path *path);
int kr_valgrind_stressor_status_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_valgrind_stressor_params_info_init(void *info);
int kr_valgrind_stressor_params_info_valid(void *info);
int kr_valgrind_stressor_params_info_random(void *info);
int kr_valgrind_stressor_params_info_patch_path(kr_patch *patch, kr_path *path);
int kr_valgrind_stressor_params_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_valgrind_stressor_info_init(void *info);
int kr_valgrind_stressor_info_valid(void *info);
int kr_valgrind_stressor_info_random(void *info);
int kr_valgrind_stressor_info_patch_path(kr_patch *patch, kr_path *path);
int kr_valgrind_stressor_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

