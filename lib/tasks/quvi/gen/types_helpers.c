#include "types_helpers.h"

#include <krad/util/util.h>
int kr_media_version_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_VERSION_META_INFO, 1);
  return res;
}

int kr_media_version_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_VERSION_META_INFO, 2);
  return res;
}

int kr_media_version_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_VERSION_META_INFO, 3);
  return res;
}

int kr_media_version_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_VERSION_META_INFO);
  return res;
}

int kr_media_version_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_VERSION_META_INFO);
  return res;
}

int kr_quvi_media_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_MEDIA_INFO_META_INFO, 1);
  return res;
}

int kr_quvi_media_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_MEDIA_INFO_META_INFO, 2);
  return res;
}

int kr_quvi_media_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_MEDIA_INFO_META_INFO, 3);
  return res;
}

int kr_quvi_media_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_QUVI_MEDIA_INFO_META_INFO);
  return res;
}

int kr_quvi_media_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_QUVI_MEDIA_INFO_META_INFO);
  return res;
}

int kr_quvi_status_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_STATUS_INFO_META_INFO, 1);
  return res;
}

int kr_quvi_status_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_STATUS_INFO_META_INFO, 2);
  return res;
}

int kr_quvi_status_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_STATUS_INFO_META_INFO, 3);
  return res;
}

int kr_quvi_status_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_QUVI_STATUS_INFO_META_INFO);
  return res;
}

int kr_quvi_status_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_QUVI_STATUS_INFO_META_INFO);
  return res;
}

int kr_quvi_params_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_PARAMS_INFO_META_INFO, 1);
  return res;
}

int kr_quvi_params_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_PARAMS_INFO_META_INFO, 2);
  return res;
}

int kr_quvi_params_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_PARAMS_INFO_META_INFO, 3);
  return res;
}

int kr_quvi_params_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_QUVI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_quvi_params_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_QUVI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_quvi_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_INFO_META_INFO, 1);
  return res;
}

int kr_quvi_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_INFO_META_INFO, 2);
  return res;
}

int kr_quvi_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_QUVI_INFO_META_INFO, 3);
  return res;
}

int kr_quvi_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_QUVI_INFO_META_INFO);
  return res;
}

int kr_quvi_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_QUVI_INFO_META_INFO);
  return res;
}

