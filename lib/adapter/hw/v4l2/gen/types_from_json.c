#include "types_from_json.h"

#include <krad/util/util.h>
int kr_v4l2_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_V4L2_PATH_INFO_META_INFO);
  return res;
}

int kr_v4l2_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_V4L2_INFO_META_INFO);
  return res;
}

