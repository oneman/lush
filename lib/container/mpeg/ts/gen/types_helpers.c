#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_mts_stream_type(char *str) {
  return str_to_enum(KR_KR_MTS_STREAM_TYPE_META_INFO, str);
}

char *kr_mts_stream_type_to_str(int val) {
  return enum_to_str(KR_KR_MTS_STREAM_TYPE_META_INFO, val);
}

int kr_mts_packet_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PACKET_INFO_META_INFO, 1);
  return res;
}

int kr_mts_packet_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PACKET_INFO_META_INFO, 2);
  return res;
}

int kr_mts_packet_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PACKET_INFO_META_INFO, 3);
  return res;
}

int kr_mts_packet_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MTS_PACKET_INFO_META_INFO);
  return res;
}

int kr_mts_packet_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MTS_PACKET_INFO_META_INFO);
  return res;
}

int kr_mts_stream_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_STREAM_INFO_META_INFO, 1);
  return res;
}

int kr_mts_stream_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_STREAM_INFO_META_INFO, 2);
  return res;
}

int kr_mts_stream_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_STREAM_INFO_META_INFO, 3);
  return res;
}

int kr_mts_stream_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MTS_STREAM_INFO_META_INFO);
  return res;
}

int kr_mts_stream_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MTS_STREAM_INFO_META_INFO);
  return res;
}

int kr_mts_program_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PROGRAM_INFO_META_INFO, 1);
  return res;
}

int kr_mts_program_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PROGRAM_INFO_META_INFO, 2);
  return res;
}

int kr_mts_program_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_PROGRAM_INFO_META_INFO, 3);
  return res;
}

int kr_mts_program_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MTS_PROGRAM_INFO_META_INFO);
  return res;
}

int kr_mts_program_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MTS_PROGRAM_INFO_META_INFO);
  return res;
}

int kr_mts_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_INFO_META_INFO, 1);
  return res;
}

int kr_mts_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_INFO_META_INFO, 2);
  return res;
}

int kr_mts_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MTS_INFO_META_INFO, 3);
  return res;
}

int kr_mts_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MTS_INFO_META_INFO);
  return res;
}

int kr_mts_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MTS_INFO_META_INFO);
  return res;
}

