#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_pixel_fmt(char *str) {
  return str_to_enum(KR_KR_PIXEL_FMT_META_INFO, str);
}

char *kr_pixel_fmt_to_str(int val) {
  return enum_to_str(KR_KR_PIXEL_FMT_META_INFO, val);
}

int str_to_kr_image_type(char *str) {
  return str_to_enum(KR_KR_IMAGE_TYPE_META_INFO, str);
}

char *kr_image_type_to_str(int val) {
  return enum_to_str(KR_KR_IMAGE_TYPE_META_INFO, val);
}

int kr_rect_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECT_META_INFO, 1);
  return res;
}

int kr_rect_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECT_META_INFO, 2);
  return res;
}

int kr_rect_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECT_META_INFO, 3);
  return res;
}

int kr_rect_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RECT_META_INFO);
  return res;
}

int kr_rect_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RECT_META_INFO);
  return res;
}

int kr_image_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_INFO_META_INFO, 1);
  return res;
}

int kr_image_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_INFO_META_INFO, 2);
  return res;
}

int kr_image_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_IMAGE_INFO_META_INFO, 3);
  return res;
}

int kr_image_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_IMAGE_INFO_META_INFO);
  return res;
}

int kr_image_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_IMAGE_INFO_META_INFO);
  return res;
}

int str_to_kr_coded_image_type(char *str) {
  return str_to_enum(KR_KR_CODED_IMAGE_TYPE_META_INFO, str);
}

char *kr_coded_image_type_to_str(int val) {
  return enum_to_str(KR_KR_CODED_IMAGE_TYPE_META_INFO, val);
}

int kr_coded_image_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODED_IMAGE_INFO_META_INFO, 1);
  return res;
}

int kr_coded_image_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODED_IMAGE_INFO_META_INFO, 2);
  return res;
}

int kr_coded_image_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODED_IMAGE_INFO_META_INFO, 3);
  return res;
}

int kr_coded_image_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CODED_IMAGE_INFO_META_INFO);
  return res;
}

int kr_coded_image_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CODED_IMAGE_INFO_META_INFO);
  return res;
}

