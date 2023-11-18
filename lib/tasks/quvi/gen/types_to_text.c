#include "types_to_text.h"

#include <krad/util/util.h>
int kr_media_version_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_MEDIA_VERSION_META_INFO);
  return res;
}

int kr_quvi_media_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_QUVI_MEDIA_INFO_META_INFO);
  return res;
}

int kr_quvi_status_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_QUVI_STATUS_INFO_META_INFO);
  return res;
}

int kr_quvi_params_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_QUVI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_quvi_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_QUVI_INFO_META_INFO);
  return res;
}

