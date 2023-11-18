#if !defined(_CODEC_GEN_TYPES_HELPERS_H)
#define _CODEC_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_video_codec(char *str);
char *kr_video_codec_to_str(int val);
int str_to_kr_audio_codec(char *str);
char *kr_audio_codec_to_str(int val);
int str_to_kr_codec_type(char *str);
char *kr_codec_type_to_str(int val);
int kr_codec_init(void *info);
int kr_codec_valid(void *info);
int kr_codec_random(void *info);
int kr_codec_patch_path(kr_patch *patch, kr_path *path);
int kr_codec_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_audio_info_init(void *info);
int kr_audio_info_valid(void *info);
int kr_audio_info_random(void *info);
int kr_audio_info_patch_path(kr_patch *patch, kr_path *path);
int kr_audio_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_video_info_init(void *info);
int kr_video_info_valid(void *info);
int kr_video_info_random(void *info);
int kr_video_info_patch_path(kr_patch *patch, kr_path *path);
int kr_video_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_av_info_init(void *info);
int kr_av_info_valid(void *info);
int kr_av_info_random(void *info);
int kr_av_info_patch_path(kr_patch *patch, kr_path *path);
int kr_av_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_daala_encoder_info_init(void *info);
int kr_daala_encoder_info_valid(void *info);
int kr_daala_encoder_info_random(void *info);
int kr_daala_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_daala_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_vpx_encoder_info_init(void *info);
int kr_vpx_encoder_info_valid(void *info);
int kr_vpx_encoder_info_random(void *info);
int kr_vpx_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_vpx_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_h264_encoder_info_init(void *info);
int kr_h264_encoder_info_valid(void *info);
int kr_h264_encoder_info_random(void *info);
int kr_h264_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_h264_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_theora_encoder_info_init(void *info);
int kr_theora_encoder_info_valid(void *info);
int kr_theora_encoder_info_random(void *info);
int kr_theora_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_theora_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_y4m_encoder_info_init(void *info);
int kr_y4m_encoder_info_valid(void *info);
int kr_y4m_encoder_info_random(void *info);
int kr_y4m_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_y4m_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_flac_info_init(void *info);
int kr_flac_info_valid(void *info);
int kr_flac_info_random(void *info);
int kr_flac_info_patch_path(kr_patch *patch, kr_path *path);
int kr_flac_info_diff(kr_diffset *diffset, void *info1, void *info2);
int str_to_kr_opus_app(char *str);
char *kr_opus_app_to_str(int val);
int kr_opus_info_init(void *info);
int kr_opus_info_valid(void *info);
int kr_opus_info_random(void *info);
int kr_opus_info_patch_path(kr_patch *patch, kr_path *path);
int kr_opus_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_vorbis_info_init(void *info);
int kr_vorbis_info_valid(void *info);
int kr_vorbis_info_random(void *info);
int kr_vorbis_info_patch_path(kr_patch *patch, kr_path *path);
int kr_vorbis_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_mp3_info_init(void *info);
int kr_mp3_info_valid(void *info);
int kr_mp3_info_random(void *info);
int kr_mp3_info_patch_path(kr_patch *patch, kr_path *path);
int kr_mp3_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_aac_info_init(void *info);
int kr_aac_info_valid(void *info);
int kr_aac_info_random(void *info);
int kr_aac_info_patch_path(kr_patch *patch, kr_path *path);
int kr_aac_info_diff(kr_diffset *diffset, void *info1, void *info2);
int kr_lavc_encoder_info_init(void *info);
int kr_lavc_encoder_info_valid(void *info);
int kr_lavc_encoder_info_random(void *info);
int kr_lavc_encoder_info_patch_path(kr_patch *patch, kr_path *path);
int kr_lavc_encoder_info_diff(kr_diffset *diffset, void *info1, void *info2);
#endif

