#include "types_from_json.h"

#include <krad/util/util.h>
int kr_transmission_cli_status_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TRANSMISSION_CLI_STATUS_INFO_META_INFO);
  return res;
}

int kr_transmission_cli_params_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TRANSMISSION_CLI_PARAMS_INFO_META_INFO);
  return res;
}

int kr_transmission_cli_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_TRANSMISSION_CLI_INFO_META_INFO);
  return res;
}

