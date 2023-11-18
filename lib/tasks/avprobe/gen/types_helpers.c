#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_avprobe_codec_type(char *str) {
  return str_to_enum(KR_KR_AVPROBE_CODEC_TYPE_META_INFO, str);
}

char *kr_avprobe_codec_type_to_str(int val) {
  return enum_to_str(KR_KR_AVPROBE_CODEC_TYPE_META_INFO, val);
}

int kr_avprobe_format_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_FORMAT_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_format_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_FORMAT_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_format_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_FORMAT_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_format_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_FORMAT_INFO_META_INFO);
  return res;
}

int kr_avprobe_format_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_FORMAT_INFO_META_INFO);
  return res;
}

int kr_avprobe_stream_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STREAM_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_stream_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STREAM_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_stream_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STREAM_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_stream_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_STREAM_INFO_META_INFO);
  return res;
}

int kr_avprobe_stream_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_STREAM_INFO_META_INFO);
  return res;
}

int kr_avprobe_media_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_MEDIA_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_media_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_MEDIA_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_media_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_MEDIA_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_media_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_MEDIA_INFO_META_INFO);
  return res;
}

int kr_avprobe_media_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_MEDIA_INFO_META_INFO);
  return res;
}

int kr_avprobe_params_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_PARAMS_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_params_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_PARAMS_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_params_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_PARAMS_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_params_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avprobe_params_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avprobe_status_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STATUS_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_status_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STATUS_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_status_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_STATUS_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_status_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_STATUS_INFO_META_INFO);
  return res;
}

int kr_avprobe_status_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_STATUS_INFO_META_INFO);
  return res;
}

int kr_avprobe_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_INFO_META_INFO, 1);
  return res;
}

int kr_avprobe_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_INFO_META_INFO, 2);
  return res;
}

int kr_avprobe_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AVPROBE_INFO_META_INFO, 3);
  return res;
}

int kr_avprobe_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AVPROBE_INFO_META_INFO);
  return res;
}

int kr_avprobe_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AVPROBE_INFO_META_INFO);
  return res;
}

