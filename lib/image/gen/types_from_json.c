#include "types_from_json.h"

#include <krad/util/util.h>
int kr_rect_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_RECT_META_INFO);
  return res;
}

int kr_image_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_IMAGE_INFO_META_INFO);
  return res;
}

int kr_coded_image_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_CODED_IMAGE_INFO_META_INFO);
  return res;
}

