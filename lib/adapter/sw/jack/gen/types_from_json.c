#include "types_from_json.h"

#include <krad/util/util.h>
int kr_jack_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_JACK_INFO_META_INFO);
  return res;
}

int kr_jack_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_JACK_PATH_INFO_META_INFO);
  return res;
}

