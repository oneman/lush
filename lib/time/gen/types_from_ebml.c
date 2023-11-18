#include "types_from_ebml.h"

#include <krad/util/util.h>
int kr_timestamp_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_TIMESTAMP_INFO_META_INFO, 1);
  return res;
}

