#include "types_helpers.h"

#include <krad/util/util.h>
int kr_avconv_status_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_STATUS_INFO_META_INFO, 1);
  return res;
}

int kr_avconv_status_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_STATUS_INFO_META_INFO, 2);
  return res;
}

int kr_avconv_status_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_STATUS_INFO_META_INFO, 3);
  return res;
}

int kr_avconv_status_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVCONV_STATUS_INFO_META_INFO);
  return res;
}

int kr_avconv_status_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVCONV_STATUS_INFO_META_INFO);
  return res;
}

int kr_avconv_params_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_PARAMS_INFO_META_INFO, 1);
  return res;
}

int kr_avconv_params_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_PARAMS_INFO_META_INFO, 2);
  return res;
}

int kr_avconv_params_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_PARAMS_INFO_META_INFO, 3);
  return res;
}

int kr_avconv_params_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVCONV_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avconv_params_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVCONV_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avconv_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_INFO_META_INFO, 1);
  return res;
}

int kr_avconv_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_INFO_META_INFO, 2);
  return res;
}

int kr_avconv_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVCONV_INFO_META_INFO, 3);
  return res;
}

int kr_avconv_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVCONV_INFO_META_INFO);
  return res;
}

int kr_avconv_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVCONV_INFO_META_INFO);
  return res;
}

