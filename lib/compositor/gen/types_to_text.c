#include "types_to_text.h"

#include <krad/util/util.h>
int kr_compositor_output_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_bus_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_BUS_INFO_META_INFO);
  return res;
}

int kr_compositor_input_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO);
  return res;
}

int kr_compositor_source_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_compositor_overlay_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO);
  return res;
}

int kr_compositor_path_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_COMPOSITOR_PATH_INFO_META_INFO);
  return res;
}

