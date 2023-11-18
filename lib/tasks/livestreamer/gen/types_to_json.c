#include "types_to_json.h"

#include <krad/util/util.h>
int kr_livestreamer_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_LIVESTREAMER_STATUS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_LIVESTREAMER_INFO_META_INFO);
  return res;
}

