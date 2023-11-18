#include <math.h>
#include <FLAC/stream_encoder.h>
#include <FLAC/stream_decoder.h>
#include <krad/mem/mem.h>
#include "encoder.h"

#define KR_FLAC_MARKER "fLaC"
#define KR_FLAC_MARKER_SZ 4
#define KR_FLAC_STREAMINFO_SZ 38
#define KR_FLAC_MIN_HEADER_SZ KR_FLAC_MARKER_SZ + KR_FLAC_STREAMINFO_SZ

#define KR_FLAC_HEADERS_MAX 1
#define KR_FLAC_NCHANNELS 2

struct kr_flac_encoder {
  FLAC__StreamEncoder *ctx;
  kr_flac_info info;
  uint8_t *out;
  size_t *out_sz;
  size_t *out_len;
  FLAC__int32 samples[KR_FLAC_NCHANNELS][8192 * 2];
  uint8_t min_header[KR_FLAC_MIN_HEADER_SZ];
  kr_codec_header headers;
};

static FLAC__StreamEncoderWriteStatus fe_write(const FLAC__StreamEncoder *ctx,
 const FLAC__byte buffer[], size_t len, unsigned ns, unsigned frame_num,
 void *user) {
  kr_flac_encoder *fe;
  fe = (kr_flac_encoder *)user;
  if (fe->headers.len == 0 && ns == 0 && len == KR_FLAC_STREAMINFO_SZ) {
    memcpy(fe->min_header, KR_FLAC_MARKER, KR_FLAC_MARKER_SZ);
    memcpy(fe->min_header + KR_FLAC_MARKER_SZ, buffer, KR_FLAC_STREAMINFO_SZ);
    fe->min_header[KR_FLAC_MARKER_SZ] = '\x80';
    fe->headers.data[0].buf = fe->min_header;
    fe->headers.data[0].sz = KR_FLAC_MIN_HEADER_SZ;
    fe->headers.len = 1;
    printk("FLAC: Encoder got header");
    return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
  }
  if (fe->info.total_frames == 0 && ns == 0) {
    printk("FLAC Encoder: skipping extra headers..");
    return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
  }
  if (*fe->out_len + len > *fe->out_sz) {
    printke("FLAC Encoder: not enough space in output buffer %zu vs %zu",
     *fe->out_len + len, *fe->out_sz);
    return FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;
  }
  fe->info.total_bytes += len;
  fe->info.total_frames += ns;
  memcpy(fe->out + *fe->out_len, buffer, len);
  *fe->out_len += len;
  return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

size_t kr_flac_encoder_sizeof() {
  return sizeof(kr_flac_encoder);
}

kr_flac_encoder *kr_flac_encoder_init(kr_flac_encoder_setup *setup) {
  FLAC__StreamEncoderInitStatus ret;
  kr_flac_encoder *fe;
  kr_flac_info *i;
  if (!setup) return NULL;
  fe = (kr_flac_encoder *)setup->mem;
  memset(fe, 0, sizeof(*fe));
  fe->info = setup->info;
  i = &fe->info;
  fe->headers.len = 0;
  fe->ctx = FLAC__stream_encoder_new();
  FLAC__stream_encoder_set_channels(fe->ctx, i->channels);
  FLAC__stream_encoder_set_bits_per_sample(fe->ctx, i->bit_depth);
  FLAC__stream_encoder_set_sample_rate(fe->ctx, i->sample_rate);
  FLAC__stream_encoder_set_compression_level(fe->ctx, 7);
  FLAC__stream_encoder_set_streamable_subset(fe->ctx, 1);
  if (i->frame_size > 0) FLAC__stream_encoder_set_blocksize(fe->ctx, i->frame_size);
  ret = FLAC__stream_encoder_init_stream(fe->ctx, fe_write, NULL, NULL, NULL,
   fe);
  if (ret != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
    failfast("FLAC Encoder: failure on init");
  }
  i->frame_size = FLAC__stream_encoder_get_blocksize(fe->ctx);
  return fe;
}

int kr_flac_encoder_get_headers(kr_flac_encoder *fe, kr_codec_header *h) {
  if (!fe || !h) return -1;
  *h = fe->headers;
  return 0;
}

int kr_flac_encoder_get_info(kr_flac_encoder *fe,
 kr_flac_info *info) {
  if (!fe || !info) return -1;
  *info = fe->info;
  return 0;
}

int kr_flac_encode(kr_flac_encoder *fe, kr_coded_audio *out, kr_audio *in) {
  const FLAC__int32 *samples[KR_FLAC_NCHANNELS];
  FLAC__bool ret;
  int c;
  int s;
  if (!fe || !out || !in) return -1;
  ret = -1;
  for (c = 0; c < fe->info.channels; c++) samples[c] = fe->samples[c];
  if (fe->info.bit_depth == 24) {
    for (c = 0; c < fe->info.channels; c++) {
      for (s = 0; s < in->count; s++) {
        fe->samples[c][s] = kr_audio_float_to_24bit(in->samples[c][s]);
      }
    }
  } else {
    for (c = 0; c < fe->info.channels; c++) {
      for (s = 0; s < in->count; s++) {
        fe->samples[c][s] = kr_audio_float_to_16bit(in->samples[c][s]);
      }
    }
  }
  fe->out = out->data;
  fe->out_sz = &out->sz;
  fe->out_len = &out->len;
  *fe->out_len = 0;
  ret = FLAC__stream_encoder_process(fe->ctx,
   samples, in->count);
  out->rate = in->rate;
  out->frames = in->count;
  out->tc = fe->info.total_frames;
  fe->info.total_frames += in->count;
  if (!ret) {
    printke("FLAC Encoder: error in process");
  }
  return 0;
}

int kr_flac_encoder_ctl(kr_flac_encoder *fe, kr_patchset *patchset) {
  if (!fe || !patchset) return -1;
  return 0;
}

int kr_flac_encoder_destroy(kr_flac_encoder *fe) {
  if (!fe) return -1;
  FLAC__stream_encoder_finish(fe->ctx);
  FLAC__stream_encoder_delete(fe->ctx);
  return 0;
}
