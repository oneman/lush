#include "types_from_json.h"

#include <krad/util/util.h>
int kr_decklink_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_DECKLINK_INFO_META_INFO);
  return res;
}

int kr_decklink_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_DECKLINK_PATH_INFO_META_INFO);
  return res;
}

