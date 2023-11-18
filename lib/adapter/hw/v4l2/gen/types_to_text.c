#include "types_to_text.h"

#include <krad/util/util.h>
int kr_v4l2_path_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_V4L2_PATH_INFO_META_INFO);
  return res;
}

int kr_v4l2_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_V4L2_INFO_META_INFO);
  return res;
}

