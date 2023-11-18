#if !defined(_WEB_GEN_TYPES_FROM_EBML_H)
#define _WEB_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_radio_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_server_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_server_setup_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_remote_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_socket_io_state_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_socket_state_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_http_io_state_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_http_state_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_client_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_client_setup_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_path_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_web_path_setup_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_path_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_path_list_unpack_ebml(kr_ebml *ebml, void *info);
#endif

