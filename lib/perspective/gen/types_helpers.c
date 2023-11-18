#include "types_helpers.h"

#include <krad/util/util.h>
int kr_pos_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_POS_META_INFO, 1);
  return res;
}

int kr_pos_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_POS_META_INFO, 2);
  return res;
}

int kr_pos_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_POS_META_INFO, 3);
  return res;
}

int kr_pos_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_POS_META_INFO);
  return res;
}

int kr_pos_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_POS_META_INFO);
  return res;
}

int kr_perspective_view_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PERSPECTIVE_VIEW_META_INFO, 1);
  return res;
}

int kr_perspective_view_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PERSPECTIVE_VIEW_META_INFO, 2);
  return res;
}

int kr_perspective_view_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PERSPECTIVE_VIEW_META_INFO, 3);
  return res;
}

int kr_perspective_view_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_PERSPECTIVE_VIEW_META_INFO);
  return res;
}

int kr_perspective_view_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_PERSPECTIVE_VIEW_META_INFO);
  return res;
}

