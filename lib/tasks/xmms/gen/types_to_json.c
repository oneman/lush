#include "types_to_json.h"

#include <krad/util/util.h>
int kr_xmms_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_XMMS_PARAMS_INFO_META_INFO);
  return res;
}

int kr_xmms_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_XMMS_STATUS_INFO_META_INFO);
  return res;
}

int kr_xmms_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_XMMS_INFO_META_INFO);
  return res;
}

