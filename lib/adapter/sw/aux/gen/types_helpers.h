#if !defined(_ADAPTER_SW_AUX_GEN_TYPES_HELPERS_H)
#define _ADAPTER_SW_AUX_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_aux_port_state(char *str);
char *kr_aux_port_state_to_str(int val);
int kr_aux_info_init(void *info);
int kr_aux_info_valid(void *info);
int kr_aux_info_random(void *info);
int kr_aux_info_patch_path(kr_patch *patch, kr_path *path);
int kr_aux_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_aux_path_info_init(void *info);
int kr_aux_path_info_valid(void *info);
int kr_aux_path_info_random(void *info);
int kr_aux_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_aux_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

