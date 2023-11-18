#include "types_helpers.h"

#include <krad/util/util.h>
int kr_timestamp_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TIMESTAMP_INFO_META_INFO, 1);
  return res;
}

int kr_timestamp_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TIMESTAMP_INFO_META_INFO, 2);
  return res;
}

int kr_timestamp_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TIMESTAMP_INFO_META_INFO, 3);
  return res;
}

int kr_timestamp_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TIMESTAMP_INFO_META_INFO);
  return res;
}

int kr_timestamp_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TIMESTAMP_INFO_META_INFO);
  return res;
}

int str_to_kr_easing(char *str) {
  return str_to_enum(KR_KR_EASING_META_INFO, str);
}

char *kr_easing_to_str(int val) {
  return enum_to_str(KR_KR_EASING_META_INFO, val);
}

