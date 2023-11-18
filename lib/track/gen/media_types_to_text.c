#include "media_types_to_text.h"

#include <krad/util/util.h>
int kr_media_source_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_MEDIA_SOURCE_INFO_META_INFO);
  return res;
}

int kr_media_packet_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_MEDIA_PACKET_META_INFO);
  return res;
}

