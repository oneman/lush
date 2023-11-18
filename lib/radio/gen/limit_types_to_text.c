#include "limit_types_to_text.h"

#include <krad/util/util.h>
int kr_limit_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_LIMIT_INFO_META_INFO);
  return res;
}

int kr_limits_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_LIMITS_INFO_META_INFO);
  return res;
}

