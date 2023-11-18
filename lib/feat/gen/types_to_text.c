#include "types_to_text.h"

#include <krad/util/util.h>
int kr_feature_aspect_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_FEATURE_ASPECT_META_INFO);
  return res;
}

int kr_feature_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_FEATURE_META_INFO);
  return res;
}

