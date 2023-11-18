#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_upload_type(char *str) {
  return str_to_enum(KR_KR_UPLOAD_TYPE_META_INFO, str);
}

char *kr_upload_type_to_str(int val) {
  return enum_to_str(KR_KR_UPLOAD_TYPE_META_INFO, val);
}

int str_to_kr_download_type(char *str) {
  return str_to_enum(KR_KR_DOWNLOAD_TYPE_META_INFO, str);
}

char *kr_download_type_to_str(int val) {
  return enum_to_str(KR_KR_DOWNLOAD_TYPE_META_INFO, val);
}

int kr_upload_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_upload_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_upload_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_upload_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_UPLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_upload_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_UPLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_upload_stash_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_STASH_INFO_META_INFO, 1);
  return res;
}

int kr_upload_stash_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_STASH_INFO_META_INFO, 2);
  return res;
}

int kr_upload_stash_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_STASH_INFO_META_INFO, 3);
  return res;
}

int kr_upload_stash_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_UPLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_upload_stash_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_UPLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_download_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_download_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_download_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_download_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DOWNLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_download_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DOWNLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_download_stash_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_STASH_INFO_META_INFO, 1);
  return res;
}

int kr_download_stash_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_STASH_INFO_META_INFO, 2);
  return res;
}

int kr_download_stash_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_STASH_INFO_META_INFO, 3);
  return res;
}

int kr_download_stash_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DOWNLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_download_stash_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DOWNLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_upload_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_INFO_META_INFO, 1);
  return res;
}

int kr_upload_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_INFO_META_INFO, 2);
  return res;
}

int kr_upload_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLOAD_INFO_META_INFO, 3);
  return res;
}

int kr_upload_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_UPLOAD_INFO_META_INFO);
  return res;
}

int kr_upload_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_UPLOAD_INFO_META_INFO);
  return res;
}

int kr_download_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_INFO_META_INFO, 1);
  return res;
}

int kr_download_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_INFO_META_INFO, 2);
  return res;
}

int kr_download_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLOAD_INFO_META_INFO, 3);
  return res;
}

int kr_download_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DOWNLOAD_INFO_META_INFO);
  return res;
}

int kr_download_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DOWNLOAD_INFO_META_INFO);
  return res;
}

int kr_accept_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ACCEPT_INFO_META_INFO, 1);
  return res;
}

int kr_accept_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ACCEPT_INFO_META_INFO, 2);
  return res;
}

int kr_accept_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ACCEPT_INFO_META_INFO, 3);
  return res;
}

int kr_accept_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_ACCEPT_INFO_META_INFO);
  return res;
}

int kr_accept_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_ACCEPT_INFO_META_INFO);
  return res;
}

int kr_serve_range_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGE_INFO_META_INFO, 1);
  return res;
}

int kr_serve_range_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGE_INFO_META_INFO, 2);
  return res;
}

int kr_serve_range_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGE_INFO_META_INFO, 3);
  return res;
}

int kr_serve_range_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_RANGE_INFO_META_INFO);
  return res;
}

int kr_serve_range_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_RANGE_INFO_META_INFO);
  return res;
}

int kr_serve_ranges_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGES_INFO_META_INFO, 1);
  return res;
}

int kr_serve_ranges_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGES_INFO_META_INFO, 2);
  return res;
}

int kr_serve_ranges_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_RANGES_INFO_META_INFO, 3);
  return res;
}

int kr_serve_ranges_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_RANGES_INFO_META_INFO);
  return res;
}

int kr_serve_ranges_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_RANGES_INFO_META_INFO);
  return res;
}

int kr_serve_full_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_FULL_INFO_META_INFO, 1);
  return res;
}

int kr_serve_full_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_FULL_INFO_META_INFO, 2);
  return res;
}

int kr_serve_full_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_FULL_INFO_META_INFO, 3);
  return res;
}

int kr_serve_full_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_FULL_INFO_META_INFO);
  return res;
}

int kr_serve_full_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_FULL_INFO_META_INFO);
  return res;
}

int str_to_kr_serve_index_format(char *str) {
  return str_to_enum(KR_KR_SERVE_INDEX_FORMAT_META_INFO, str);
}

char *kr_serve_index_format_to_str(int val) {
  return enum_to_str(KR_KR_SERVE_INDEX_FORMAT_META_INFO, val);
}

int kr_serve_index_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INDEX_INFO_META_INFO, 1);
  return res;
}

int kr_serve_index_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INDEX_INFO_META_INFO, 2);
  return res;
}

int kr_serve_index_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INDEX_INFO_META_INFO, 3);
  return res;
}

int kr_serve_index_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_INDEX_INFO_META_INFO);
  return res;
}

int kr_serve_index_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_INDEX_INFO_META_INFO);
  return res;
}

int kr_serve_redirect_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_REDIRECT_INFO_META_INFO, 1);
  return res;
}

int kr_serve_redirect_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_REDIRECT_INFO_META_INFO, 2);
  return res;
}

int kr_serve_redirect_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_REDIRECT_INFO_META_INFO, 3);
  return res;
}

int kr_serve_redirect_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_REDIRECT_INFO_META_INFO);
  return res;
}

int kr_serve_redirect_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_REDIRECT_INFO_META_INFO);
  return res;
}

int str_to_kr_serve_type(char *str) {
  return str_to_enum(KR_KR_SERVE_TYPE_META_INFO, str);
}

char *kr_serve_type_to_str(int val) {
  return enum_to_str(KR_KR_SERVE_TYPE_META_INFO, val);
}

int kr_serve_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INFO_META_INFO, 1);
  return res;
}

int kr_serve_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INFO_META_INFO, 2);
  return res;
}

int kr_serve_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SERVE_INFO_META_INFO, 3);
  return res;
}

int kr_serve_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SERVE_INFO_META_INFO);
  return res;
}

int kr_serve_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SERVE_INFO_META_INFO);
  return res;
}

int kr_downlink_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLINK_INFO_META_INFO, 1);
  return res;
}

int kr_downlink_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLINK_INFO_META_INFO, 2);
  return res;
}

int kr_downlink_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DOWNLINK_INFO_META_INFO, 3);
  return res;
}

int kr_downlink_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DOWNLINK_INFO_META_INFO);
  return res;
}

int kr_downlink_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DOWNLINK_INFO_META_INFO);
  return res;
}

int kr_uplink_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLINK_INFO_META_INFO, 1);
  return res;
}

int kr_uplink_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLINK_INFO_META_INFO, 2);
  return res;
}

int kr_uplink_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_UPLINK_INFO_META_INFO, 3);
  return res;
}

int kr_uplink_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_UPLINK_INFO_META_INFO);
  return res;
}

int kr_uplink_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_UPLINK_INFO_META_INFO);
  return res;
}

int kr_source_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_INFO_META_INFO, 1);
  return res;
}

int kr_source_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_INFO_META_INFO, 2);
  return res;
}

int kr_source_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_INFO_META_INFO, 3);
  return res;
}

int kr_source_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_source_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_transmit_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMIT_INFO_META_INFO, 1);
  return res;
}

int kr_transmit_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMIT_INFO_META_INFO, 2);
  return res;
}

int kr_transmit_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMIT_INFO_META_INFO, 3);
  return res;
}

int kr_transmit_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TRANSMIT_INFO_META_INFO);
  return res;
}

int kr_transmit_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TRANSMIT_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_client_type(char *str) {
  return str_to_enum(KR_KR_XFER_CLIENT_TYPE_META_INFO, str);
}

char *kr_xfer_client_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_CLIENT_TYPE_META_INFO, val);
}

int kr_xfer_reconnect_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECONNECT_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_reconnect_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECONNECT_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_reconnect_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECONNECT_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_reconnect_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_RECONNECT_INFO_META_INFO);
  return res;
}

int kr_xfer_reconnect_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_RECONNECT_INFO_META_INFO);
  return res;
}

int kr_xfer_client_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_CLIENT_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_client_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_CLIENT_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_client_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_CLIENT_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_client_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_CLIENT_INFO_META_INFO);
  return res;
}

int kr_xfer_client_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_CLIENT_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_dir_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_acceptor_dir_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_acceptor_dir_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_acceptor_dir_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_dir_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_acceptor_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_acceptor_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_acceptor_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_buf_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_acceptor_buf_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_acceptor_buf_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_acceptor_buf_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_buf_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO);
  return res;
}

int kr_xfer_share_dir_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_DIR_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_share_dir_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_DIR_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_share_dir_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_DIR_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_share_dir_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_SHARE_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_share_dir_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_SHARE_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_share_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_share_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_share_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_share_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_SHARE_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_share_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_SHARE_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_share_buf_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_BUF_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_share_buf_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_BUF_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_share_buf_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_BUF_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_share_buf_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_SHARE_BUF_INFO_META_INFO);
  return res;
}

int kr_xfer_share_buf_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_SHARE_BUF_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_acceptor_type(char *str) {
  return str_to_enum(KR_KR_XFER_ACCEPTOR_TYPE_META_INFO, str);
}

char *kr_xfer_acceptor_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_ACCEPTOR_TYPE_META_INFO, val);
}

int kr_xfer_acceptor_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_acceptor_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_acceptor_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_ACCEPTOR_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_acceptor_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_ACCEPTOR_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_ACCEPTOR_INFO_META_INFO);
  return res;
}

int kr_xfer_receiver_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECEIVER_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_receiver_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECEIVER_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_receiver_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_RECEIVER_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_receiver_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_RECEIVER_INFO_META_INFO);
  return res;
}

int kr_xfer_receiver_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_RECEIVER_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_transmitter_type(char *str) {
  return str_to_enum(KR_KR_XFER_TRANSMITTER_TYPE_META_INFO, str);
}

char *kr_xfer_transmitter_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_TRANSMITTER_TYPE_META_INFO, val);
}

int kr_xfer_transmitter_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_TRANSMITTER_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_transmitter_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_TRANSMITTER_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_transmitter_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_TRANSMITTER_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_transmitter_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_TRANSMITTER_INFO_META_INFO);
  return res;
}

int kr_xfer_transmitter_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_TRANSMITTER_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_share_type(char *str) {
  return str_to_enum(KR_KR_XFER_SHARE_TYPE_META_INFO, str);
}

char *kr_xfer_share_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_SHARE_TYPE_META_INFO, val);
}

int kr_xfer_share_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_share_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_share_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SHARE_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_share_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_SHARE_INFO_META_INFO);
  return res;
}

int kr_xfer_share_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_SHARE_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_server_type(char *str) {
  return str_to_enum(KR_KR_XFER_SERVER_TYPE_META_INFO, str);
}

char *kr_xfer_server_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_SERVER_TYPE_META_INFO, val);
}

int kr_xfer_server_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SERVER_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_server_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SERVER_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_server_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_SERVER_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_server_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_SERVER_INFO_META_INFO);
  return res;
}

int kr_xfer_server_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_SERVER_INFO_META_INFO);
  return res;
}

int str_to_kr_xfer_path_type(char *str) {
  return str_to_enum(KR_KR_XFER_PATH_TYPE_META_INFO, str);
}

char *kr_xfer_path_type_to_str(int val) {
  return enum_to_str(KR_KR_XFER_PATH_TYPE_META_INFO, val);
}

int kr_xfer_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_xfer_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_xfer_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_XFER_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_xfer_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_XFER_PATH_INFO_META_INFO);
  return res;
}

int kr_xfer_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_XFER_PATH_INFO_META_INFO);
  return res;
}

