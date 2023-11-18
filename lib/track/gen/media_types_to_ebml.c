#include "media_types_to_ebml.h"

#include <krad/util/util.h>
int kr_media_source_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MEDIA_SOURCE_INFO_META_INFO, 0);
  return res;
}

int kr_media_packet_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MEDIA_PACKET_META_INFO, 0);
  return res;
}

