#include "limit_types_to_ebml.h"

#include <krad/util/util.h>
int kr_limit_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_LIMIT_INFO_META_INFO, 0);
  return res;
}

int kr_limits_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_LIMITS_INFO_META_INFO, 0);
  return res;
}

