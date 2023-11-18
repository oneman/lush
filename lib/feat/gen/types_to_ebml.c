#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_feature_aspect_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_FEATURE_ASPECT_META_INFO, 0);
  return res;
}

int kr_feature_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_FEATURE_META_INFO, 0);
  return res;
}

