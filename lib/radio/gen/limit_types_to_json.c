#include "limit_types_to_json.h"

#include <krad/util/util.h>
int kr_limit_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_LIMIT_INFO_META_INFO);
  return res;
}

int kr_limits_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_LIMITS_INFO_META_INFO);
  return res;
}

