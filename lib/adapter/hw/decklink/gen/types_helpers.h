#if !defined(_ADAPTER_HW_DECKLINK_GEN_TYPES_HELPERS_H)
#define _ADAPTER_HW_DECKLINK_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_decklink_video_conn_type(char *str);
char *kr_decklink_video_conn_type_to_str(int val);
int str_to_kr_decklink_audio_conn_type(char *str);
char *kr_decklink_audio_conn_type_to_str(int val);
int kr_decklink_info_init(void *info);
int kr_decklink_info_valid(void *info);
int kr_decklink_info_random(void *info);
int kr_decklink_info_patch_path(kr_patch *patch, kr_path *path);
int kr_decklink_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_decklink_path_info_init(void *info);
int kr_decklink_path_info_valid(void *info);
int kr_decklink_path_info_random(void *info);
int kr_decklink_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_decklink_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

