#include "types_from_json.h"

#include <krad/util/util.h>
int kr_avconv_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVCONV_STATUS_INFO_META_INFO);
  return res;
}

int kr_avconv_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVCONV_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avconv_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVCONV_INFO_META_INFO);
  return res;
}

