#include "types_to_text.h"

#include <krad/util/util.h>
int kr_fail_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_FAIL_INFO_META_INFO);
  return res;
}

int kr_task_status_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TASK_STATUS_META_INFO);
  return res;
}

int kr_task_params_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TASK_PARAMS_INFO_META_INFO);
  return res;
}

int kr_task_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TASK_INFO_META_INFO);
  return res;
}

