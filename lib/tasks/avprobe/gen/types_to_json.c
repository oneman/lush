#include "types_to_json.h"

#include <krad/util/util.h>
int kr_avprobe_format_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_FORMAT_INFO_META_INFO);
  return res;
}

int kr_avprobe_stream_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_STREAM_INFO_META_INFO);
  return res;
}

int kr_avprobe_media_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_MEDIA_INFO_META_INFO);
  return res;
}

int kr_avprobe_params_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_PARAMS_INFO_META_INFO);
  return res;
}

int kr_avprobe_status_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_STATUS_INFO_META_INFO);
  return res;
}

int kr_avprobe_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AVPROBE_INFO_META_INFO);
  return res;
}

