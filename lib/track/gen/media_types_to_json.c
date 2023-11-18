#include "media_types_to_json.h"

#include <krad/util/util.h>
int kr_media_source_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_SOURCE_INFO_META_INFO);
  return res;
}

int kr_media_packet_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_PACKET_META_INFO);
  return res;
}

