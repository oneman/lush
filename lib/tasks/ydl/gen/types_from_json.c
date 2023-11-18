#include "types_from_json.h"

#include <krad/util/util.h>
int kr_ydl_format_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_YDL_FORMAT_INFO_META_INFO);
  return res;
}

int kr_ydl_video_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_YDL_VIDEO_INFO_META_INFO);
  return res;
}

int kr_ydl_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_YDL_PARAMS_INFO_META_INFO);
  return res;
}

int kr_ydl_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_YDL_STATUS_INFO_META_INFO);
  return res;
}

int kr_ydl_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_YDL_INFO_META_INFO);
  return res;
}

