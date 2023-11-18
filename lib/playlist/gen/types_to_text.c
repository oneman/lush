#include "types_to_text.h"

#include <krad/util/util.h>
int kr_m3u_tag_prim_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_M3U_TAG_PRIM_INFO_META_INFO);
  return res;
}

int kr_m3u_tag_alist_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_M3U_TAG_ALIST_INFO_META_INFO);
  return res;
}

int m3u_tag_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_M3U_TAG_INFO_META_INFO);
  return res;
}

int m3u_el_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_M3U_EL_INFO_META_INFO);
  return res;
}

int kr_m3u_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_M3U_INFO_META_INFO);
  return res;
}

