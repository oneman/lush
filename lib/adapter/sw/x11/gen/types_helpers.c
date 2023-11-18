#include "types_helpers.h"

#include <krad/util/util.h>
int kr_x11_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_INFO_META_INFO, 1);
  return res;
}

int kr_x11_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_INFO_META_INFO, 2);
  return res;
}

int kr_x11_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_INFO_META_INFO, 3);
  return res;
}

int kr_x11_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_X11_INFO_META_INFO);
  return res;
}

int kr_x11_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_X11_INFO_META_INFO);
  return res;
}

int kr_x11_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_x11_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_x11_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_X11_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_x11_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_X11_PATH_INFO_META_INFO);
  return res;
}

int kr_x11_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_X11_PATH_INFO_META_INFO);
  return res;
}

