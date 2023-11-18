#if !defined(_XFER_GEN_TYPES_FROM_JSON_H)
#define _XFER_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_upload_file_info_unpack_json(char *json, void *info);
int kr_upload_stash_info_unpack_json(char *json, void *info);
int kr_download_file_info_unpack_json(char *json, void *info);
int kr_download_stash_info_unpack_json(char *json, void *info);
int kr_upload_info_unpack_json(char *json, void *info);
int kr_download_info_unpack_json(char *json, void *info);
int kr_accept_info_unpack_json(char *json, void *info);
int kr_serve_range_info_unpack_json(char *json, void *info);
int kr_serve_ranges_info_unpack_json(char *json, void *info);
int kr_serve_full_info_unpack_json(char *json, void *info);
int kr_serve_index_info_unpack_json(char *json, void *info);
int kr_serve_redirect_info_unpack_json(char *json, void *info);
int kr_serve_info_unpack_json(char *json, void *info);
int kr_downlink_info_unpack_json(char *json, void *info);
int kr_uplink_info_unpack_json(char *json, void *info);
int kr_source_info_unpack_json(char *json, void *info);
int kr_transmit_info_unpack_json(char *json, void *info);
int kr_xfer_reconnect_info_unpack_json(char *json, void *info);
int kr_xfer_client_info_unpack_json(char *json, void *info);
int kr_xfer_acceptor_dir_info_unpack_json(char *json, void *info);
int kr_xfer_acceptor_file_info_unpack_json(char *json, void *info);
int kr_xfer_acceptor_buf_info_unpack_json(char *json, void *info);
int kr_xfer_share_dir_info_unpack_json(char *json, void *info);
int kr_xfer_share_file_info_unpack_json(char *json, void *info);
int kr_xfer_share_buf_info_unpack_json(char *json, void *info);
int kr_xfer_acceptor_info_unpack_json(char *json, void *info);
int kr_xfer_receiver_info_unpack_json(char *json, void *info);
int kr_xfer_transmitter_info_unpack_json(char *json, void *info);
int kr_xfer_share_info_unpack_json(char *json, void *info);
int kr_xfer_server_info_unpack_json(char *json, void *info);
int kr_xfer_path_info_unpack_json(char *json, void *info);
#endif

