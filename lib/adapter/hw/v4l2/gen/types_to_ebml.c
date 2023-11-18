#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_v4l2_path_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_V4L2_PATH_INFO_META_INFO, 0);
  return res;
}

int kr_v4l2_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_V4L2_INFO_META_INFO, 0);
  return res;
}

