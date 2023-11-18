#include "types_to_json.h"

#include <krad/util/util.h>
int kr_valgrind_stressor_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_VALGRIND_STRESSOR_STATUS_INFO_META_INFO);
  return res;
}

int kr_valgrind_stressor_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_VALGRIND_STRESSOR_PARAMS_INFO_META_INFO);
  return res;
}

int kr_valgrind_stressor_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_VALGRIND_STRESSOR_INFO_META_INFO);
  return res;
}

