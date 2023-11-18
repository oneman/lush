#include "types_from_json.h"

#include <krad/util/util.h>
int kr_compositor_output_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_bus_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_BUS_INFO_META_INFO);
  return res;
}

int kr_compositor_input_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_source_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_compositor_overlay_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO);
  return res;
}

int kr_compositor_path_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_COMPOSITOR_PATH_INFO_META_INFO);
  return res;
}

