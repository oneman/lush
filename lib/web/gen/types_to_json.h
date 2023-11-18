#if !defined(_WEB_GEN_TYPES_TO_JSON_H)
#define _WEB_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_radio_info_pack_json(char *json, void *info, int max);
int kr_web_server_info_pack_json(char *json, void *info, int max);
int kr_web_server_setup_info_pack_json(char *json, void *info, int max);
int kr_web_remote_info_pack_json(char *json, void *info, int max);
int kr_web_socket_io_state_pack_json(char *json, void *info, int max);
int kr_web_socket_state_pack_json(char *json, void *info, int max);
int kr_web_http_io_state_pack_json(char *json, void *info, int max);
int kr_web_http_state_pack_json(char *json, void *info, int max);
int kr_web_client_info_pack_json(char *json, void *info, int max);
int kr_web_client_setup_info_pack_json(char *json, void *info, int max);
int kr_web_path_info_pack_json(char *json, void *info, int max);
int kr_web_path_setup_info_pack_json(char *json, void *info, int max);
int kr_path_info_pack_json(char *json, void *info, int max);
int kr_path_list_pack_json(char *json, void *info, int max);
#endif

