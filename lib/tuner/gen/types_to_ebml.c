#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_hls_variant_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_VARIANT_META_INFO, 0);
  return res;
}

int kr_hls_rendition_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_RENDITION_META_INFO, 0);
  return res;
}

int kr_hls_master_playlist_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_MASTER_PLAYLIST_META_INFO, 0);
  return res;
}

int kr_hls_media_segment_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_MEDIA_SEGMENT_META_INFO, 0);
  return res;
}

int kr_hls_media_playlist_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO, 0);
  return res;
}

int kr_hls_playlist_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_PLAYLIST_META_INFO, 0);
  return res;
}

int kr_hls_tuner_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_HLS_TUNER_INFO_META_INFO, 0);
  return res;
}

