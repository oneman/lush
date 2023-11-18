#include "types_from_json.h"

#include <krad/util/util.h>
int kr_media_file_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_FILE_INFO_META_INFO);
  return res;
}

int kr_media_metadata_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_METADATA_INFO_META_INFO);
  return res;
}

int kr_media_url_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_URL_INFO_META_INFO);
  return res;
}

int kr_media_item_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_ITEM_INFO_META_INFO);
  return res;
}

int kr_media_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_MEDIA_INFO_META_INFO);
  return res;
}

