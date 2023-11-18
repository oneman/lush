#include "types_to_json.h"

#include <krad/util/util.h>
int kr_pos_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_POS_META_INFO);
  return res;
}

int kr_perspective_view_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_PERSPECTIVE_VIEW_META_INFO);
  return res;
}

