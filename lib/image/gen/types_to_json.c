#include "types_to_json.h"

#include <krad/util/util.h>
int kr_rect_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RECT_META_INFO);
  return res;
}

int kr_image_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_IMAGE_INFO_META_INFO);
  return res;
}

int kr_coded_image_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CODED_IMAGE_INFO_META_INFO);
  return res;
}

