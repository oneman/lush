#include "types_from_json.h"

#include <krad/util/util.h>
int kr_hls_variant_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_VARIANT_META_INFO);
  return res;
}

int kr_hls_rendition_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_RENDITION_META_INFO);
  return res;
}

int kr_hls_master_playlist_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_MASTER_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_media_segment_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_MEDIA_SEGMENT_META_INFO);
  return res;
}

int kr_hls_media_playlist_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_playlist_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_tuner_info_unpack_json(char *json, void *info) {
  int res;
  res = info_unpack_json(json, info, KR_KR_HLS_TUNER_INFO_META_INFO);
  return res;
}

