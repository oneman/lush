#include "types_from_json.h"

#include <krad/util/util.h>
int kr_wayland_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_WAYLAND_INFO_META_INFO);
  return res;
}

int kr_wayland_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_WAYLAND_PATH_INFO_META_INFO);
  return res;
}

