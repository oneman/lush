#include "media_types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_media_source_format(char *str) {
  return str_to_enum(KR_KR_MEDIA_SOURCE_FORMAT_META_INFO, str);
}

char *kr_media_source_format_to_str(int val) {
  return enum_to_str(KR_KR_MEDIA_SOURCE_FORMAT_META_INFO, val);
}

int kr_media_source_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_SOURCE_INFO_META_INFO, 1);
  return res;
}

int kr_media_source_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_SOURCE_INFO_META_INFO, 2);
  return res;
}

int kr_media_source_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_SOURCE_INFO_META_INFO, 3);
  return res;
}

int kr_media_source_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_SOURCE_INFO_META_INFO);
  return res;
}

int kr_media_source_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_SOURCE_INFO_META_INFO);
  return res;
}

int str_to_kr_media_packet_type(char *str) {
  return str_to_enum(KR_KR_MEDIA_PACKET_TYPE_META_INFO, str);
}

char *kr_media_packet_type_to_str(int val) {
  return enum_to_str(KR_KR_MEDIA_PACKET_TYPE_META_INFO, val);
}

int kr_media_packet_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_PACKET_META_INFO, 1);
  return res;
}

int kr_media_packet_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_PACKET_META_INFO, 2);
  return res;
}

int kr_media_packet_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MEDIA_PACKET_META_INFO, 3);
  return res;
}

int kr_media_packet_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MEDIA_PACKET_META_INFO);
  return res;
}

int kr_media_packet_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MEDIA_PACKET_META_INFO);
  return res;
}

