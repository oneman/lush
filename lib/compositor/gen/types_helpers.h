#if !defined(_COMPOSITOR_GEN_TYPES_HELPERS_H)
#define _COMPOSITOR_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_compositor_path_type(char *str);
char *kr_compositor_path_type_to_str(int val);
int str_to_kr_compositor_overlay_type(char *str);
char *kr_compositor_overlay_type_to_str(int val);
int kr_compositor_output_info_init(void *info);
int kr_compositor_output_info_valid(void *info);
int kr_compositor_output_info_random(void *info);
int kr_compositor_output_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_output_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_compositor_bus_info_init(void *info);
int kr_compositor_bus_info_valid(void *info);
int kr_compositor_bus_info_random(void *info);
int kr_compositor_bus_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_bus_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_compositor_input_info_init(void *info);
int kr_compositor_input_info_valid(void *info);
int kr_compositor_input_info_random(void *info);
int kr_compositor_input_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_input_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_compositor_source_info_init(void *info);
int kr_compositor_source_info_valid(void *info);
int kr_compositor_source_info_random(void *info);
int kr_compositor_source_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_source_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_compositor_overlay_info_init(void *info);
int kr_compositor_overlay_info_valid(void *info);
int kr_compositor_overlay_info_random(void *info);
int kr_compositor_overlay_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_overlay_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_compositor_path_info_init(void *info);
int kr_compositor_path_info_valid(void *info);
int kr_compositor_path_info_random(void *info);
int kr_compositor_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_compositor_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

