#include "types_from_json.h"

#include <krad/util/util.h>
int kr_livestreamer_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LIVESTREAMER_STATUS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LIVESTREAMER_INFO_META_INFO);
  return res;
}

