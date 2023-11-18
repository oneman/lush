#include "media_types_from_json.h"

#include <krad/util/util.h>
int kr_media_source_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_SOURCE_INFO_META_INFO);
  return res;
}

int kr_media_packet_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_PACKET_META_INFO);
  return res;
}

