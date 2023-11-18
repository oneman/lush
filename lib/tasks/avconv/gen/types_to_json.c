#include "types_to_json.h"

#include <krad/util/util.h>
int kr_avconv_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVCONV_STATUS_INFO_META_INFO);
  return res;
}

int kr_avconv_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVCONV_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avconv_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVCONV_INFO_META_INFO);
  return res;
}

