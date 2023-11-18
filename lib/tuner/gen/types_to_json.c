#include "types_to_json.h"

#include <krad/util/util.h>
int kr_hls_variant_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_VARIANT_META_INFO);
  return res;
}

int kr_hls_rendition_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_RENDITION_META_INFO);
  return res;
}

int kr_hls_master_playlist_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_MASTER_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_media_segment_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_MEDIA_SEGMENT_META_INFO);
  return res;
}

int kr_hls_media_playlist_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_playlist_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_tuner_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_HLS_TUNER_INFO_META_INFO);
  return res;
}

