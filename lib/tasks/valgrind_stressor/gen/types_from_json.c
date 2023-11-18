#include "types_from_json.h"

#include <krad/util/util.h>
int kr_valgrind_stressor_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_VALGRIND_STRESSOR_STATUS_INFO_META_INFO);
  return res;
}

int kr_valgrind_stressor_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_VALGRIND_STRESSOR_PARAMS_INFO_META_INFO);
  return res;
}

int kr_valgrind_stressor_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_VALGRIND_STRESSOR_INFO_META_INFO);
  return res;
}

