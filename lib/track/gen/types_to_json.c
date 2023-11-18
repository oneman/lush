#include "types_to_json.h"

#include <krad/util/util.h>
int kr_track_name_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TRACK_NAME_META_INFO);
  return res;
}

int kr_mux_name_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MUX_NAME_META_INFO);
  return res;
}

int kr_demux_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_DEMUX_INFO_META_INFO);
  return res;
}

int kr_mux_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_MUX_INFO_META_INFO);
  return res;
}

int kr_video_encode_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_VIDEO_ENCODE_INFO_META_INFO);
  return res;
}

int kr_audio_encode_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_AUDIO_ENCODE_INFO_META_INFO);
  return res;
}

int kr_encode_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_ENCODE_INFO_META_INFO);
  return res;
}

int kr_record_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RECORD_INFO_META_INFO);
  return res;
}

int kr_transmission_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TRANSMISSION_INFO_META_INFO);
  return res;
}

int kr_output_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_capture_audio_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CAPTURE_AUDIO_INFO_META_INFO);
  return res;
}

int kr_capture_video_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CAPTURE_VIDEO_INFO_META_INFO);
  return res;
}

int kr_capture_image_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CAPTURE_IMAGE_INFO_META_INFO);
  return res;
}

int kr_capture_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_CAPTURE_INFO_META_INFO);
  return res;
}

int kr_source_file_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_SOURCE_FILE_INFO_META_INFO);
  return res;
}

int kr_source_remote_file_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO);
  return res;
}

int kr_source_stream_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_SOURCE_STREAM_INFO_META_INFO);
  return res;
}

int kr_studio_source_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_STUDIO_SOURCE_INFO_META_INFO);
  return res;
}

int kr_decode_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_DECODE_INFO_META_INFO);
  return res;
}

int kr_play_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_PLAY_INFO_META_INFO);
  return res;
}

int kr_render_image_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RENDER_IMAGE_INFO_META_INFO);
  return res;
}

int kr_render_video_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RENDER_VIDEO_INFO_META_INFO);
  return res;
}

int kr_render_audio_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RENDER_AUDIO_INFO_META_INFO);
  return res;
}

int kr_render_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_RENDER_INFO_META_INFO);
  return res;
}

int kr_generate_audio_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_GENERATE_AUDIO_INFO_META_INFO);
  return res;
}

int kr_generate_color_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_GENERATE_COLOR_INFO_META_INFO);
  return res;
}

int kr_generate_video_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_GENERATE_VIDEO_INFO_META_INFO);
  return res;
}

int kr_generate_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_GENERATE_INFO_META_INFO);
  return res;
}

int kr_studio_path_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_STUDIO_PATH_INFO_META_INFO);
  return res;
}

int kr_track_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TRACK_INFO_META_INFO);
  return res;
}

int kr_track_block_info_pack_json(char *json, void *info, int max) {
  int res;
  res = info_pack_json(json, info, max, KR_KR_TRACK_BLOCK_INFO_META_INFO);
  return res;
}

