#include "types_from_ebml.h"

#include <krad/util/util.h>
int kr_jack_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_JACK_INFO_META_INFO, 1);
  return res;
}

int kr_jack_path_info_unpack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_JACK_PATH_INFO_META_INFO, 1);
  return res;
}

