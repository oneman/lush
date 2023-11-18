#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_file2_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_FILE2_INFO_META_INFO, 0);
  return res;
}

