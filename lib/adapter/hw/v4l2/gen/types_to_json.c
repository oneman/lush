#include "types_to_json.h"

#include <krad/util/util.h>
int kr_v4l2_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_V4L2_PATH_INFO_META_INFO);
  return res;
}

int kr_v4l2_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_V4L2_INFO_META_INFO);
  return res;
}

