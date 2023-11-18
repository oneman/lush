#if !defined(_TASKS_GEN_TYPES_HELPERS_H)
#define _TASKS_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_task_type(char *str);
char *kr_task_type_to_str(int val);
int str_to_kr_task_state(char *str);
char *kr_task_state_to_str(int val);
int str_to_kr_task_event_type(char *str);
char *kr_task_event_type_to_str(int val);
int kr_fail_info_init(void *info);
int kr_fail_info_valid(void *info);
int kr_fail_info_random(void *info);
int kr_fail_info_patch_path(kr_patch *patch, kr_path *path);
int kr_fail_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_task_status_init(void *info);
int kr_task_status_valid(void *info);
int kr_task_status_random(void *info);
int kr_task_status_patch_path(kr_patch *patch, kr_path *path);
int kr_task_status_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_task_params_info_init(void *info);
int kr_task_params_info_valid(void *info);
int kr_task_params_info_random(void *info);
int kr_task_params_info_patch_path(kr_patch *patch, kr_path *path);
int kr_task_params_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_task_info_init(void *info);
int kr_task_info_valid(void *info);
int kr_task_info_random(void *info);
int kr_task_info_patch_path(kr_patch *patch, kr_path *path);
int kr_task_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

