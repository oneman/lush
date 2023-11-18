#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_radio_mode(char *str) {
  return str_to_enum(KR_KR_RADIO_MODE_META_INFO, str);
}

char *kr_radio_mode_to_str(int val) {
  return enum_to_str(KR_KR_RADIO_MODE_META_INFO, val);
}

int kr_radio_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RADIO_INFO_META_INFO, 1);
  return res;
}

int kr_radio_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RADIO_INFO_META_INFO, 2);
  return res;
}

int kr_radio_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RADIO_INFO_META_INFO, 3);
  return res;
}

int kr_radio_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RADIO_INFO_META_INFO);
  return res;
}

int kr_radio_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RADIO_INFO_META_INFO);
  return res;
}

int str_to_kr_web_client_protocol(char *str) {
  return str_to_enum(KR_KR_WEB_CLIENT_PROTOCOL_META_INFO, str);
}

char *kr_web_client_protocol_to_str(int val) {
  return enum_to_str(KR_KR_WEB_CLIENT_PROTOCOL_META_INFO, val);
}

int str_to_kr_web_client_type(char *str) {
  return str_to_enum(KR_KR_WEB_CLIENT_TYPE_META_INFO, str);
}

char *kr_web_client_type_to_str(int val) {
  return enum_to_str(KR_KR_WEB_CLIENT_TYPE_META_INFO, val);
}

int str_to_kr_web_client_state(char *str) {
  return str_to_enum(KR_KR_WEB_CLIENT_STATE_META_INFO, str);
}

char *kr_web_client_state_to_str(int val) {
  return enum_to_str(KR_KR_WEB_CLIENT_STATE_META_INFO, val);
}

int str_to_kr_web_server_type(char *str) {
  return str_to_enum(KR_KR_WEB_SERVER_TYPE_META_INFO, str);
}

char *kr_web_server_type_to_str(int val) {
  return enum_to_str(KR_KR_WEB_SERVER_TYPE_META_INFO, val);
}

int str_to_kr_multipart_state(char *str) {
  return str_to_enum(KR_KR_MULTIPART_STATE_META_INFO, str);
}

char *kr_multipart_state_to_str(int val) {
  return enum_to_str(KR_KR_MULTIPART_STATE_META_INFO, val);
}

int kr_web_server_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_INFO_META_INFO, 1);
  return res;
}

int kr_web_server_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_INFO_META_INFO, 2);
  return res;
}

int kr_web_server_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_INFO_META_INFO, 3);
  return res;
}

int kr_web_server_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_SERVER_INFO_META_INFO);
  return res;
}

int kr_web_server_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_SERVER_INFO_META_INFO);
  return res;
}

int kr_web_server_setup_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO, 1);
  return res;
}

int kr_web_server_setup_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO, 2);
  return res;
}

int kr_web_server_setup_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO, 3);
  return res;
}

int kr_web_server_setup_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_server_setup_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_SERVER_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_remote_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_REMOTE_INFO_META_INFO, 1);
  return res;
}

int kr_web_remote_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_REMOTE_INFO_META_INFO, 2);
  return res;
}

int kr_web_remote_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_REMOTE_INFO_META_INFO, 3);
  return res;
}

int kr_web_remote_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_REMOTE_INFO_META_INFO);
  return res;
}

int kr_web_remote_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_REMOTE_INFO_META_INFO);
  return res;
}

int kr_web_socket_io_state_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_IO_STATE_META_INFO, 1);
  return res;
}

int kr_web_socket_io_state_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_IO_STATE_META_INFO, 2);
  return res;
}

int kr_web_socket_io_state_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_IO_STATE_META_INFO, 3);
  return res;
}

int kr_web_socket_io_state_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_SOCKET_IO_STATE_META_INFO);
  return res;
}

int kr_web_socket_io_state_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_SOCKET_IO_STATE_META_INFO);
  return res;
}

int str_to_kr_websocket_frame_type(char *str) {
  return str_to_enum(KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO, str);
}

char *kr_websocket_frame_type_to_str(int val) {
  return enum_to_str(KR_KR_WEBSOCKET_FRAME_TYPE_META_INFO, val);
}

int str_to_kr_websocket_protocol(char *str) {
  return str_to_enum(KR_KR_WEBSOCKET_PROTOCOL_META_INFO, str);
}

char *kr_websocket_protocol_to_str(int val) {
  return enum_to_str(KR_KR_WEBSOCKET_PROTOCOL_META_INFO, val);
}

int kr_web_socket_state_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_STATE_META_INFO, 1);
  return res;
}

int kr_web_socket_state_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_STATE_META_INFO, 2);
  return res;
}

int kr_web_socket_state_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_SOCKET_STATE_META_INFO, 3);
  return res;
}

int kr_web_socket_state_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_SOCKET_STATE_META_INFO);
  return res;
}

int kr_web_socket_state_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_SOCKET_STATE_META_INFO);
  return res;
}

int str_to_kr_http_message_state(char *str) {
  return str_to_enum(KR_KR_HTTP_MESSAGE_STATE_META_INFO, str);
}

char *kr_http_message_state_to_str(int val) {
  return enum_to_str(KR_KR_HTTP_MESSAGE_STATE_META_INFO, val);
}

int kr_web_http_io_state_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_IO_STATE_META_INFO, 1);
  return res;
}

int kr_web_http_io_state_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_IO_STATE_META_INFO, 2);
  return res;
}

int kr_web_http_io_state_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_IO_STATE_META_INFO, 3);
  return res;
}

int kr_web_http_io_state_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_HTTP_IO_STATE_META_INFO);
  return res;
}

int kr_web_http_io_state_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_HTTP_IO_STATE_META_INFO);
  return res;
}

int kr_web_http_state_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_STATE_META_INFO, 1);
  return res;
}

int kr_web_http_state_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_STATE_META_INFO, 2);
  return res;
}

int kr_web_http_state_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_HTTP_STATE_META_INFO, 3);
  return res;
}

int kr_web_http_state_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_HTTP_STATE_META_INFO);
  return res;
}

int kr_web_http_state_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_HTTP_STATE_META_INFO);
  return res;
}

int kr_web_client_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_INFO_META_INFO, 1);
  return res;
}

int kr_web_client_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_INFO_META_INFO, 2);
  return res;
}

int kr_web_client_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_INFO_META_INFO, 3);
  return res;
}

int kr_web_client_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_CLIENT_INFO_META_INFO);
  return res;
}

int kr_web_client_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_CLIENT_INFO_META_INFO);
  return res;
}

int kr_web_client_setup_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO, 1);
  return res;
}

int kr_web_client_setup_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO, 2);
  return res;
}

int kr_web_client_setup_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO, 3);
  return res;
}

int kr_web_client_setup_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_client_setup_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_CLIENT_SETUP_INFO_META_INFO);
  return res;
}

int str_to_kr_web_path_type(char *str) {
  return str_to_enum(KR_KR_WEB_PATH_TYPE_META_INFO, str);
}

char *kr_web_path_type_to_str(int val) {
  return enum_to_str(KR_KR_WEB_PATH_TYPE_META_INFO, val);
}

int kr_web_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_web_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_web_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_web_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_PATH_INFO_META_INFO);
  return res;
}

int kr_web_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_PATH_INFO_META_INFO);
  return res;
}

int kr_web_path_setup_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_SETUP_INFO_META_INFO, 1);
  return res;
}

int kr_web_path_setup_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_SETUP_INFO_META_INFO, 2);
  return res;
}

int kr_web_path_setup_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_WEB_PATH_SETUP_INFO_META_INFO, 3);
  return res;
}

int kr_web_path_setup_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_WEB_PATH_SETUP_INFO_META_INFO);
  return res;
}

int kr_web_path_setup_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_WEB_PATH_SETUP_INFO_META_INFO);
  return res;
}

int kr_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_PATH_INFO_META_INFO);
  return res;
}

int kr_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_PATH_INFO_META_INFO);
  return res;
}

int kr_path_list_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_LIST_META_INFO, 1);
  return res;
}

int kr_path_list_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_LIST_META_INFO, 2);
  return res;
}

int kr_path_list_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PATH_LIST_META_INFO, 3);
  return res;
}

int kr_path_list_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_PATH_LIST_META_INFO);
  return res;
}

int kr_path_list_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_PATH_LIST_META_INFO);
  return res;
}

