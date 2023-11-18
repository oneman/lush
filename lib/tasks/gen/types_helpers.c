#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_task_type(char *str) {
  return str_to_enum(KR_KR_TASK_TYPE_META_INFO, str);
}

char *kr_task_type_to_str(int val) {
  return enum_to_str(KR_KR_TASK_TYPE_META_INFO, val);
}

int str_to_kr_task_state(char *str) {
  return str_to_enum(KR_KR_TASK_STATE_META_INFO, str);
}

char *kr_task_state_to_str(int val) {
  return enum_to_str(KR_KR_TASK_STATE_META_INFO, val);
}

int str_to_kr_task_event_type(char *str) {
  return str_to_enum(KR_KR_TASK_EVENT_TYPE_META_INFO, str);
}

char *kr_task_event_type_to_str(int val) {
  return enum_to_str(KR_KR_TASK_EVENT_TYPE_META_INFO, val);
}

int kr_fail_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FAIL_INFO_META_INFO, 1);
  return res;
}

int kr_fail_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FAIL_INFO_META_INFO, 2);
  return res;
}

int kr_fail_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FAIL_INFO_META_INFO, 3);
  return res;
}

int kr_fail_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_FAIL_INFO_META_INFO);
  return res;
}

int kr_fail_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_FAIL_INFO_META_INFO);
  return res;
}

int kr_task_status_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_STATUS_META_INFO, 1);
  return res;
}

int kr_task_status_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_STATUS_META_INFO, 2);
  return res;
}

int kr_task_status_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_STATUS_META_INFO, 3);
  return res;
}

int kr_task_status_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TASK_STATUS_META_INFO);
  return res;
}

int kr_task_status_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TASK_STATUS_META_INFO);
  return res;
}

int kr_task_params_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_PARAMS_INFO_META_INFO, 1);
  return res;
}

int kr_task_params_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_PARAMS_INFO_META_INFO, 2);
  return res;
}

int kr_task_params_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_PARAMS_INFO_META_INFO, 3);
  return res;
}

int kr_task_params_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TASK_PARAMS_INFO_META_INFO);
  return res;
}

int kr_task_params_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TASK_PARAMS_INFO_META_INFO);
  return res;
}

int kr_task_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_INFO_META_INFO, 1);
  return res;
}

int kr_task_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_INFO_META_INFO, 2);
  return res;
}

int kr_task_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TASK_INFO_META_INFO, 3);
  return res;
}

int kr_task_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TASK_INFO_META_INFO);
  return res;
}

int kr_task_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TASK_INFO_META_INFO);
  return res;
}

