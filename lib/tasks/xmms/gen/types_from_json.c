#include "types_from_json.h"

#include <krad/util/util.h>
int kr_xmms_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_XMMS_PARAMS_INFO_META_INFO);
  return res;
}

int kr_xmms_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_XMMS_STATUS_INFO_META_INFO);
  return res;
}

int kr_xmms_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_XMMS_INFO_META_INFO);
  return res;
}

