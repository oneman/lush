#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <lame/lame.h>
#include "decoder.h"

struct kr_mp3_decoder {
  short pcm[2][8192 * 2];
  hip_t dec;
  mp3data_struct mp3_data;
  kr_mp3_info info;
};

size_t kr_mp3_decoder_sizeof() {
  return sizeof(kr_mp3_decoder);
}

kr_mp3_decoder *kr_mp3_decoder_init(kr_mp3_decoder_setup *setup) {
  kr_mp3_decoder *d;
  if (!setup || !setup->mem) return NULL;
  d = (kr_mp3_decoder *)setup->mem;
  memset(d, 0, sizeof(kr_mp3_decoder));
  d->dec = hip_decode_init();
  if (!d->dec) {
    printke("MP3 Decoder: hip_decode_init failed!");
    return NULL;
  }
  return d;
}

int kr_mp3_decoder_get_info(kr_mp3_decoder *dec, kr_mp3_info *info) {
  if (!dec || !info) return -1;
  *info = dec->info;
  return 0;
}

int kr_mp3_decode(kr_mp3_decoder *dec, kr_audio *out, kr_coded_audio *in) {
  int i;
  int ret;
  if (!dec || !out || !in) return -1;
  if (in->channels != 2) return -2;
  ret = hip_decode_headers(dec->dec, in->data, in->len,
   dec->pcm[0], dec->pcm[1], &dec->mp3_data);
  if (ret < 0) {
    printke("MP3 Decoder: decoding failed %d", ret);
    return -3;
  }
  if (ret == 0) printkd("MP3 Decoder: need more data");
  if (ret > 0) printkd("MP3 Decoder: decoded %d samples!", ret);
  if (dec->mp3_data.samplerate && dec->mp3_data.samplerate < 44100) return -4;
  if (dec->mp3_data.samplerate) dec->info.sample_rate = dec->mp3_data.samplerate;
  dec->info.channels = 2;
  out->channels = in->channels;
  out->rate = dec->info.sample_rate;
  out->count = ret;
  for (i = 0; i < out->count; i++) {
    out->samples[0][i] = kr_audio_16bit_to_float(dec->pcm[0][i]);
    out->samples[1][i] = kr_audio_16bit_to_float(dec->pcm[1][i]);
  }
  dec->info.total_samples += ret;
  return 0;
}

int kr_mp3_decoder_destroy(kr_mp3_decoder *dec) {
  if (!dec) return -1;
  hip_decode_exit(dec->dec);
  return 0;
}
