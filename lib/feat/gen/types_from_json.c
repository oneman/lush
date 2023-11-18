#include "types_from_json.h"

#include <krad/util/util.h>
int kr_feature_aspect_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_FEATURE_ASPECT_META_INFO);
  return res;
}

int kr_feature_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_FEATURE_META_INFO);
  return res;
}

