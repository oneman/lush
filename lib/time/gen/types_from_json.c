#include "types_from_json.h"

#include <krad/util/util.h>
int kr_timestamp_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TIMESTAMP_INFO_META_INFO);
  return res;
}

