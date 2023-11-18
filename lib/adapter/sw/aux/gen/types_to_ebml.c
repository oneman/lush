#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_aux_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AUX_INFO_META_INFO, 0);
  return res;
}

int kr_aux_path_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AUX_PATH_INFO_META_INFO, 0);
  return res;
}

