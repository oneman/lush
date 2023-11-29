#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_file_access_mode(char *str) {
  return str_to_enum(KR_KR_FILE_ACCESS_MODE_META_INFO, str);
}

char *kr_file_access_mode_to_str(int val) {
  return enum_to_str(KR_KR_FILE_ACCESS_MODE_META_INFO, val);
}

int kr_file2_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FILE2_INFO_META_INFO, 1);
  return res;
}

int kr_file2_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FILE2_INFO_META_INFO, 2);
  return res;
}

int kr_file2_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FILE2_INFO_META_INFO, 3);
  return res;
}

int kr_file2_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_FILE2_INFO_META_INFO);
  return res;
}

int kr_file2_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_FILE2_INFO_META_INFO);
  return res;
}

