#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <fdk-aac/aacenc_lib.h>
#include "encoder.h"

#define KR_AAC_NCHANNELS 2

struct kr_aac_encoder {
  HANDLE_AACENCODER aac;
  kr_aac_info info;
  int16_t samples[KR_AAC_NCHANNELS * 8192];
};

size_t kr_aac_encoder_sizeof() {
  return sizeof(kr_aac_encoder);
}

int kr_aac_encoder_destroy(kr_aac_encoder *enc) {
  if (!enc) return -1;
  aacEncClose(&enc->aac);
  return 0;
}

int kr_aac_encoder_get_info(kr_aac_encoder *enc, kr_aac_info *info) {
  if (!enc || !info) return -1;
  *info = enc->info;
  return 0;
}

static int aac_bitrate_to_aot(int brate) {
  if (brate < 40000 || brate > 576000) return -1;
  if (brate >= 40000 && brate <= 64000) return 29;
  if (brate > 64000 && brate <= 128000) return 5;
  if (brate > 128000 && brate <= 576000) return 2;
  return -2;
}

kr_aac_encoder *kr_aac_encoder_init(kr_aac_encoder_setup *setup) {
  int ret;
  int aot;
  kr_aac_encoder *enc;
  CHANNEL_MODE mode;
  if (!setup || !setup->mem) return NULL;
  if (setup->info.channels != 2) return NULL;
  enc = (kr_aac_encoder *)setup->mem;
  enc->info = setup->info;
  switch (enc->info.channels) {
    case 1: mode = MODE_1;       break;
    case 2: mode = MODE_2;       break;
    case 3: mode = MODE_1_2;     break;
    case 4: mode = MODE_1_2_1;   break;
    case 5: mode = MODE_1_2_2;   break;
    case 6: mode = MODE_1_2_2_1; break;
    default:
    fprintf(stderr, "Unsupported channels number %d\n", enc->info.channels);
    return NULL;
  }
  ret = aacEncOpen(&enc->aac, 0, enc->info.channels);
  if (ret != AACENC_OK) {
    printke("kr_aac_encoder_init: fail %d", ret);
    return NULL;
  }
  aot = aac_bitrate_to_aot(enc->info.bit_rate);
  if (aot < 0) {
    printke("kr_aac: input bitrate was no good, setting a default.");
    enc->info.bit_rate = 96000;
    aot = 5;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_AOT, aot) != AACENC_OK) {
    printke("Unable to set the AOT\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_SAMPLERATE,
   enc->info.sample_rate) != AACENC_OK) {
    printke("Unable to set the AOT\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_CHANNELMODE, mode) != AACENC_OK) {
    printke("Unable to set the channel mode\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_CHANNELORDER, 1) != AACENC_OK) {
    printke("Unable to set the wav channel order\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_BITRATEMODE, 0) != AACENC_OK) {
    printke("Unable to set the bitrate mode\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_BITRATE,
   enc->info.bit_rate) != AACENC_OK) {
    printke("Unable to set the bitrate\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_TRANSMUX, 2) != AACENC_OK) {
    printke("Unable to set the ADTS transmux\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_SIGNALING_MODE, 0) != AACENC_OK) {
    printke("Unable to set the signaling mode\n");
    return NULL;
  }
  if (aacEncoder_SetParam(enc->aac, AACENC_AFTERBURNER, 1) != AACENC_OK) {
    printke("Unable to set the afterburner mode\n");
    return NULL;
  }
  return enc;
}

int kr_aac_encode(kr_aac_encoder *enc,
 kr_coded_audio *out, kr_audio *in) {
  int ret;
  AACENC_BufDesc in_buf;
  AACENC_BufDesc out_buf;
  AACENC_InArgs in_args;
  AACENC_OutArgs out_args;
  int ids[KR_AAC_NCHANNELS];
  void *bufs[KR_AAC_NCHANNELS];
  int buf_sz[KR_AAC_NCHANNELS];
  int buf_el_sz[KR_AAC_NCHANNELS];
  int out_id;
  int out_el_sz;
  memset(&in_buf, 0, sizeof(in_buf));
  memset(&out_buf, 0, sizeof(out_buf));
  memset(&in_args, 0, sizeof(in_args));
  memset(&out_args, 0, sizeof(out_args));
  if (!enc || !out || !in) return -1;
  ret = kr_audio_interleave_16bit(enc->samples, in);
  if (ret < 0) {
    printke("Audio: Interleaving failed %d", ret);
    return -2;
  }
  in_buf.numBufs = 1;
  bufs[0] = enc->samples;
  in_buf.bufs = bufs;
  ids[0] = IN_AUDIO_DATA;
  in_buf.bufferIdentifiers = ids;
  buf_sz[0] = in->count * enc->info.channels * sizeof(int16_t);
  buf_el_sz[0] = sizeof(int16_t);
  in_buf.bufSizes = buf_sz;
  in_buf.bufElSizes = buf_el_sz;
  in_args.numInSamples = in->count * enc->info.channels;
  out->tc = enc->info.total_samples;
  enc->info.total_samples += in->count;
  in_args.numAncBytes = 0;
  out_buf.numBufs = 1;
  out_id = OUT_BITSTREAM_DATA;
  out_buf.bufferIdentifiers = &out_id;
  out_buf.bufs = (void **)&out->data;
  out_buf.bufSizes = (int *)&out->sz;
  out_el_sz = sizeof(uint8_t);
  out_buf.bufElSizes = &out_el_sz;
  ret = aacEncEncode(enc->aac, &in_buf, &out_buf, &in_args, &out_args);
  if (ret != AACENC_OK) {
    printke("kr_aac_encode: fail %d", ret);
    return -2;
  }
  out->len = out_args.numOutBytes;
  out->rate = in->rate;
  enc->info.total_bytes += out->len;
  return 0;
}
