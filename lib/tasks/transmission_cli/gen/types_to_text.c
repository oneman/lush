#include "types_to_text.h"

#include <krad/util/util.h>
int kr_transmission_cli_status_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TRANSMISSION_CLI_STATUS_INFO_META_INFO);
  return res;
}

int kr_transmission_cli_params_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TRANSMISSION_CLI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_transmission_cli_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TRANSMISSION_CLI_INFO_META_INFO);
  return res;
}

