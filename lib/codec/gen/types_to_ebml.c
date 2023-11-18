#include "types_to_ebml.h"

#include <krad/util/util.h>
int kr_codec_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_CODEC_META_INFO, 0);
  return res;
}

int kr_audio_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AUDIO_INFO_META_INFO, 0);
  return res;
}

int kr_video_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_VIDEO_INFO_META_INFO, 0);
  return res;
}

int kr_av_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AV_INFO_META_INFO, 0);
  return res;
}

int kr_daala_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_DAALA_ENCODER_INFO_META_INFO, 0);
  return res;
}

int kr_vpx_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_VPX_ENCODER_INFO_META_INFO, 0);
  return res;
}

int kr_h264_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_H264_ENCODER_INFO_META_INFO, 0);
  return res;
}

int kr_theora_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_THEORA_ENCODER_INFO_META_INFO, 0);
  return res;
}

int kr_y4m_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_Y4M_ENCODER_INFO_META_INFO, 0);
  return res;
}

int kr_flac_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_FLAC_INFO_META_INFO, 0);
  return res;
}

int kr_opus_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_OPUS_INFO_META_INFO, 0);
  return res;
}

int kr_vorbis_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_VORBIS_INFO_META_INFO, 0);
  return res;
}

int kr_mp3_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_MP3_INFO_META_INFO, 0);
  return res;
}

int kr_aac_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_AAC_INFO_META_INFO, 0);
  return res;
}

int kr_lavc_encoder_info_pack_ebml(kr_ebml *ebml, void *info) {
  int res;
  res = info_pack_unpack_ebml(ebml, info, KR_KR_LAVC_ENCODER_INFO_META_INFO, 0);
  return res;
}

