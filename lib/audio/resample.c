#include <stdlib.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <samplerate.h>
#include <krad/ring/ring.h>
#include "resample.h"

#define KR_RESAMPLER_SRC_QUALITY SRC_LINEAR
#define KR_RESAMPLER_NCHANNELS 2

struct kr_resampler {
  int channels;
  int64_t speed;
	SRC_STATE *src[KR_RESAMPLER_NCHANNELS];
};

size_t kr_resampler_sizeof() {
  return sizeof(kr_resampler);
}

int kr_resampler_reset(kr_resampler *r) {
  int c;
  for (c = 0; c < r->channels;) {
    src_reset(r->src[c]);
  }
  if (!r) return -1;
  return 0;
}

int kr_resampler_process(kr_resampler *r, kr_audio *out, kr_audio *in) {
  int c;
  int err;
  SRC_DATA data;
  if (!r || !out || !in) return -1;
  data.end_of_input = 0;
  if (in->rate < 4000 || in->rate > 192000) {
    printke("Resampler: bad sample input rate %d", in->rate);
    return -2;
  }
  data.src_ratio = (double)48000 / (double)in->rate;
  for (c = 0; c < r->channels; c++) {
    data.input_frames = in->count;
    data.output_frames = out->count;
    data.data_in = in->samples[c];
    data.data_out = out->samples[c];
    err = src_process(r->src[c], &data);
    if (err != 0) {
      failfast("Resampler: %s", src_strerror(err));
    }
    if (data.input_frames_used != in->count) {
      printke("Resampler: shit didn't use all the input frames");
    }
    printkd("%d of %d used to generate %d frames (%d -> 48000)",
     data.input_frames_used, in->count, data.output_frames_gen,
     in->rate);
    out->count = data.output_frames_gen;
  }
  return 0;
}

int kr_resampler_destroy(kr_resampler *r) {
  int c;
  if (!r) return -1;
  for (c = 0; c < r->channels; c++) {
    src_delete(r->src[c]);
  }
  return 0;
}

kr_resampler *kr_resampler_init(kr_resampler_setup *setup) {
  kr_resampler *r;
  int c;
  int err;
  if (!setup || !setup->mem) return NULL;
  if (setup->channels < 1 || setup->channels > KR_RESAMPLER_NCHANNELS) {
    printke("Resampler: bad channel count %d", setup->channels);
    return NULL;
  }
  r = (kr_resampler *)setup->mem;
  r->channels = setup->channels;
  for (c = 0; c < r->channels; c++) {
    r->src[c] = src_new(KR_RESAMPLER_SRC_QUALITY, 1, &err);
    if (r->src[c] == NULL) {
      failfast("Resampler: %s", src_strerror(err));
    }
  }
  return r;
}
