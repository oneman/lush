#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <fdk-aac/aacdecoder_lib.h>
#include "decoder.h"

struct kr_aac_decoder {
  HANDLE_AACDECODER dec;
  kr_aac_info info;
  int16_t samples[8192 * 2 * 8];
};

size_t kr_aac_decoder_sizeof() {
  return sizeof(kr_aac_decoder);
}

kr_aac_decoder *kr_aac_decoder_init(kr_aac_decoder_setup *setup) {
  kr_aac_decoder *dec;
  if (!setup || !setup->mem) return NULL;
  dec = (kr_aac_decoder *)setup->mem;
  memset(dec, 0, sizeof(kr_aac_decoder));
  dec->dec = aacDecoder_Open(TT_MP4_ADTS, 1);
  if (!dec->dec) return NULL;
  /*fixme*/
  //aacDecoder_SetParam(dec->dec, AAC_PCM_OUTPUT_INTERLEAVED, 1);
  return dec;
}

int kr_aac_decoder_get_info(kr_aac_decoder *dec, kr_aac_info *info) {
  if (!dec || !info) return -1;
  *info = dec->info;
  return 0;
}

int kr_aac_decode(kr_aac_decoder *dec, kr_audio *out, kr_coded_audio *in) {
  int ret;
  uint bytes;
  uint len;
  uint8_t *data;
  size_t samples;
  int16_t *samples_ptr;
  size_t samples_sz;
  CStreamInfo *info;
  if (!dec || !out || !in) return -1;
  samples = 0;
  bytes = in->len;
  data = in->data;
  len = in->len;
  samples_ptr = dec->samples;
  samples_sz = sizeof(dec->samples);
  info = NULL;
  for (;;) {
    ret = aacDecoder_Fill(dec->dec, &data, &len, &bytes);
    if (ret != 0) {
      printke("AAC Decoder: aacDecoder_Fill failed %d", ret);
      return -2;
    }
    ret = aacDecoder_DecodeFrame(dec->dec, samples_ptr, samples_sz, 0);
    if (ret == AAC_DEC_NOT_ENOUGH_BITS) {
      printkd("AAC Decoder: not enough data %u", bytes);
      break;
    }
    if (ret != 0) {
      printke("AAC Decoder: aacDecoder_DecodeFrame failed %d", ret);
      return -3;
    }
    info = aacDecoder_GetStreamInfo(dec->dec);
    if (!info) return -4;
    dec->info.sample_rate = info->sampleRate;
    dec->info.channels = info->numChannels;
    samples += info->frameSize;
    if (bytes == 0) break;
    if (samples_sz <= (info->frameSize * info->numChannels * sizeof(int16_t)) ) {
      printke("AAC Decoder: Not enough space in buffer");
      return -5;
    }
    samples_ptr += (info->frameSize * info->numChannels);
    samples_sz -= (info->frameSize * info->numChannels * sizeof(int16_t));
    data = in->data + in->len - bytes;
    len = bytes;
  }
  printkd("AAC Decoder: decoded %d samples!", samples);
  out->rate = dec->info.sample_rate;
  out->channels = dec->info.channels;
  out->count = samples;
  kr_audio_deinterleave_16bit(out, dec->samples);
  return 0;
}

int kr_aac_decoder_destroy(kr_aac_decoder *dec) {
  if (!dec) return -1;
  aacDecoder_Close(dec->dec);
  return 0;
}
