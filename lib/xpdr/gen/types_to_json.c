#include "types_to_json.h"

#include <krad/util/util.h>
int kr_xpdr_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_XPDR_PATH_INFO_META_INFO);
  return res;
}

