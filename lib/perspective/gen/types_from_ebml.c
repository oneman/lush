#include "types_from_ebml.h"

#include <krad/util/util.h>
int kr_pos_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_POS_META_INFO, 1);
  return res;
}

int kr_perspective_view_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_PERSPECTIVE_VIEW_META_INFO, 1);
  return res;
}

