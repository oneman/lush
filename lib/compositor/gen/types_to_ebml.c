#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_compositor_output_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_OUTPUT_INFO_META_INFO, 0);
  return res;
}

int kr_compositor_bus_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_BUS_INFO_META_INFO, 0);
  return res;
}

int kr_compositor_input_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_INPUT_INFO_META_INFO, 0);
  return res;
}

int kr_compositor_source_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_SOURCE_INFO_META_INFO, 0);
  return res;
}

int kr_compositor_overlay_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_OVERLAY_INFO_META_INFO, 0);
  return res;
}

int kr_compositor_path_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_COMPOSITOR_PATH_INFO_META_INFO, 0);
  return res;
}

