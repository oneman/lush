#include "types_to_json.h"

#include <krad/util/util.h>
int kr_curl_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CURL_STATUS_INFO_META_INFO);
  return res;
}

int kr_curl_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CURL_PARAMS_INFO_META_INFO);
  return res;
}

int kr_curl_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CURL_INFO_META_INFO);
  return res;
}

