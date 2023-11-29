#if !defined(_IO_GEN_TYPES_HELPERS_H)
#define _IO_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_file_access_mode(char *str);
char *kr_file_access_mode_to_str(int val);
int kr_file2_info_init(void *info);
int kr_file2_info_valid(void *info);
int kr_file2_info_random(void *info);
int kr_file2_info_patch_path(kr_patch *patch, kr_path *path);
int kr_file2_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

