#include "types_to_text.h"

#include <krad/util/util.h>
int kr_upload_file_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_UPLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_upload_stash_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_UPLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_download_file_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DOWNLOAD_FILE_INFO_META_INFO);
  return res;
}

int kr_download_stash_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DOWNLOAD_STASH_INFO_META_INFO);
  return res;
}

int kr_upload_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_UPLOAD_INFO_META_INFO);
  return res;
}

int kr_download_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DOWNLOAD_INFO_META_INFO);
  return res;
}

int kr_accept_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_ACCEPT_INFO_META_INFO);
  return res;
}

int kr_serve_range_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_RANGE_INFO_META_INFO);
  return res;
}

int kr_serve_ranges_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_RANGES_INFO_META_INFO);
  return res;
}

int kr_serve_full_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_FULL_INFO_META_INFO);
  return res;
}

int kr_serve_index_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_INDEX_INFO_META_INFO);
  return res;
}

int kr_serve_redirect_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_REDIRECT_INFO_META_INFO);
  return res;
}

int kr_serve_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SERVE_INFO_META_INFO);
  return res;
}

int kr_downlink_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_DOWNLINK_INFO_META_INFO);
  return res;
}

int kr_uplink_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_UPLINK_INFO_META_INFO);
  return res;
}

int kr_source_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_SOURCE_INFO_META_INFO);
  return res;
}

int kr_transmit_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_TRANSMIT_INFO_META_INFO);
  return res;
}

int kr_xfer_reconnect_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_RECONNECT_INFO_META_INFO);
  return res;
}

int kr_xfer_client_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_CLIENT_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_dir_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_ACCEPTOR_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_file_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_ACCEPTOR_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_buf_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_ACCEPTOR_BUF_INFO_META_INFO);
  return res;
}

int kr_xfer_share_dir_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_SHARE_DIR_INFO_META_INFO);
  return res;
}

int kr_xfer_share_file_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_SHARE_FILE_INFO_META_INFO);
  return res;
}

int kr_xfer_share_buf_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_SHARE_BUF_INFO_META_INFO);
  return res;
}

int kr_xfer_acceptor_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_ACCEPTOR_INFO_META_INFO);
  return res;
}

int kr_xfer_receiver_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_RECEIVER_INFO_META_INFO);
  return res;
}

int kr_xfer_transmitter_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_TRANSMITTER_INFO_META_INFO);
  return res;
}

int kr_xfer_share_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_SHARE_INFO_META_INFO);
  return res;
}

int kr_xfer_server_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_SERVER_INFO_META_INFO);
  return res;
}

int kr_xfer_path_info_to_text(char *text, void *info, int max) {
  int res;
  res = info_to_text(text, info, max, KR_KR_XFER_PATH_INFO_META_INFO);
  return res;
}

