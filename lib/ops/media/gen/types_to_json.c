#include "types_to_json.h"

#include <krad/util/util.h>
int kr_media_file_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_FILE_INFO_META_INFO);
  return res;
}

int kr_media_metadata_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_METADATA_INFO_META_INFO);
  return res;
}

int kr_media_url_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_URL_INFO_META_INFO);
  return res;
}

int kr_media_item_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_ITEM_INFO_META_INFO);
  return res;
}

int kr_media_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MEDIA_INFO_META_INFO);
  return res;
}

