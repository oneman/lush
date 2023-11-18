#if !defined(_ADAPTER_SW_WAYLAND_GEN_TYPES_HELPERS_H)
#define _ADAPTER_SW_WAYLAND_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_wayland_state(char *str);
char *kr_wayland_state_to_str(int val);
int kr_wayland_info_init(void *info);
int kr_wayland_info_valid(void *info);
int kr_wayland_info_random(void *info);
int kr_wayland_info_patch_path(kr_patch *patch, kr_path *path);
int kr_wayland_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_wayland_path_info_init(void *info);
int kr_wayland_path_info_valid(void *info);
int kr_wayland_path_info_random(void *info);
int kr_wayland_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_wayland_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

