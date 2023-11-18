#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_hls_playlist_type(char *str) {
  return str_to_enum(KR_KR_HLS_PLAYLIST_TYPE_META_INFO, str);
}

char *kr_hls_playlist_type_to_str(int val) {
  return enum_to_str(KR_KR_HLS_PLAYLIST_TYPE_META_INFO, val);
}

int str_to_kr_hls_rendition_type(char *str) {
  return str_to_enum(KR_KR_HLS_RENDITION_TYPE_META_INFO, str);
}

char *kr_hls_rendition_type_to_str(int val) {
  return enum_to_str(KR_KR_HLS_RENDITION_TYPE_META_INFO, val);
}

int str_to_kr_hls_media_playlist_type(char *str) {
  return str_to_enum(KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO, str);
}

char *kr_hls_media_playlist_type_to_str(int val) {
  return enum_to_str(KR_KR_HLS_MEDIA_PLAYLIST_TYPE_META_INFO, val);
}

int kr_hls_variant_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_VARIANT_META_INFO, 1);
  return res;
}

int kr_hls_variant_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_VARIANT_META_INFO, 2);
  return res;
}

int kr_hls_variant_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_VARIANT_META_INFO, 3);
  return res;
}

int kr_hls_variant_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_VARIANT_META_INFO);
  return res;
}

int kr_hls_variant_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_VARIANT_META_INFO);
  return res;
}

int kr_hls_rendition_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_RENDITION_META_INFO, 1);
  return res;
}

int kr_hls_rendition_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_RENDITION_META_INFO, 2);
  return res;
}

int kr_hls_rendition_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_RENDITION_META_INFO, 3);
  return res;
}

int kr_hls_rendition_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_RENDITION_META_INFO);
  return res;
}

int kr_hls_rendition_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_RENDITION_META_INFO);
  return res;
}

int kr_hls_master_playlist_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MASTER_PLAYLIST_META_INFO, 1);
  return res;
}

int kr_hls_master_playlist_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MASTER_PLAYLIST_META_INFO, 2);
  return res;
}

int kr_hls_master_playlist_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MASTER_PLAYLIST_META_INFO, 3);
  return res;
}

int kr_hls_master_playlist_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_MASTER_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_master_playlist_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_MASTER_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_media_segment_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_SEGMENT_META_INFO, 1);
  return res;
}

int kr_hls_media_segment_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_SEGMENT_META_INFO, 2);
  return res;
}

int kr_hls_media_segment_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_SEGMENT_META_INFO, 3);
  return res;
}

int kr_hls_media_segment_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_MEDIA_SEGMENT_META_INFO);
  return res;
}

int kr_hls_media_segment_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_MEDIA_SEGMENT_META_INFO);
  return res;
}

int kr_hls_media_playlist_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO, 1);
  return res;
}

int kr_hls_media_playlist_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO, 2);
  return res;
}

int kr_hls_media_playlist_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO, 3);
  return res;
}

int kr_hls_media_playlist_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_media_playlist_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_MEDIA_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_playlist_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_PLAYLIST_META_INFO, 1);
  return res;
}

int kr_hls_playlist_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_PLAYLIST_META_INFO, 2);
  return res;
}

int kr_hls_playlist_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_PLAYLIST_META_INFO, 3);
  return res;
}

int kr_hls_playlist_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_playlist_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_PLAYLIST_META_INFO);
  return res;
}

int kr_hls_tuner_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_TUNER_INFO_META_INFO, 1);
  return res;
}

int kr_hls_tuner_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_TUNER_INFO_META_INFO, 2);
  return res;
}

int kr_hls_tuner_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_HLS_TUNER_INFO_META_INFO, 3);
  return res;
}

int kr_hls_tuner_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_HLS_TUNER_INFO_META_INFO);
  return res;
}

int kr_hls_tuner_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_HLS_TUNER_INFO_META_INFO);
  return res;
}

