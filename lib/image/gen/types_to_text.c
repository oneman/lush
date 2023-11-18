#include "types_to_text.h"

#include <krad/util/util.h>
int kr_rect_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_RECT_META_INFO);
  return res;
}

int kr_image_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_IMAGE_INFO_META_INFO);
  return res;
}

int kr_coded_image_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_CODED_IMAGE_INFO_META_INFO);
  return res;
}

