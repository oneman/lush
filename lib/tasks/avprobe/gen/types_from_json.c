#include "types_from_json.h"

#include <krad/util/util.h>
int kr_avprobe_format_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_FORMAT_INFO_META_INFO);
  return res;
}

int kr_avprobe_stream_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_STREAM_INFO_META_INFO);
  return res;
}

int kr_avprobe_media_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_MEDIA_INFO_META_INFO);
  return res;
}

int kr_avprobe_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avprobe_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_STATUS_INFO_META_INFO);
  return res;
}

int kr_avprobe_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_AVPROBE_INFO_META_INFO);
  return res;
}

