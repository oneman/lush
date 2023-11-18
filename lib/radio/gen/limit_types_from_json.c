#include "limit_types_from_json.h"

#include <krad/util/util.h>
int kr_limit_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LIMIT_INFO_META_INFO);
  return res;
}

int kr_limits_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_LIMITS_INFO_META_INFO);
  return res;
}

