#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_rect_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_RECT_META_INFO, 0);
  return res;
}

int kr_image_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_IMAGE_INFO_META_INFO, 0);
  return res;
}

int kr_coded_image_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_CODED_IMAGE_INFO_META_INFO, 0);
  return res;
}

