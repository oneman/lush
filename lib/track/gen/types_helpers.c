#include "types_helpers.h"

#include <krad/util/util.h>
int kr_track_name_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_NAME_META_INFO, 1);
  return res;
}

int kr_track_name_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_NAME_META_INFO, 2);
  return res;
}

int kr_track_name_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_NAME_META_INFO, 3);
  return res;
}

int kr_track_name_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TRACK_NAME_META_INFO);
  return res;
}

int kr_track_name_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TRACK_NAME_META_INFO);
  return res;
}

int kr_mux_name_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_NAME_META_INFO, 1);
  return res;
}

int kr_mux_name_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_NAME_META_INFO, 2);
  return res;
}

int kr_mux_name_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_NAME_META_INFO, 3);
  return res;
}

int kr_mux_name_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MUX_NAME_META_INFO);
  return res;
}

int kr_mux_name_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MUX_NAME_META_INFO);
  return res;
}

int kr_demux_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DEMUX_INFO_META_INFO, 1);
  return res;
}

int kr_demux_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DEMUX_INFO_META_INFO, 2);
  return res;
}

int kr_demux_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DEMUX_INFO_META_INFO, 3);
  return res;
}

int kr_demux_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DEMUX_INFO_META_INFO);
  return res;
}

int kr_demux_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DEMUX_INFO_META_INFO);
  return res;
}

int kr_mux_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_INFO_META_INFO, 1);
  return res;
}

int kr_mux_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_INFO_META_INFO, 2);
  return res;
}

int kr_mux_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MUX_INFO_META_INFO, 3);
  return res;
}

int kr_mux_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MUX_INFO_META_INFO);
  return res;
}

int kr_mux_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MUX_INFO_META_INFO);
  return res;
}

int kr_video_encode_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_ENCODE_INFO_META_INFO, 1);
  return res;
}

int kr_video_encode_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_ENCODE_INFO_META_INFO, 2);
  return res;
}

int kr_video_encode_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_ENCODE_INFO_META_INFO, 3);
  return res;
}

int kr_video_encode_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_VIDEO_ENCODE_INFO_META_INFO);
  return res;
}

int kr_video_encode_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_VIDEO_ENCODE_INFO_META_INFO);
  return res;
}

int kr_audio_encode_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_ENCODE_INFO_META_INFO, 1);
  return res;
}

int kr_audio_encode_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_ENCODE_INFO_META_INFO, 2);
  return res;
}

int kr_audio_encode_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_ENCODE_INFO_META_INFO, 3);
  return res;
}

int kr_audio_encode_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AUDIO_ENCODE_INFO_META_INFO);
  return res;
}

int kr_audio_encode_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AUDIO_ENCODE_INFO_META_INFO);
  return res;
}

int str_to_kr_encode_type(char *str) {
  return str_to_enum(KR_KR_ENCODE_TYPE_META_INFO, str);
}

char *kr_encode_type_to_str(int val) {
  return enum_to_str(KR_KR_ENCODE_TYPE_META_INFO, val);
}

int kr_encode_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ENCODE_INFO_META_INFO, 1);
  return res;
}

int kr_encode_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ENCODE_INFO_META_INFO, 2);
  return res;
}

int kr_encode_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_ENCODE_INFO_META_INFO, 3);
  return res;
}

int kr_encode_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_ENCODE_INFO_META_INFO);
  return res;
}

int kr_encode_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_ENCODE_INFO_META_INFO);
  return res;
}

int str_to_kr_capture_type(char *str) {
  return str_to_enum(KR_KR_CAPTURE_TYPE_META_INFO, str);
}

char *kr_capture_type_to_str(int val) {
  return enum_to_str(KR_KR_CAPTURE_TYPE_META_INFO, val);
}

int kr_record_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECORD_INFO_META_INFO, 1);
  return res;
}

int kr_record_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECORD_INFO_META_INFO, 2);
  return res;
}

int kr_record_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RECORD_INFO_META_INFO, 3);
  return res;
}

int kr_record_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RECORD_INFO_META_INFO);
  return res;
}

int kr_record_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RECORD_INFO_META_INFO);
  return res;
}

int kr_transmission_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMISSION_INFO_META_INFO, 1);
  return res;
}

int kr_transmission_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMISSION_INFO_META_INFO, 2);
  return res;
}

int kr_transmission_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRANSMISSION_INFO_META_INFO, 3);
  return res;
}

int kr_transmission_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TRANSMISSION_INFO_META_INFO);
  return res;
}

int kr_transmission_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TRANSMISSION_INFO_META_INFO);
  return res;
}

int str_to_kr_output_type(char *str) {
  return str_to_enum(KR_KR_OUTPUT_TYPE_META_INFO, str);
}

char *kr_output_type_to_str(int val) {
  return enum_to_str(KR_KR_OUTPUT_TYPE_META_INFO, val);
}

int kr_output_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OUTPUT_INFO_META_INFO, 1);
  return res;
}

int kr_output_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OUTPUT_INFO_META_INFO, 2);
  return res;
}

int kr_output_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OUTPUT_INFO_META_INFO, 3);
  return res;
}

int kr_output_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_output_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_OUTPUT_INFO_META_INFO);
  return res;
}

int kr_capture_audio_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_AUDIO_INFO_META_INFO, 1);
  return res;
}

int kr_capture_audio_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_AUDIO_INFO_META_INFO, 2);
  return res;
}

int kr_capture_audio_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_AUDIO_INFO_META_INFO, 3);
  return res;
}

int kr_capture_audio_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CAPTURE_AUDIO_INFO_META_INFO);
  return res;
}

int kr_capture_audio_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CAPTURE_AUDIO_INFO_META_INFO);
  return res;
}

int kr_capture_video_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_VIDEO_INFO_META_INFO, 1);
  return res;
}

int kr_capture_video_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_VIDEO_INFO_META_INFO, 2);
  return res;
}

int kr_capture_video_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_VIDEO_INFO_META_INFO, 3);
  return res;
}

int kr_capture_video_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CAPTURE_VIDEO_INFO_META_INFO);
  return res;
}

int kr_capture_video_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CAPTURE_VIDEO_INFO_META_INFO);
  return res;
}

int kr_capture_image_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_IMAGE_INFO_META_INFO, 1);
  return res;
}

int kr_capture_image_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_IMAGE_INFO_META_INFO, 2);
  return res;
}

int kr_capture_image_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_IMAGE_INFO_META_INFO, 3);
  return res;
}

int kr_capture_image_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CAPTURE_IMAGE_INFO_META_INFO);
  return res;
}

int kr_capture_image_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CAPTURE_IMAGE_INFO_META_INFO);
  return res;
}

int kr_capture_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_INFO_META_INFO, 1);
  return res;
}

int kr_capture_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_INFO_META_INFO, 2);
  return res;
}

int kr_capture_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CAPTURE_INFO_META_INFO, 3);
  return res;
}

int kr_capture_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CAPTURE_INFO_META_INFO);
  return res;
}

int kr_capture_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CAPTURE_INFO_META_INFO);
  return res;
}

int kr_source_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_source_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_source_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_source_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SOURCE_FILE_INFO_META_INFO);
  return res;
}

int kr_source_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SOURCE_FILE_INFO_META_INFO);
  return res;
}

int kr_source_remote_file_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO, 1);
  return res;
}

int kr_source_remote_file_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO, 2);
  return res;
}

int kr_source_remote_file_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO, 3);
  return res;
}

int kr_source_remote_file_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO);
  return res;
}

int kr_source_remote_file_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SOURCE_REMOTE_FILE_INFO_META_INFO);
  return res;
}

int str_to_kr_studio_source_stream_type(char *str) {
  return str_to_enum(KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO, str);
}

char *kr_studio_source_stream_type_to_str(int val) {
  return enum_to_str(KR_KR_STUDIO_SOURCE_STREAM_TYPE_META_INFO, val);
}

int kr_source_stream_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_STREAM_INFO_META_INFO, 1);
  return res;
}

int kr_source_stream_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_STREAM_INFO_META_INFO, 2);
  return res;
}

int kr_source_stream_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_SOURCE_STREAM_INFO_META_INFO, 3);
  return res;
}

int kr_source_stream_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_SOURCE_STREAM_INFO_META_INFO);
  return res;
}

int kr_source_stream_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_SOURCE_STREAM_INFO_META_INFO);
  return res;
}

int str_to_kr_studio_source_type(char *str) {
  return str_to_enum(KR_KR_STUDIO_SOURCE_TYPE_META_INFO, str);
}

char *kr_studio_source_type_to_str(int val) {
  return enum_to_str(KR_KR_STUDIO_SOURCE_TYPE_META_INFO, val);
}

int kr_studio_source_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_SOURCE_INFO_META_INFO, 1);
  return res;
}

int kr_studio_source_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_SOURCE_INFO_META_INFO, 2);
  return res;
}

int kr_studio_source_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_SOURCE_INFO_META_INFO, 3);
  return res;
}

int kr_studio_source_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_STUDIO_SOURCE_INFO_META_INFO);
  return res;
}

int kr_studio_source_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_STUDIO_SOURCE_INFO_META_INFO);
  return res;
}

int kr_decode_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DECODE_INFO_META_INFO, 1);
  return res;
}

int kr_decode_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DECODE_INFO_META_INFO, 2);
  return res;
}

int kr_decode_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DECODE_INFO_META_INFO, 3);
  return res;
}

int kr_decode_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DECODE_INFO_META_INFO);
  return res;
}

int kr_decode_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DECODE_INFO_META_INFO);
  return res;
}

int kr_play_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PLAY_INFO_META_INFO, 1);
  return res;
}

int kr_play_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PLAY_INFO_META_INFO, 2);
  return res;
}

int kr_play_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_PLAY_INFO_META_INFO, 3);
  return res;
}

int kr_play_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_PLAY_INFO_META_INFO);
  return res;
}

int kr_play_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_PLAY_INFO_META_INFO);
  return res;
}

int kr_render_image_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_IMAGE_INFO_META_INFO, 1);
  return res;
}

int kr_render_image_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_IMAGE_INFO_META_INFO, 2);
  return res;
}

int kr_render_image_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_IMAGE_INFO_META_INFO, 3);
  return res;
}

int kr_render_image_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RENDER_IMAGE_INFO_META_INFO);
  return res;
}

int kr_render_image_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RENDER_IMAGE_INFO_META_INFO);
  return res;
}

int kr_render_video_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_VIDEO_INFO_META_INFO, 1);
  return res;
}

int kr_render_video_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_VIDEO_INFO_META_INFO, 2);
  return res;
}

int kr_render_video_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_VIDEO_INFO_META_INFO, 3);
  return res;
}

int kr_render_video_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RENDER_VIDEO_INFO_META_INFO);
  return res;
}

int kr_render_video_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RENDER_VIDEO_INFO_META_INFO);
  return res;
}

int kr_render_audio_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_AUDIO_INFO_META_INFO, 1);
  return res;
}

int kr_render_audio_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_AUDIO_INFO_META_INFO, 2);
  return res;
}

int kr_render_audio_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_AUDIO_INFO_META_INFO, 3);
  return res;
}

int kr_render_audio_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RENDER_AUDIO_INFO_META_INFO);
  return res;
}

int kr_render_audio_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RENDER_AUDIO_INFO_META_INFO);
  return res;
}

int str_to_kr_render_type(char *str) {
  return str_to_enum(KR_KR_RENDER_TYPE_META_INFO, str);
}

char *kr_render_type_to_str(int val) {
  return enum_to_str(KR_KR_RENDER_TYPE_META_INFO, val);
}

int kr_render_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_INFO_META_INFO, 1);
  return res;
}

int kr_render_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_INFO_META_INFO, 2);
  return res;
}

int kr_render_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_RENDER_INFO_META_INFO, 3);
  return res;
}

int kr_render_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_RENDER_INFO_META_INFO);
  return res;
}

int kr_render_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_RENDER_INFO_META_INFO);
  return res;
}

int str_to_kr_audio_generate_type(char *str) {
  return str_to_enum(KR_KR_AUDIO_GENERATE_TYPE_META_INFO, str);
}

char *kr_audio_generate_type_to_str(int val) {
  return enum_to_str(KR_KR_AUDIO_GENERATE_TYPE_META_INFO, val);
}

int kr_generate_audio_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_AUDIO_INFO_META_INFO, 1);
  return res;
}

int kr_generate_audio_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_AUDIO_INFO_META_INFO, 2);
  return res;
}

int kr_generate_audio_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_AUDIO_INFO_META_INFO, 3);
  return res;
}

int kr_generate_audio_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_GENERATE_AUDIO_INFO_META_INFO);
  return res;
}

int kr_generate_audio_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_GENERATE_AUDIO_INFO_META_INFO);
  return res;
}

int str_to_kr_video_generate_type(char *str) {
  return str_to_enum(KR_KR_VIDEO_GENERATE_TYPE_META_INFO, str);
}

char *kr_video_generate_type_to_str(int val) {
  return enum_to_str(KR_KR_VIDEO_GENERATE_TYPE_META_INFO, val);
}

int kr_generate_color_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_COLOR_INFO_META_INFO, 1);
  return res;
}

int kr_generate_color_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_COLOR_INFO_META_INFO, 2);
  return res;
}

int kr_generate_color_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_COLOR_INFO_META_INFO, 3);
  return res;
}

int kr_generate_color_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_GENERATE_COLOR_INFO_META_INFO);
  return res;
}

int kr_generate_color_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_GENERATE_COLOR_INFO_META_INFO);
  return res;
}

int kr_generate_video_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_VIDEO_INFO_META_INFO, 1);
  return res;
}

int kr_generate_video_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_VIDEO_INFO_META_INFO, 2);
  return res;
}

int kr_generate_video_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_VIDEO_INFO_META_INFO, 3);
  return res;
}

int kr_generate_video_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_GENERATE_VIDEO_INFO_META_INFO);
  return res;
}

int kr_generate_video_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_GENERATE_VIDEO_INFO_META_INFO);
  return res;
}

int str_to_kr_generate_type(char *str) {
  return str_to_enum(KR_KR_GENERATE_TYPE_META_INFO, str);
}

char *kr_generate_type_to_str(int val) {
  return enum_to_str(KR_KR_GENERATE_TYPE_META_INFO, val);
}

int kr_generate_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_INFO_META_INFO, 1);
  return res;
}

int kr_generate_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_INFO_META_INFO, 2);
  return res;
}

int kr_generate_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_GENERATE_INFO_META_INFO, 3);
  return res;
}

int kr_generate_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_GENERATE_INFO_META_INFO);
  return res;
}

int kr_generate_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_GENERATE_INFO_META_INFO);
  return res;
}

int str_to_kr_studio_path_type(char *str) {
  return str_to_enum(KR_KR_STUDIO_PATH_TYPE_META_INFO, str);
}

char *kr_studio_path_type_to_str(int val) {
  return enum_to_str(KR_KR_STUDIO_PATH_TYPE_META_INFO, val);
}

int kr_studio_path_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_PATH_INFO_META_INFO, 1);
  return res;
}

int kr_studio_path_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_PATH_INFO_META_INFO, 2);
  return res;
}

int kr_studio_path_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_STUDIO_PATH_INFO_META_INFO, 3);
  return res;
}

int kr_studio_path_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_STUDIO_PATH_INFO_META_INFO);
  return res;
}

int kr_studio_path_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_STUDIO_PATH_INFO_META_INFO);
  return res;
}

int str_to_kr_track_block_type(char *str) {
  return str_to_enum(KR_KR_TRACK_BLOCK_TYPE_META_INFO, str);
}

char *kr_track_block_type_to_str(int val) {
  return enum_to_str(KR_KR_TRACK_BLOCK_TYPE_META_INFO, val);
}

int str_to_kr_track_mode(char *str) {
  return str_to_enum(KR_KR_TRACK_MODE_META_INFO, str);
}

char *kr_track_mode_to_str(int val) {
  return enum_to_str(KR_KR_TRACK_MODE_META_INFO, val);
}

int str_to_kr_track_event_type(char *str) {
  return str_to_enum(KR_KR_TRACK_EVENT_TYPE_META_INFO, str);
}

char *kr_track_event_type_to_str(int val) {
  return enum_to_str(KR_KR_TRACK_EVENT_TYPE_META_INFO, val);
}

int kr_track_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_INFO_META_INFO, 1);
  return res;
}

int kr_track_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_INFO_META_INFO, 2);
  return res;
}

int kr_track_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_INFO_META_INFO, 3);
  return res;
}

int kr_track_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TRACK_INFO_META_INFO);
  return res;
}

int kr_track_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TRACK_INFO_META_INFO);
  return res;
}

int kr_track_block_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_BLOCK_INFO_META_INFO, 1);
  return res;
}

int kr_track_block_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_BLOCK_INFO_META_INFO, 2);
  return res;
}

int kr_track_block_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_TRACK_BLOCK_INFO_META_INFO, 3);
  return res;
}

int kr_track_block_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_TRACK_BLOCK_INFO_META_INFO);
  return res;
}

int kr_track_block_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_TRACK_BLOCK_INFO_META_INFO);
  return res;
}

