#include "types_to_json.h"

#include <krad/util/util.h>
int kr_mts_packet_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MTS_PACKET_INFO_META_INFO);
  return res;
}

int kr_mts_stream_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MTS_STREAM_INFO_META_INFO);
  return res;
}

int kr_mts_program_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MTS_PROGRAM_INFO_META_INFO);
  return res;
}

int kr_mts_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MTS_INFO_META_INFO);
  return res;
}

