#include "vector_types_from_json.h"

#include <krad/util/util.h>
int kr_vector_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_VECTOR_INFO_META_INFO);
  return res;
}

