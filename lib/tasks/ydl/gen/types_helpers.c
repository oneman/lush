#include "types_helpers.h"

#include <krad/util/util.h>
int kr_ydl_format_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_FORMAT_INFO_META_INFO, 1);
  return res;
}

int kr_ydl_format_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_FORMAT_INFO_META_INFO, 2);
  return res;
}

int kr_ydl_format_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_FORMAT_INFO_META_INFO, 3);
  return res;
}

int kr_ydl_format_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_YDL_FORMAT_INFO_META_INFO);
  return res;
}

int kr_ydl_format_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_YDL_FORMAT_INFO_META_INFO);
  return res;
}

int kr_ydl_video_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_VIDEO_INFO_META_INFO, 1);
  return res;
}

int kr_ydl_video_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_VIDEO_INFO_META_INFO, 2);
  return res;
}

int kr_ydl_video_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_VIDEO_INFO_META_INFO, 3);
  return res;
}

int kr_ydl_video_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_YDL_VIDEO_INFO_META_INFO);
  return res;
}

int kr_ydl_video_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_YDL_VIDEO_INFO_META_INFO);
  return res;
}

int kr_ydl_params_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_PARAMS_INFO_META_INFO, 1);
  return res;
}

int kr_ydl_params_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_PARAMS_INFO_META_INFO, 2);
  return res;
}

int kr_ydl_params_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_PARAMS_INFO_META_INFO, 3);
  return res;
}

int kr_ydl_params_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_YDL_PARAMS_INFO_META_INFO);
  return res;
}

int kr_ydl_params_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_YDL_PARAMS_INFO_META_INFO);
  return res;
}

int kr_ydl_status_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_STATUS_INFO_META_INFO, 1);
  return res;
}

int kr_ydl_status_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_STATUS_INFO_META_INFO, 2);
  return res;
}

int kr_ydl_status_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_STATUS_INFO_META_INFO, 3);
  return res;
}

int kr_ydl_status_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_YDL_STATUS_INFO_META_INFO);
  return res;
}

int kr_ydl_status_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_YDL_STATUS_INFO_META_INFO);
  return res;
}

int kr_ydl_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_INFO_META_INFO, 1);
  return res;
}

int kr_ydl_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_INFO_META_INFO, 2);
  return res;
}

int kr_ydl_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_YDL_INFO_META_INFO, 3);
  return res;
}

int kr_ydl_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_YDL_INFO_META_INFO);
  return res;
}

int kr_ydl_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_YDL_INFO_META_INFO);
  return res;
}

