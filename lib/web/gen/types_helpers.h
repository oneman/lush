#if !defined(_WEB_GEN_TYPES_HELPERS_H)
#define _WEB_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_radio_mode(char *str);
char *kr_radio_mode_to_str(int val);
int kr_radio_info_init(void *info);
int kr_radio_info_valid(void *info);
int kr_radio_info_random(void *info);
int kr_radio_info_patch_path(kr_patch *patch, kr_path *path);
int kr_radio_info_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_web_client_protocol(char *str);
char *kr_web_client_protocol_to_str(int val);
int str_to_kr_web_client_type(char *str);
char *kr_web_client_type_to_str(int val);
int str_to_kr_web_client_state(char *str);
char *kr_web_client_state_to_str(int val);
int str_to_kr_web_server_type(char *str);
char *kr_web_server_type_to_str(int val);
int str_to_kr_multipart_state(char *str);
char *kr_multipart_state_to_str(int val);
int kr_web_server_info_init(void *info);
int kr_web_server_info_valid(void *info);
int kr_web_server_info_random(void *info);
int kr_web_server_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_server_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_server_setup_info_init(void *info);
int kr_web_server_setup_info_valid(void *info);
int kr_web_server_setup_info_random(void *info);
int kr_web_server_setup_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_server_setup_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_remote_info_init(void *info);
int kr_web_remote_info_valid(void *info);
int kr_web_remote_info_random(void *info);
int kr_web_remote_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_remote_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_socket_io_state_init(void *info);
int kr_web_socket_io_state_valid(void *info);
int kr_web_socket_io_state_random(void *info);
int kr_web_socket_io_state_patch_path(kr_patch *patch, kr_path *path);
int kr_web_socket_io_state_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_websocket_frame_type(char *str);
char *kr_websocket_frame_type_to_str(int val);
int str_to_kr_websocket_protocol(char *str);
char *kr_websocket_protocol_to_str(int val);
int kr_web_socket_state_init(void *info);
int kr_web_socket_state_valid(void *info);
int kr_web_socket_state_random(void *info);
int kr_web_socket_state_patch_path(kr_patch *patch, kr_path *path);
int kr_web_socket_state_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_http_message_state(char *str);
char *kr_http_message_state_to_str(int val);
int kr_web_http_io_state_init(void *info);
int kr_web_http_io_state_valid(void *info);
int kr_web_http_io_state_random(void *info);
int kr_web_http_io_state_patch_path(kr_patch *patch, kr_path *path);
int kr_web_http_io_state_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_http_state_init(void *info);
int kr_web_http_state_valid(void *info);
int kr_web_http_state_random(void *info);
int kr_web_http_state_patch_path(kr_patch *patch, kr_path *path);
int kr_web_http_state_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_client_info_init(void *info);
int kr_web_client_info_valid(void *info);
int kr_web_client_info_random(void *info);
int kr_web_client_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_client_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_client_setup_info_init(void *info);
int kr_web_client_setup_info_valid(void *info);
int kr_web_client_setup_info_random(void *info);
int kr_web_client_setup_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_client_setup_info_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_web_path_type(char *str);
char *kr_web_path_type_to_str(int val);
int kr_web_path_info_init(void *info);
int kr_web_path_info_valid(void *info);
int kr_web_path_info_random(void *info);
int kr_web_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_web_path_setup_info_init(void *info);
int kr_web_path_setup_info_valid(void *info);
int kr_web_path_setup_info_random(void *info);
int kr_web_path_setup_info_patch_path(kr_patch *patch, kr_path *path);
int kr_web_path_setup_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_path_info_init(void *info);
int kr_path_info_valid(void *info);
int kr_path_info_random(void *info);
int kr_path_info_patch_path(kr_patch *patch, kr_path *path);
int kr_path_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_path_list_init(void *info);
int kr_path_list_valid(void *info);
int kr_path_list_random(void *info);
int kr_path_list_patch_path(kr_patch *patch, kr_path *path);
int kr_path_list_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

