#include "types_from_json.h"

#include <krad/util/util.h>
int kr_fail_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_FAIL_INFO_META_INFO);
  return res;
}

int kr_task_status_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TASK_STATUS_META_INFO);
  return res;
}

int kr_task_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TASK_PARAMS_INFO_META_INFO);
  return res;
}

int kr_task_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TASK_INFO_META_INFO);
  return res;
}

