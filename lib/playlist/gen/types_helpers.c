#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_m3u_el_type(char *str) {
  return str_to_enum(KR_M3U_EL_TYPE_META_INFO, str);
}

char *m3u_el_type_to_str(int val) {
  return enum_to_str(KR_M3U_EL_TYPE_META_INFO, val);
}

int str_to_kr_m3u_tag_name(char *str) {
  return str_to_enum(KR_KR_M3U_TAG_NAME_META_INFO, str);
}

char *kr_m3u_tag_name_to_str(int val) {
  return enum_to_str(KR_KR_M3U_TAG_NAME_META_INFO, val);
}

int str_to_kr_m3u_tag_val_type(char *str) {
  return str_to_enum(KR_KR_M3U_TAG_VAL_TYPE_META_INFO, str);
}

char *kr_m3u_tag_val_type_to_str(int val) {
  return enum_to_str(KR_KR_M3U_TAG_VAL_TYPE_META_INFO, val);
}

int str_to_kr_m3u_tag_attr_name(char *str) {
  return str_to_enum(KR_KR_M3U_TAG_ATTR_NAME_META_INFO, str);
}

char *kr_m3u_tag_attr_name_to_str(int val) {
  return enum_to_str(KR_KR_M3U_TAG_ATTR_NAME_META_INFO, val);
}

int kr_m3u_tag_prim_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_PRIM_INFO_META_INFO, 1);
  return res;
}

int kr_m3u_tag_prim_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_PRIM_INFO_META_INFO, 2);
  return res;
}

int kr_m3u_tag_prim_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_PRIM_INFO_META_INFO, 3);
  return res;
}

int kr_m3u_tag_prim_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_M3U_TAG_PRIM_INFO_META_INFO);
  return res;
}

int kr_m3u_tag_prim_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_M3U_TAG_PRIM_INFO_META_INFO);
  return res;
}

int kr_m3u_tag_alist_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_ALIST_INFO_META_INFO, 1);
  return res;
}

int kr_m3u_tag_alist_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_ALIST_INFO_META_INFO, 2);
  return res;
}

int kr_m3u_tag_alist_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_TAG_ALIST_INFO_META_INFO, 3);
  return res;
}

int kr_m3u_tag_alist_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_M3U_TAG_ALIST_INFO_META_INFO);
  return res;
}

int kr_m3u_tag_alist_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_M3U_TAG_ALIST_INFO_META_INFO);
  return res;
}

int m3u_tag_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_TAG_INFO_META_INFO, 1);
  return res;
}

int m3u_tag_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_TAG_INFO_META_INFO, 2);
  return res;
}

int m3u_tag_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_TAG_INFO_META_INFO, 3);
  return res;
}

int m3u_tag_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_M3U_TAG_INFO_META_INFO);
  return res;
}

int m3u_tag_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_M3U_TAG_INFO_META_INFO);
  return res;
}

int m3u_el_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_EL_INFO_META_INFO, 1);
  return res;
}

int m3u_el_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_EL_INFO_META_INFO, 2);
  return res;
}

int m3u_el_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_M3U_EL_INFO_META_INFO, 3);
  return res;
}

int m3u_el_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_M3U_EL_INFO_META_INFO);
  return res;
}

int m3u_el_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_M3U_EL_INFO_META_INFO);
  return res;
}

int kr_m3u_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_INFO_META_INFO, 1);
  return res;
}

int kr_m3u_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_INFO_META_INFO, 2);
  return res;
}

int kr_m3u_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_M3U_INFO_META_INFO, 3);
  return res;
}

int kr_m3u_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_M3U_INFO_META_INFO);
  return res;
}

int kr_m3u_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_M3U_INFO_META_INFO);
  return res;
}

