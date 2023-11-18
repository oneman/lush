#include "types_to_json.h"

#include <krad/util/util.h>
int kr_wayland_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WAYLAND_INFO_META_INFO);
  return res;
}

int kr_wayland_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WAYLAND_PATH_INFO_META_INFO);
  return res;
}

