#include "types_to_text.h"

#include <krad/util/util.h>
int kr_pos_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_POS_META_INFO);
  return res;
}

int kr_perspective_view_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_PERSPECTIVE_VIEW_META_INFO);
  return res;
}

