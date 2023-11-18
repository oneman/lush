#if !defined(_CONTAINER_MPEG_TS_GEN_TYPES_HELPERS_H)
#define _CONTAINER_MPEG_TS_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_mts_stream_type(char *str);
char *kr_mts_stream_type_to_str(int val);
int kr_mts_packet_info_init(void *info);
int kr_mts_packet_info_valid(void *info);
int kr_mts_packet_info_random(void *info);
int kr_mts_packet_info_patch_path(kr_patch *patch, kr_path *path);
int kr_mts_packet_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_mts_stream_info_init(void *info);
int kr_mts_stream_info_valid(void *info);
int kr_mts_stream_info_random(void *info);
int kr_mts_stream_info_patch_path(kr_patch *patch, kr_path *path);
int kr_mts_stream_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_mts_program_info_init(void *info);
int kr_mts_program_info_valid(void *info);
int kr_mts_program_info_random(void *info);
int kr_mts_program_info_patch_path(kr_patch *patch, kr_path *path);
int kr_mts_program_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_mts_info_init(void *info);
int kr_mts_info_valid(void *info);
int kr_mts_info_random(void *info);
int kr_mts_info_patch_path(kr_patch *patch, kr_path *path);
int kr_mts_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

