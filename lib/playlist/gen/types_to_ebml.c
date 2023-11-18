#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_m3u_tag_prim_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_M3U_TAG_PRIM_INFO_META_INFO, 0);
  return res;
}

int kr_m3u_tag_alist_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_M3U_TAG_ALIST_INFO_META_INFO, 0);
  return res;
}

int m3u_tag_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_M3U_TAG_INFO_META_INFO, 0);
  return res;
}

int m3u_el_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_M3U_EL_INFO_META_INFO, 0);
  return res;
}

int kr_m3u_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_M3U_INFO_META_INFO, 0);
  return res;
}

