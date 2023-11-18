#include "types_to_json.h"

#include <krad/util/util.h>
int kr_feature_aspect_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_FEATURE_ASPECT_META_INFO);
  return res;
}

int kr_feature_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_FEATURE_META_INFO);
  return res;
}

