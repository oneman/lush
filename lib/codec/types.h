#if !defined(_codec_types_H)
# define _codec_types_H (1)

#include <krad/image/types.h>

typedef enum {
  KR_VIDEO_CODEC_DAALA = 1,
  KR_VIDEO_CODEC_VPX,
  KR_VIDEO_CODEC_H264,
  KR_VIDEO_CODEC_THEORA,
  KR_VIDEO_CODEC_Y4M
} kr_video_codec;

typedef enum {
  KR_AUDIO_CODEC_FLAC = 1,
  KR_AUDIO_CODEC_OPUS,
  KR_AUDIO_CODEC_VORBIS,
  KR_AUDIO_CODEC_MP3,
  KR_AUDIO_CODEC_AAC
} kr_audio_codec;

typedef enum {
  KR_CODEC_AUDIO = 1,
  KR_CODEC_VIDEO
} kr_codec_type;

typedef struct {
  kr_codec_type type;
  union {
    kr_audio_codec audio;
    kr_video_codec video;
  };
} kr_codec;

typedef struct {
  int channels;
  int bit_depth;
  int sample_rate;
  int samples;
} kr_audio_info;

typedef struct {
  int width;
  int height;
  int fps_num;
  int fps_den;
  kr_pixel_fmt pixel_fmt;
} kr_video_info;

typedef struct {
  kr_codec_type type;
  union {
    kr_audio_info audio;
    kr_video_info video;
  };
} kr_av_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt pixel_fmt;
  int fps_num;
  int fps_den;
  int bitrate;
  int key_rate;
} kr_daala_encoder_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt pixel_fmt;
  int fps_num;
  int fps_den;
  int bit_rate;
  int key_rate;
} kr_vpx_encoder_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt pixel_fmt;
  int fps_num;
  int fps_den;
  int bit_rate;
  int key_rate;
} kr_h264_encoder_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt pixel_fmt;
  int fps_num;
  int fps_den;
  int key_rate;
  uint8_t quality;
  uint8_t eos;
} kr_theora_encoder_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt pixel_fmt;
  int fps_num;
  int fps_den;
} kr_y4m_encoder_info;

typedef struct {
  int channels;
  int bit_depth;
  int sample_rate;
  int frame_size; /* opt */
  int64_t total_frames; /* opt */
  int64_t total_bytes; /* opt */
  uint64_t total_samples; /* opt */
} kr_flac_info;

typedef enum {
  KR_OPUS_APP_AUDIO = 1,
  KR_OPUS_APP_LOWDELAY,
  KR_OPUS_APP_VOIP
} kr_opus_app;

typedef struct {
  int channels;
  int sample_rate;
  int bit_rate;
  int frame_size;
  kr_opus_app app; /* opt */
  int64_t total_frames; /* opt */
  int64_t total_bytes; /* opt */
} kr_opus_info;

typedef struct {
  int channels;
  int sample_rate;
  float quality;
  int small_block_sz; /* opt */
  int large_block_sz; /* opt */
  int64_t total_frames; /* opt */
  int64_t total_bytes;  /* opt */
} kr_vorbis_info;

typedef struct {
  int channels;
  int sample_rate;
  int bit_rate;
  int64_t total_frames; /* opt */
  int64_t total_bytes; /* opt */
  uint64_t total_samples; /* opt */
} kr_mp3_info;

typedef struct {
  uint8_t channels;
  uint32_t sample_rate;
  uint32_t bit_rate; /* 96000, 40000, 576000 */
  uint64_t total_samples; /* opt */
  uint64_t total_bytes; /* opt */
} kr_aac_info;

typedef struct {
  int bongo;
} kr_lavc_encoder_info;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
