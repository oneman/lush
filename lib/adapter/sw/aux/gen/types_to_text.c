#include "types_to_text.h"

#include <krad/util/util.h>
int kr_aux_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_AUX_INFO_META_INFO);
  return res;
}

int kr_aux_path_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_AUX_PATH_INFO_META_INFO);
  return res;
}

