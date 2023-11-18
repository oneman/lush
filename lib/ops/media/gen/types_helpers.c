#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_media_event_type(char *str) {
  return str_to_enum(KR_KR_MEDIA_EVENT_TYPE_META_INFO, str);
}

char *kr_media_event_type_to_str(int val) {
  return enum_to_str(KR_KR_MEDIA_EVENT_TYPE_META_INFO, val);
}

int str_to_kr_media_item_type(char *str) {
  return str_to_enum(KR_KR_MEDIA_ITEM_TYPE_META_INFO, str);
}

char *kr_media_item_type_to_str(int val) {
  return enum_to_str(KR_KR_MEDIA_ITEM_TYPE_META_INFO, val);
}

int kr_media_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_media_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_media_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_media_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_FILE_INFO_META_INFO);
  return res;
}

int kr_media_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_FILE_INFO_META_INFO);
  return res;
}

int kr_media_metadata_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_METADATA_INFO_META_INFO, 1);
  return res;
}

int kr_media_metadata_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_METADATA_INFO_META_INFO, 2);
  return res;
}

int kr_media_metadata_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_METADATA_INFO_META_INFO, 3);
  return res;
}

int kr_media_metadata_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_METADATA_INFO_META_INFO);
  return res;
}

int kr_media_metadata_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_METADATA_INFO_META_INFO);
  return res;
}

int kr_media_url_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_URL_INFO_META_INFO, 1);
  return res;
}

int kr_media_url_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_URL_INFO_META_INFO, 2);
  return res;
}

int kr_media_url_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_URL_INFO_META_INFO, 3);
  return res;
}

int kr_media_url_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_URL_INFO_META_INFO);
  return res;
}

int kr_media_url_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_URL_INFO_META_INFO);
  return res;
}

int kr_media_item_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_ITEM_INFO_META_INFO, 1);
  return res;
}

int kr_media_item_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_ITEM_INFO_META_INFO, 2);
  return res;
}

int kr_media_item_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_ITEM_INFO_META_INFO, 3);
  return res;
}

int kr_media_item_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_ITEM_INFO_META_INFO);
  return res;
}

int kr_media_item_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_ITEM_INFO_META_INFO);
  return res;
}

int kr_media_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_INFO_META_INFO, 1);
  return res;
}

int kr_media_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_INFO_META_INFO, 2);
  return res;
}

int kr_media_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_INFO_META_INFO, 3);
  return res;
}

int kr_media_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_INFO_META_INFO);
  return res;
}

int kr_media_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_INFO_META_INFO);
  return res;
}

