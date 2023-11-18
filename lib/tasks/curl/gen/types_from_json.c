#include "types_from_json.h"

#include <krad/util/util.h>
int kr_curl_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_CURL_STATUS_INFO_META_INFO);
  return res;
}

int kr_curl_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_CURL_PARAMS_INFO_META_INFO);
  return res;
}

int kr_curl_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_CURL_INFO_META_INFO);
  return res;
}

