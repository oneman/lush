#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <lame/lame.h>
#include "encoder.h"

struct kr_mp3_encoder {
  lame_global_flags *flags;
  kr_mp3_info info;
};

size_t kr_mp3_encoder_sizeof() {
  return sizeof(kr_mp3_encoder);
}

int kr_mp3_encoder_destroy(kr_mp3_encoder *enc) {
  if (!enc) return -1;
  lame_close(enc->flags);
  return 0;
}

int kr_mp3_encoder_get_info(kr_mp3_encoder *enc, kr_mp3_info *info) {
  if (!enc || !info) return -1;
  *info = enc->info;
  return 0;
}

kr_mp3_encoder *kr_mp3_encoder_init(kr_mp3_encoder_setup *setup) {
  int ret;
  kr_mp3_encoder *enc;
  if (!setup || !setup->mem) return NULL;
  if (setup->info.channels != 2) return NULL;
  enc = (kr_mp3_encoder *)setup->mem;
  memset(enc, 0, sizeof(*enc));
  enc->info = setup->info;
  enc->flags = lame_init();
  lame_set_brate(enc->flags, setup->info.bit_rate / 1000);
  //lame_set_quality(enc->flags, 7);
  lame_set_num_channels(enc->flags, setup->info.channels);
  lame_set_mode(enc->flags, STEREO);
  lame_set_in_samplerate(enc->flags, setup->info.sample_rate);
  lame_set_out_samplerate(enc->flags, setup->info.sample_rate);
  ret = lame_init_params(enc->flags);
  if (ret != 0) {
    printke("kr_mp3_encoder_init: fail");
    return NULL;
  }
  return enc;
}

int kr_mp3_encode(kr_mp3_encoder *enc,
 kr_coded_audio *out, kr_audio *in) {
  int ret;
  if (!enc || !out || !in) return -1;
  ret = lame_encode_buffer_ieee_float(enc->flags,
   in->samples[0], in->samples[1], in->count, out->data, out->sz);
  if (ret < 0) {
    printke("kr_mp3_encode: fail %d", ret);
    return -2;
  }
  out->len = ret;
  out->channels = enc->info.channels;
  out->rate = enc->info.sample_rate;
  out->tc = enc->info.total_frames;
  enc->info.total_frames += in->count;
  enc->info.total_bytes += out->len;
  out->frames = in->count;
  return 0;
}
