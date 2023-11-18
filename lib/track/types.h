#if !defined(_track_types_H)
# define _track_types_H (1)

#include <krad/codec/types.h>
#include <krad/xfer/types.h>

typedef struct {
  char name[256];
  int len;
} kr_track_name;

typedef struct {
  char name[256];
  int len;
} kr_mux_name;

typedef struct {
  kr_container_type type;
  int source;
} kr_demux_info;

typedef struct {
  kr_track_name input[4];
  kr_container_type type;
  int ntracks;
} kr_mux_info;

typedef struct {
  kr_video_codec codec;
  union {
    kr_daala_encoder_info daala;
    kr_vpx_encoder_info vpx;
    kr_h264_encoder_info h264;
    kr_theora_encoder_info theora;
    kr_y4m_encoder_info y4m;
  };
} kr_video_encode_info;

typedef struct {
  kr_audio_codec codec;
  union {
    kr_flac_info flac;
    kr_opus_info opus;
    kr_vorbis_info vorbis;
    kr_mp3_info mp3;
    kr_aac_info aac;
  };
} kr_audio_encode_info;

typedef enum {
  KR_ENCODE_AUDIO = 1,
  KR_ENCODE_VIDEO,
} kr_encode_type;

typedef struct {
  kr_track_name in;
  kr_track_name out; /* opt */
  kr_encode_type type;
  union {
    kr_audio_encode_info audio;
    kr_video_encode_info video;
  };
} kr_encode_info;

typedef enum {
  KR_CAPTURE_AUDIO = 1,
  KR_CAPTURE_VIDEO,
  KR_CAPTURE_IMAGE
} kr_capture_type;

typedef struct {
  char name[256];
  int len;
} kr_record_info;

typedef struct {
  kr_xfer_transmitter_info txmtr;
  char mount[256];
} kr_transmission_info;

typedef enum {
  KR_STUDIO_RECORD = 1,
  KR_STUDIO_UPLINK,
  KR_STUDIO_TRANSMISSION
} kr_output_type;

typedef struct {
  kr_mux_name input;
  kr_output_type type;
  union {
    kr_record_info record;
    kr_uplink_info uplink;
    kr_transmission_info tx;
  };
} kr_output_info;

typedef struct {
  kr_track_name track; /* opt */
  int channels; /* 1, 2, 2 */
  int64_t total_frames; /* opt */
} kr_capture_audio_info;

typedef struct {
  kr_track_name track; /* opt */
  int width;
  int height;
  int fps_num;
  int fps_den;
  kr_pixel_fmt format;
  int64_t total_frames; /* opt */
} kr_capture_video_info;

typedef struct {
  int width;
  int height;
  kr_pixel_fmt format;
} kr_capture_image_info;

typedef struct {
  kr_capture_type type;
  union {
    kr_capture_audio_info audio;
    kr_capture_video_info video;
    kr_capture_image_info image;
  };
} kr_capture_info;

typedef struct {
  char path[256];
  int64_t size;
  uint64_t lastmod;
} kr_source_file_info;

typedef struct {
  char path[256];
} kr_source_remote_file_info;

typedef enum {
  KR_SOURCE_STREAM_ACCEPT = 1,
  KR_SOURCE_STREAM_DOWNLINK
} kr_studio_source_stream_type;

typedef struct {
  char path[256];
  int len;
  kr_studio_source_stream_type type;
  int num; /* opt */
  uint64_t bytes; /* opt */
} kr_source_stream_info;

typedef enum {
  KR_SOURCE_LOCAL_FILE = 1,
  KR_SOURCE_REMOTE_FILE,
  KR_SOURCE_STREAM
} kr_studio_source_type;

typedef struct {
  kr_studio_source_type type;
  union {
    kr_source_file_info file;
    kr_source_remote_file_info remote;
    kr_source_stream_info stream;
  };
} kr_studio_source_info;

typedef struct {
  int bongo;
} kr_decode_info;

typedef struct {
  kr_track_name input;
} kr_play_info;

typedef struct {
  char id[16];
} kr_render_image_info;

typedef struct {
  char id[16];
} kr_render_video_info;

typedef struct {
  char id[16];
} kr_render_audio_info;

typedef enum {
  KR_RENDER_IMAGE = 1,
  KR_RENDER_VIDEO,
  KR_RENDER_AUDIO
} kr_render_type;

typedef struct {
  kr_render_type type;
  union {
    kr_render_image_info image;
    kr_render_video_info video;
    kr_render_audio_info audio;
  };
} kr_render_info;

typedef enum {
  KR_GENERATE_NOISE = 1,
  KR_GENERATE_TONE,
  KR_GENERATE_SILENCE
} kr_audio_generate_type;

typedef struct {
  kr_audio_generate_type type;
  int channels;
} kr_generate_audio_info;

typedef enum {
  KR_GENERATE_COLOR = 1,
  KR_GENERATE_NO_SIGNAL
} kr_video_generate_type;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} kr_generate_color_info;

typedef struct {
  kr_video_generate_type type;
  union {
    kr_generate_color_info color;
    int with_coconut;
  };
  int width;
  int height;
  int fps_num;
  int fps_den;
  kr_pixel_fmt format;
} kr_generate_video_info;

typedef enum {
  KR_GENERATE_AUDIO = 1,
  KR_GENERATE_VIDEO
} kr_generate_type;

typedef struct {
  kr_generate_type type;
  union {
    kr_generate_audio_info audio;
    kr_generate_video_info video;
  };
} kr_generate_info;

typedef enum {
  KR_STUDIO_SOURCE = 1,
  KR_STUDIO_GENERATE,
  KR_STUDIO_CAPTURE,
  KR_STUDIO_RENDER,
  KR_STUDIO_PLAY,
  KR_STUDIO_ENCODE,
  KR_STUDIO_DECODE,
  KR_STUDIO_DEMUX,
  KR_STUDIO_MUX,
  KR_STUDIO_OUTPUT
} kr_studio_path_type;

typedef struct {
  kr_studio_path_type type;
  union {
    kr_studio_source_info source;
    kr_generate_info generate;
    kr_capture_info capture;
    kr_render_info render;
    kr_play_info play;
    kr_encode_info encode;
    kr_decode_info decode;
    kr_demux_info demux;
    kr_mux_info mux;
    kr_output_info output;
  };
} kr_studio_path_info;

typedef enum {
  KR_TRACK_CODED_AUDIO = 1,
  KR_TRACK_CODED_VIDEO,
  KR_TRACK_MUX,
  KR_TRACK_AUDIO,
  KR_TRACK_VIDEO,
  KR_TRACK_SUBTITLE,
  KR_TRACK_METADATA,
  KR_TRACK_CONTROL
} kr_track_block_type;

typedef enum {
  KR_TRACK_FINITE = 1,
  KR_TRACK_INFINITE
} kr_track_mode;

typedef enum {
  KR_TRACK_ADD = 1,
  KR_TRACK_REMOVE,
  KR_TRACK_READ,
  KR_TRACK_WRITE
} kr_track_event_type;

typedef struct {
  kr_track_name track;
  kr_codec codec;
  kr_track_mode mode;
  int64_t duration;
  kr_track_block_type type;
  union {
    kr_audio_info audio;
    kr_video_info video;
    int coconut;
    int coconut2;
    int coconut3;
  };
} kr_track_info;

typedef struct {
  uint64_t tc;
} kr_track_block_info;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
