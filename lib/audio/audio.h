#include <inttypes.h>
#include <sys/types.h>

#include "convert.h"
#include "types.h"

#if !defined(_audio_audio_H)
# define _audio_audio_H (1)

typedef struct {
  float *samples[KR_AUDIO_NCHANNELS];
  uint32_t channels;
  int16_t count;
  int16_t zc[KR_AUDIO_NCHANNELS];
  int32_t rate;
  uint64_t stc;
  void *owner;
} kr_audio;

typedef struct {
  uint8_t *data;
  size_t sz;
  size_t len;
  int64_t tc;
  int16_t frames;
  int8_t channels;
  int32_t rate;
} kr_coded_audio;

typedef struct {
  kr_audio *audio;
  uint8_t *mem;
  uint32_t channels;
  int32_t rate;
  int16_t count;
} kr_audio_setup;

float kr_lintodb(float val);
float kr_dbtolin(float db);
int kr_audio_zero(kr_audio *a);
int kr_audio_init(kr_audio_setup *setup);

#endif
