#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_video_codec(char *str) {
  return str_to_enum(KR_KR_VIDEO_CODEC_META_INFO, str);
}

char *kr_video_codec_to_str(int val) {
  return enum_to_str(KR_KR_VIDEO_CODEC_META_INFO, val);
}

int str_to_kr_audio_codec(char *str) {
  return str_to_enum(KR_KR_AUDIO_CODEC_META_INFO, str);
}

char *kr_audio_codec_to_str(int val) {
  return enum_to_str(KR_KR_AUDIO_CODEC_META_INFO, val);
}

int str_to_kr_codec_type(char *str) {
  return str_to_enum(KR_KR_CODEC_TYPE_META_INFO, str);
}

char *kr_codec_type_to_str(int val) {
  return enum_to_str(KR_KR_CODEC_TYPE_META_INFO, val);
}

int kr_codec_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODEC_META_INFO, 1);
  return res;
}

int kr_codec_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODEC_META_INFO, 2);
  return res;
}

int kr_codec_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_CODEC_META_INFO, 3);
  return res;
}

int kr_codec_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_CODEC_META_INFO);
  return res;
}

int kr_codec_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_CODEC_META_INFO);
  return res;
}

int kr_audio_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_INFO_META_INFO, 1);
  return res;
}

int kr_audio_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_INFO_META_INFO, 2);
  return res;
}

int kr_audio_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AUDIO_INFO_META_INFO, 3);
  return res;
}

int kr_audio_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AUDIO_INFO_META_INFO);
  return res;
}

int kr_audio_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AUDIO_INFO_META_INFO);
  return res;
}

int kr_video_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_INFO_META_INFO, 1);
  return res;
}

int kr_video_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_INFO_META_INFO, 2);
  return res;
}

int kr_video_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VIDEO_INFO_META_INFO, 3);
  return res;
}

int kr_video_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_VIDEO_INFO_META_INFO);
  return res;
}

int kr_video_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_VIDEO_INFO_META_INFO);
  return res;
}

int kr_av_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AV_INFO_META_INFO, 1);
  return res;
}

int kr_av_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AV_INFO_META_INFO, 2);
  return res;
}

int kr_av_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AV_INFO_META_INFO, 3);
  return res;
}

int kr_av_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AV_INFO_META_INFO);
  return res;
}

int kr_av_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AV_INFO_META_INFO);
  return res;
}

int kr_daala_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DAALA_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_daala_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DAALA_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_daala_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_DAALA_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_daala_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_DAALA_ENCODER_INFO_META_INFO);
  return res;
}

int kr_daala_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_DAALA_ENCODER_INFO_META_INFO);
  return res;
}

int kr_vpx_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VPX_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_vpx_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VPX_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_vpx_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VPX_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_vpx_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_VPX_ENCODER_INFO_META_INFO);
  return res;
}

int kr_vpx_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_VPX_ENCODER_INFO_META_INFO);
  return res;
}

int kr_h264_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_H264_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_h264_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_H264_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_h264_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_H264_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_h264_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_H264_ENCODER_INFO_META_INFO);
  return res;
}

int kr_h264_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_H264_ENCODER_INFO_META_INFO);
  return res;
}

int kr_theora_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_THEORA_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_theora_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_THEORA_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_theora_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_THEORA_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_theora_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_THEORA_ENCODER_INFO_META_INFO);
  return res;
}

int kr_theora_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_THEORA_ENCODER_INFO_META_INFO);
  return res;
}

int kr_y4m_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_Y4M_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_y4m_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_Y4M_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_y4m_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_Y4M_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_y4m_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_Y4M_ENCODER_INFO_META_INFO);
  return res;
}

int kr_y4m_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_Y4M_ENCODER_INFO_META_INFO);
  return res;
}

int kr_flac_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FLAC_INFO_META_INFO, 1);
  return res;
}

int kr_flac_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FLAC_INFO_META_INFO, 2);
  return res;
}

int kr_flac_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_FLAC_INFO_META_INFO, 3);
  return res;
}

int kr_flac_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_FLAC_INFO_META_INFO);
  return res;
}

int kr_flac_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_FLAC_INFO_META_INFO);
  return res;
}

int str_to_kr_opus_app(char *str) {
  return str_to_enum(KR_KR_OPUS_APP_META_INFO, str);
}

char *kr_opus_app_to_str(int val) {
  return enum_to_str(KR_KR_OPUS_APP_META_INFO, val);
}

int kr_opus_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OPUS_INFO_META_INFO, 1);
  return res;
}

int kr_opus_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OPUS_INFO_META_INFO, 2);
  return res;
}

int kr_opus_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_OPUS_INFO_META_INFO, 3);
  return res;
}

int kr_opus_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_OPUS_INFO_META_INFO);
  return res;
}

int kr_opus_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_OPUS_INFO_META_INFO);
  return res;
}

int kr_vorbis_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VORBIS_INFO_META_INFO, 1);
  return res;
}

int kr_vorbis_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VORBIS_INFO_META_INFO, 2);
  return res;
}

int kr_vorbis_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_VORBIS_INFO_META_INFO, 3);
  return res;
}

int kr_vorbis_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_VORBIS_INFO_META_INFO);
  return res;
}

int kr_vorbis_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_VORBIS_INFO_META_INFO);
  return res;
}

int kr_mp3_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MP3_INFO_META_INFO, 1);
  return res;
}

int kr_mp3_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MP3_INFO_META_INFO, 2);
  return res;
}

int kr_mp3_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_MP3_INFO_META_INFO, 3);
  return res;
}

int kr_mp3_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_MP3_INFO_META_INFO);
  return res;
}

int kr_mp3_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_MP3_INFO_META_INFO);
  return res;
}

int kr_aac_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AAC_INFO_META_INFO, 1);
  return res;
}

int kr_aac_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AAC_INFO_META_INFO, 2);
  return res;
}

int kr_aac_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_AAC_INFO_META_INFO, 3);
  return res;
}

int kr_aac_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_AAC_INFO_META_INFO);
  return res;
}

int kr_aac_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_AAC_INFO_META_INFO);
  return res;
}

int kr_lavc_encoder_info_init(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LAVC_ENCODER_INFO_META_INFO, 1);
  return res;
}

int kr_lavc_encoder_info_valid(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LAVC_ENCODER_INFO_META_INFO, 2);
  return res;
}

int kr_lavc_encoder_info_random(void *info) {
  int res;
  res = info_helpers(info, KR_KR_LAVC_ENCODER_INFO_META_INFO, 3);
  return res;
}

int kr_lavc_encoder_info_patch_path(kr_patch *patch, kr_path *path) {
  int res;
  res = patch_path(patch, path, KR_KR_LAVC_ENCODER_INFO_META_INFO);
  return res;
}

int kr_lavc_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2) {
  int res;
  res = info_diff(diffset, info1, info2, KR_KR_LAVC_ENCODER_INFO_META_INFO);
  return res;
}

