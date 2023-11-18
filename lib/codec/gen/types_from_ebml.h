#if !defined(_CODEC_GEN_TYPES_FROM_EBML_H)
#define _CODEC_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_codec_unpack_ebml(kr_ebml *ebml, void *info);
int kr_audio_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_video_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_av_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_daala_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_vpx_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_h264_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_theora_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_y4m_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_flac_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_opus_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_vorbis_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_mp3_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_aac_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_lavc_encoder_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

