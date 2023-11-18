#if !defined(_TASKS_AVPROBE_GEN_TYPES_HELPERS_H)
#define _TASKS_AVPROBE_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_avprobe_codec_type(char *str);
char *kr_avprobe_codec_type_to_str(int val);
int kr_avprobe_format_info_init(void *info);
int kr_avprobe_format_info_valid(void *info);
int kr_avprobe_format_info_random(void *info);
int kr_avprobe_format_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_format_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_avprobe_stream_info_init(void *info);
int kr_avprobe_stream_info_valid(void *info);
int kr_avprobe_stream_info_random(void *info);
int kr_avprobe_stream_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_stream_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_avprobe_media_info_init(void *info);
int kr_avprobe_media_info_valid(void *info);
int kr_avprobe_media_info_random(void *info);
int kr_avprobe_media_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_media_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_avprobe_params_info_init(void *info);
int kr_avprobe_params_info_valid(void *info);
int kr_avprobe_params_info_random(void *info);
int kr_avprobe_params_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_params_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_avprobe_status_info_init(void *info);
int kr_avprobe_status_info_valid(void *info);
int kr_avprobe_status_info_random(void *info);
int kr_avprobe_status_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_status_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_avprobe_info_init(void *info);
int kr_avprobe_info_valid(void *info);
int kr_avprobe_info_random(void *info);
int kr_avprobe_info_patch_path(kr_patch *patch, kr_path *path);
int kr_avprobe_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

