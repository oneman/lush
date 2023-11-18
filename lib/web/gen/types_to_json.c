#include "types_to_json.h"

#include <krad/util/util.h>
int kr_radio_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RADIO_INFO_META_INFO);
  return res;
}

int kr_web_server_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_SERVER_INFO_META_INFO);
  return res;
}

int kr_web_server_setup_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_remote_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_REMOTE_INFO_META_INFO);
  return res;
}

int kr_web_socket_io_state_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_SOCKET_IO_STATE_META_INFO);
  return res;
}

int kr_web_socket_state_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_SOCKET_STATE_META_INFO);
  return res;
}

int kr_web_http_io_state_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_HTTP_IO_STATE_META_INFO);
  return res;
}

int kr_web_http_state_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_HTTP_STATE_META_INFO);
  return res;
}

int kr_web_client_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_CLIENT_INFO_META_INFO);
  return res;
}

int kr_web_client_setup_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_PATH_INFO_META_INFO);
  return res;
}

int kr_web_path_setup_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_WEB_PATH_SETUP_INFO_META_INFO);
  return res;
}

int kr_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_PATH_INFO_META_INFO);
  return res;
}

int kr_path_list_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_PATH_LIST_META_INFO);
  return res;
}

