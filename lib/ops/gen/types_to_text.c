#include "types_to_text.h"

#include <krad/util/util.h>
int kr_ops_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_OPS_INFO_META_INFO);
  return res;
}

