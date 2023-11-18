#include "types_from_ebml.h"

#include <krad/util/util.h>
int kr_mts_packet_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MTS_PACKET_INFO_META_INFO, 1);
  return res;
}

int kr_mts_stream_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MTS_STREAM_INFO_META_INFO, 1);
  return res;
}

int kr_mts_program_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MTS_PROGRAM_INFO_META_INFO, 1);
  return res;
}

int kr_mts_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MTS_INFO_META_INFO, 1);
  return res;
}

