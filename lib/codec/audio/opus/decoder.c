#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <opus/opus.h>
#include <opus/opus_multistream.h>
#include "decoder.h"

struct kr_opus_decoder {
  float samples[8192 * 8];
  kr_opus_info info;
};

size_t kr_opus_decoder_sizeof() {
  size_t sz;
  sz = sizeof(kr_opus_decoder);
  sz += opus_multistream_decoder_get_size(1, 1);
  return sz;
}

static OpusMSDecoder *get_ctx(kr_opus_decoder *dec) {
  off_t os;
  uint8_t *ptr;
  OpusMSDecoder *ctx;
  os = sizeof(kr_opus_decoder);
  ptr = (uint8_t *)dec;
  ptr += os;
  ctx = (OpusMSDecoder *)ptr;
  return ctx;
}

kr_opus_decoder *kr_opus_decoder_init(kr_opus_decoder_setup *setup) {
  int ret;
  kr_opus_decoder *dec;
  OpusMSDecoder *ctx;
  uint8_t mapping[256];
  if (!setup || !setup->mem) return NULL;
  dec = (kr_opus_decoder *)setup->mem;
  memset(dec, 0, kr_opus_decoder_sizeof());
  ctx = get_ctx(dec);
  memset(mapping, 0, sizeof(mapping));
  mapping[0] = 0;
  mapping[1] = 1;
  ret = opus_multistream_decoder_init(ctx, setup->info.sample_rate,
   2, 1, 1, mapping);
  if (ret != OPUS_OK) {
    printke("Opus Decoder: %s", opus_strerror(ret));
    return NULL;
  }
  return dec;
}

int kr_opus_decoder_get_info(kr_opus_decoder *dec, kr_opus_info *info) {
  if (!dec || !info) return -1;
  *info = dec->info;
  return 0;
}

int kr_opus_decode(kr_opus_decoder *dec, kr_audio *out, kr_coded_audio *in) {
  int ret;
  OpusMSDecoder *ctx;
  if (!dec || !out || !in) return -1;
  ctx = get_ctx(dec);
  ret = opus_multistream_decode_float(ctx, in->data, in->len, dec->samples,
   sizeof(dec->samples) / 8, 0);
  if (ret == OPUS_BUFFER_TOO_SMALL) return 0;
  if (ret < 0) {
    printke("Opus Decoder: %s", opus_strerror(ret));
    return -2;
  }
  printkd("Opus Decoder: decoded %d samples", ret);
  out->channels = in->channels;
  out->count = ret;
  out->rate = 48000;
  kr_audio_deinterleave(out, dec->samples);
  return 0;
}

int kr_opus_decoder_destroy(kr_opus_decoder *dec) {
  if (!dec) return -1;
  return 0;
}
