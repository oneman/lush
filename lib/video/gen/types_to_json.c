#include "types_to_json.h"

#include <krad/util/util.h>
int kr_fps_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_FPS_INFO_META_INFO);
  return res;
}

