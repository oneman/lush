#include "types_to_text.h"

#include <krad/util/util.h>
int kr_decklink_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DECKLINK_INFO_META_INFO);
  return res;
}

int kr_decklink_path_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DECKLINK_PATH_INFO_META_INFO);
  return res;
}

