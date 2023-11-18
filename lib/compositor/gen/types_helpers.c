#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_compositor_path_type(char *str) {
  return str_to_enum(KR_KR_COMPOSITOR_PATH_TYPE_META_INFO, str);
}

char *kr_compositor_path_type_to_str(int val) {
  return enum_to_str(KR_KR_COMPOSITOR_PATH_TYPE_META_INFO, val);
}

int str_to_kr_compositor_overlay_type(char *str) {
  return str_to_enum(KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO, str);
}

char *kr_compositor_overlay_type_to_str(int val) {
  return enum_to_str(KR_KR_COMPOSITOR_OVERLAY_TYPE_META_INFO, val);
}

int kr_compositor_output_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_output_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_output_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_output_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_output_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_bus_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_BUS_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_bus_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_BUS_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_bus_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_BUS_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_bus_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_BUS_INFO_META_INFO);
  return res;
}

int kr_compositor_bus_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_BUS_INFO_META_INFO);
  return res;
}

int kr_compositor_input_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_input_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_input_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_input_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_input_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_source_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_source_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_source_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_source_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_compositor_source_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_compositor_overlay_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_overlay_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_overlay_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_overlay_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO);
  return res;
}

int kr_compositor_overlay_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO);
  return res;
}

int kr_compositor_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_compositor_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_compositor_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_COMPOSITOR_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_compositor_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_COMPOSITOR_PATH_INFO_META_INFO);
  return res;
}

int kr_compositor_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_COMPOSITOR_PATH_INFO_META_INFO);
  return res;
}

