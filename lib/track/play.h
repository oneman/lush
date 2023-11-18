#include <krad/radio/version.h>
#include <krad/app/types.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include <krad/ring/ring.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/audio/resample.h>
#include <krad/codec/audio/vorbis/decoder.h>
#include <krad/codec/audio/flac/decoder.h>
#include <krad/codec/audio/opus/decoder.h>
#include <krad/codec/audio/mp3/decoder.h>
#include <krad/codec/audio/aac/decoder.h>

#include "types.h"

#if !defined(_track_play_H)
# define _track_play_H (1)

#define KR_AUDIO_PLAY_NCHANNELS 2

typedef struct {
  kr_codec codec;
  union {
    struct {
      kr_flac_decoder *flac;
      kr_opus_decoder *opus;
      kr_vorbis_decoder *vorbis;
      kr_mp3_decoder *mp3;
      kr_aac_decoder *aac;
      kr_mixer_path *mixer_port;
      kr_ringbuf *samples[KR_AUDIO_PLAY_NCHANNELS];
      kr_resampler *resampler;
    } audio;
    struct {
      kr_compositor_path *comp_port;
    } video;
  };
  kr_amem(mem, 8192 * 64)
  kr_amem(mem2, 64)
  kr_track *in;
  kr_track_head *rd;
} studio_play;

int studio_play_destroy(kr_studio_path *path);
int studio_play_create(kr_studio_path *path);

#endif
