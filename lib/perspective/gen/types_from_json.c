#include "types_from_json.h"

#include <krad/util/util.h>
int kr_pos_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_POS_META_INFO);
  return res;
}

int kr_perspective_view_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_PERSPECTIVE_VIEW_META_INFO);
  return res;
}

