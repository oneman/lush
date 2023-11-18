#include "types_to_json.h"

#include <krad/util/util.h>
int kr_decklink_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_DECKLINK_INFO_META_INFO);
  return res;
}

int kr_decklink_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_DECKLINK_PATH_INFO_META_INFO);
  return res;
}

