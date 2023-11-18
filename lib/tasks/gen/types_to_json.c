#include "types_to_json.h"

#include <krad/util/util.h>
int kr_fail_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_FAIL_INFO_META_INFO);
  return res;
}

int kr_task_status_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TASK_STATUS_META_INFO);
  return res;
}

int kr_task_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TASK_PARAMS_INFO_META_INFO);
  return res;
}

int kr_task_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TASK_INFO_META_INFO);
  return res;
}

