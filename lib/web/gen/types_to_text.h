#if !defined(_WEB_GEN_TYPES_TO_TEXT_H)
#define _WEB_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_radio_info_to_text(char *text, void *info, int max);
int kr_web_server_info_to_text(char *text, void *info, int max);
int kr_web_server_setup_info_to_text(char *text, void *info, int max);
int kr_web_remote_info_to_text(char *text, void *info, int max);
int kr_web_socket_io_state_to_text(char *text, void *info, int max);
int kr_web_socket_state_to_text(char *text, void *info, int max);
int kr_web_http_io_state_to_text(char *text, void *info, int max);
int kr_web_http_state_to_text(char *text, void *info, int max);
int kr_web_client_info_to_text(char *text, void *info, int max);
int kr_web_client_setup_info_to_text(char *text, void *info, int max);
int kr_web_path_info_to_text(char *text, void *info, int max);
int kr_web_path_setup_info_to_text(char *text, void *info, int max);
int kr_path_info_to_text(char *text, void *info, int max);
int kr_path_list_to_text(char *text, void *info, int max);
#endif

