#include <krad/radio/version.h>
#include <krad/app/types.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include <krad/ring/ring.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/codec/video/y4m/encoder.h>
#include <krad/codec/video/h264/encoder.h>
#include <krad/codec/video/theora/encoder.h>
#include <krad/codec/video/vpx/encoder.h>
#include <krad/codec/video/daala/encoder.h>
#include <krad/codec/audio/vorbis/encoder.h>
#include <krad/codec/audio/flac/encoder.h>
#include <krad/codec/audio/opus/encoder.h>
#include <krad/codec/audio/mp3/encoder.h>
#include <krad/codec/audio/aac/encoder.h>

#include "types.h"

#if !defined(_track_encode_H)
# define _track_encode_H (1)

#define KR_ENCODE_NCHANNELS 2

typedef struct {
  union {
    kr_daala_encoder *daala;
    kr_vpx_encoder *vpx;
    kr_h264_encoder *h264;
    kr_theora_encoder *theora;
    kr_y4m_encoder *y4m;
    kr_flac_encoder *flac;
    kr_opus_encoder *opus;
    kr_vorbis_encoder *vorbis;
    kr_mp3_encoder *mp3;
    kr_aac_encoder *aac;
  };
  kr_amem(mem, 256000)
  kr_ringbuf *audio[KR_ENCODE_NCHANNELS];
  float samples[KR_ENCODE_NCHANNELS][4096];
  kr_track *in;
  kr_track_head *rd;
  kr_track *out;
  kr_track_head *wr;
  kr_pool *coded;
  uint64_t frames;
} studio_encode;

int studio_encode_destroy(kr_studio_path *path);
int studio_encode_create(kr_studio_path *path);

#endif
