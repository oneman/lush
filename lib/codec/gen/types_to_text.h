#if !defined(_CODEC_GEN_TYPES_TO_TEXT_H)
#define _CODEC_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_codec_to_text(char *text, void *info, int max);
int kr_audio_info_to_text(char *text, void *info, int max);
int kr_video_info_to_text(char *text, void *info, int max);
int kr_av_info_to_text(char *text, void *info, int max);
int kr_daala_encoder_info_to_text(char *text, void *info, int max);
int kr_vpx_encoder_info_to_text(char *text, void *info, int max);
int kr_h264_encoder_info_to_text(char *text, void *info, int max);
int kr_theora_encoder_info_to_text(char *text, void *info, int max);
int kr_y4m_encoder_info_to_text(char *text, void *info, int max);
int kr_flac_info_to_text(char *text, void *info, int max);
int kr_opus_info_to_text(char *text, void *info, int max);
int kr_vorbis_info_to_text(char *text, void *info, int max);
int kr_mp3_info_to_text(char *text, void *info, int max);
int kr_aac_info_to_text(char *text, void *info, int max);
int kr_lavc_encoder_info_to_text(char *text, void *info, int max);
#endif

