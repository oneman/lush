#include "types_from_json.h"

#include <krad/util/util.h>
int kr_media_version_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_VERSION_META_INFO);
  return res;
}

int kr_quvi_media_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_QUVI_MEDIA_INFO_META_INFO);
  return res;
}

int kr_quvi_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_QUVI_STATUS_INFO_META_INFO);
  return res;
}

int kr_quvi_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_QUVI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_quvi_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_QUVI_INFO_META_INFO);
  return res;
}

