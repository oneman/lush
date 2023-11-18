#include "types_to_text.h"

#include <krad/util/util.h>
int kr_livestreamer_params_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_LIVESTREAMER_PARAMS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_status_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_LIVESTREAMER_STATUS_INFO_META_INFO);
  return res;
}

int kr_livestreamer_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_LIVESTREAMER_INFO_META_INFO);
  return res;
}

