#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <FLAC/stream_decoder.h>
#include "decoder.h"

#define FLAC_FRAME_SZ 4608

struct kr_flac_decoder {
  FLAC__StreamDecoder *ctx;
  uint8_t *coded;
  size_t len;
  size_t pos;
  kr_audio *out;
  kr_flac_info info;
};

static FLAC__StreamDecoderReadStatus read_cb(const FLAC__StreamDecoder *ctx,
 FLAC__byte buf[], size_t *sz, void *user) {
  kr_flac_decoder *dec;
  dec = (kr_flac_decoder *)user;
  printkd("FLAC Decoder: READ CB %d", FLAC__stream_decoder_get_state(ctx));
  if (dec->pos == dec->len) {
    printke("FLAC Decoder: no more data to be read!");
    return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
  }
  if ((dec->len - dec->pos) < *sz) {
    *sz = dec->len - dec->pos;
  }
  memcpy(buf, dec->coded + dec->pos, *sz);
  dec->pos += *sz;
  return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

static void metadata_cb(const FLAC__StreamDecoder *ctx,
 const FLAC__StreamMetadata *md, void *user) {
  kr_flac_decoder *dec;
  dec = (kr_flac_decoder *)user;
  dec->info.channels = md->data.stream_info.channels;
  dec->info.sample_rate = md->data.stream_info.sample_rate;
  dec->info.bit_depth = md->data.stream_info.bits_per_sample;
  dec->info.frame_size = md->data.stream_info.min_blocksize;
  printk("FLAC Decoder: %dch %dhz %dbit - %d framesize",
   dec->info.channels, dec->info.sample_rate, dec->info.bit_depth,
   dec->info.frame_size);
  if (md->data.stream_info.total_samples) {
    dec->info.total_samples = md->data.stream_info.total_samples;
  }
}

static FLAC__StreamDecoderWriteStatus write_cb(const FLAC__StreamDecoder *ctx,
 const FLAC__Frame *frame, const FLAC__int32 *const buf[], void *user) {
  int c;
  int s;
  int k;
  size_t count;
  kr_flac_decoder *dec;
  dec = (kr_flac_decoder *)user;
  printkd("FLAC Decoder: WRITE CB");
  dec->info.bit_depth = frame->header.bits_per_sample;
  dec->info.sample_rate = frame->header.sample_rate;
  dec->info.channels = frame->header.channels;
  dec->out->channels = dec->info.channels;
  dec->out->rate = dec->info.sample_rate;
  count = dec->out->count;
  dec->out->count += frame->header.blocksize;
  if (dec->info.bit_depth == 16) {
    for (c = 0; c < dec->out->channels; c++) {
      k = 0;
      for (s = count; s < dec->out->count; s++) {
        dec->out->samples[c][s] = kr_audio_16bit_to_float(buf[c][k++]);
      }
    }
  } else if (dec->info.bit_depth == 24) {
    for (c = 0; c < dec->out->channels; c++) {
      k = 0;
      for (s = count; s < dec->out->count; s++) {
        dec->out->samples[c][s] = kr_audio_24bit_to_float(buf[c][k++]);
      }
    }
  } else {
    printke("FLAC Decoder: Wrong bit-depth");
    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }
  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static void error_cb(const FLAC__StreamDecoder *ctx,
 FLAC__StreamDecoderErrorStatus status, void *user) {
  printk("FLAC Decoder: %s", FLAC__StreamDecoderErrorStatusString[status]);
  return;
}

size_t kr_flac_decoder_sizeof() {
  return sizeof(kr_flac_decoder);
}

kr_flac_decoder *kr_flac_decoder_init(kr_flac_decoder_setup *setup) {
  FLAC__StreamDecoderInitStatus ret;
  kr_flac_decoder *dec;
  if (!setup || !setup->mem) return NULL;
  dec = (kr_flac_decoder *)setup->mem;
  memset(dec, 0, sizeof(kr_flac_decoder));
  dec->ctx = FLAC__stream_decoder_new();
  if (!dec->ctx) return NULL;
  ret = FLAC__stream_decoder_init_stream(dec->ctx, read_cb, NULL, NULL, NULL,
   NULL, write_cb, metadata_cb, error_cb, dec);
  if (ret != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    printke("FLAC Decoder: FLAC__stream_decoder_init_stream failed. %d", ret);
    return NULL;
  }
  return dec;
}

int kr_flac_decoder_get_info(kr_flac_decoder *dec, kr_flac_info *info) {
  if (!dec || !info) return -1;
  *info = dec->info;
  return 0;
}

int kr_flac_decode(kr_flac_decoder *dec, kr_audio *out, kr_coded_audio *in) {
  FLAC__bool ret;
  if (!dec || !in || !out) return -1;
  dec->coded = in->data;
  dec->len = in->len;
  dec->out = out;
  dec->out->count = 0;
  dec->pos = 0;
  ret = FLAC__stream_decoder_process_single(dec->ctx);
  if (!ret) {
    printke("FLAC Decoder: FLAC__stream_decoder_process_single failed");
    return -3;
  }
  printkd("FLAC Decoder: decoded %d samples!", dec->out->count);
  return 0;
}

int kr_flac_decoder_flush(kr_flac_decoder *dec, kr_audio *out) {
  FLAC__bool ret;
  if (!dec || !out) return -1;
  dec->out->count = 0;
  ret = FLAC__stream_decoder_process_single(dec->ctx);
  if (!ret) {
    printke("FLAC Decoder: FLAC__stream_decoder_process_until_end_of_stream failed");
    return -2;
  }
  printkd("FLAC Decoder: decoded %d samples!", dec->out->count);
  return dec->out->count;
}

int kr_flac_decoder_destroy(kr_flac_decoder *dec) {
  if (!dec) return -1;
  FLAC__stream_decoder_finish(dec->ctx);
  FLAC__stream_decoder_delete(dec->ctx);
  return 0;
}
