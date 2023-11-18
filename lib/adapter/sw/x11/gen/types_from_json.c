#include "types_from_json.h"

#include <krad/util/util.h>
int kr_x11_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_X11_INFO_META_INFO);
  return res;
}

int kr_x11_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_X11_PATH_INFO_META_INFO);
  return res;
}

