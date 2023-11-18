#include "types_to_json.h"

#include <krad/util/util.h>
int kr_m3u_tag_prim_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_M3U_TAG_PRIM_INFO_META_INFO);
  return res;
}

int kr_m3u_tag_alist_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_M3U_TAG_ALIST_INFO_META_INFO);
  return res;
}

int m3u_tag_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_M3U_TAG_INFO_META_INFO);
  return res;
}

int m3u_el_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_M3U_EL_INFO_META_INFO);
  return res;
}

int kr_m3u_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_M3U_INFO_META_INFO);
  return res;
}

