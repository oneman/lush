#include <math.h>
#include <opus.h>
#include <opus_multistream.h>
#include <krad/mem/mem.h>
#include <krad/radio/version.h>
#include "header.h"
#include "encoder.h"

#define KR_OPUS_HEADER_SZ_MAX 512
#define KR_OPUS_NUM_STREAMS 1
#define KR_OPUS_NUM_COUPLED_STREAMS 1
#define KR_OPUS_NHEADERS 2
#define KR_OPUS_NCHANNELS 2

struct kr_opus_encoder {
  kr_opus_info info;
  uint8_t header[KR_OPUS_HEADER_SZ_MAX];
  kr_codec_header headers;
};

static OpusMSEncoder *get_ctx(kr_opus_encoder *oe);
static void gen_header(kr_opus_encoder *oe);

static OpusMSEncoder *get_ctx(kr_opus_encoder *oe) {
  off_t os;
  uint8_t *ptr;
  OpusMSEncoder *ctx;
  os = sizeof(kr_opus_encoder);
  ptr = (uint8_t *)oe;
  ptr += os;
  ctx = (OpusMSEncoder *)ptr;
  return ctx;
}

static void gen_header(kr_opus_encoder *oe) {
  OpusMSEncoder *ctx;
  OpusHeader hdr;
  int lookahead;
  uint8_t *th;
  memset(&hdr, 0, sizeof(hdr));
  ctx = get_ctx(oe);
  oe->headers.len = KR_OPUS_NHEADERS;
  hdr.channels = oe->info.channels;
  hdr.nb_streams = KR_OPUS_NUM_STREAMS;
  hdr.nb_coupled = KR_OPUS_NUM_COUPLED_STREAMS;
  opus_multistream_encoder_ctl(ctx, OPUS_GET_LOOKAHEAD(&lookahead));
  hdr.preskip = lookahead;
  hdr.gain = 0;
  hdr.input_sample_rate = oe->info.sample_rate;
  oe->headers.data[0].sz = opus_header_to_packet(&hdr, oe->header,
   sizeof(oe->header));
  oe->headers.data[0].buf = oe->header;
  oe->headers.data[1].buf = oe->header + oe->headers.data[0].sz;
  th = oe->headers.data[1].buf;
  /* the following 12 lines suck */
  oe->headers.data[1].sz = 8 + 4 + strlen(opus_get_version_string()) + 4 + 4
   + strlen ("ENCODER=") + strlen(KR_VERSION_STR_FULL);
  memcpy(th, "OpusTags", 8);
  th[8] = strlen(opus_get_version_string());
  memcpy(th + 12, opus_get_version_string(),
   strlen(opus_get_version_string()));
  th[12 + strlen(opus_get_version_string())] = 1;
  th[12 + strlen(opus_get_version_string()) + 4] =
   strlen("ENCODER=") + strlen(KR_VERSION_STR_FULL);
  memcpy(th + 12 + strlen(opus_get_version_string()) + 4 + 4, "ENCODER=",
   strlen("ENCODER="));
  memcpy(th + 12 + strlen(opus_get_version_string()) + 4 + 4
   + strlen("ENCODER="), KR_VERSION_STR_FULL, strlen(KR_VERSION_STR_FULL));
}

size_t kr_opus_encoder_sizeof() {
  size_t sz;
  sz = 0;
  sz += opus_multistream_encoder_get_size(KR_OPUS_NUM_STREAMS,
   KR_OPUS_NUM_COUPLED_STREAMS);
  sz += sizeof(kr_opus_encoder);
  printk("Opus Encoder Version: %s", opus_get_version_string());
  return sz;
}

kr_opus_encoder *kr_opus_encoder_init(kr_opus_encoder_setup *setup) {
  int ret;
  kr_opus_encoder *oe;
  OpusMSEncoder *ctx;
  int app;
  int num_streams;
  int num_coupled_streams;
  uint8_t mapping[256];
  if (!setup) return NULL;
  oe = (kr_opus_encoder *)setup->mem;
  memset(mapping, 0, sizeof(mapping));
  memset(oe, 0, sizeof(*oe));
  oe->info = setup->info;
  oe->headers.len = 0;
  if (oe->info.app == KR_OPUS_APP_LOWDELAY) {
    app = OPUS_APPLICATION_RESTRICTED_LOWDELAY;
  } else if (oe->info.app == KR_OPUS_APP_VOIP) {
    app = OPUS_APPLICATION_VOIP;
  } else {
    app = OPUS_APPLICATION_AUDIO;
    oe->info.app = KR_OPUS_APP_AUDIO;
  }
  num_streams = KR_OPUS_NUM_STREAMS;
  num_coupled_streams = KR_OPUS_NUM_COUPLED_STREAMS;
  mapping[0] = 0;
  mapping[1] = 1;
  ctx = get_ctx(oe);
  ret = opus_multistream_encoder_init(ctx, oe->info.sample_rate,
   oe->info.channels, num_streams, num_coupled_streams, mapping, app);
  if (ret < 0) {
    printke("Opus Encoder: %s", opus_strerror(ret));
    return NULL;
  }
  opus_multistream_encoder_ctl(ctx, OPUS_SET_BITRATE(oe->info.bit_rate));
  gen_header(oe);
  return oe;
}

int kr_opus_encoder_get_headers(kr_opus_encoder *oe, kr_codec_header *h) {
  if (!oe || !h) return -1;
  *h = oe->headers;
  return 0;
}

int kr_opus_encoder_get_info(kr_opus_encoder *ve,
 kr_opus_info *info) {
  if (!ve || !info) return -1;
  *info = ve->info;
  return 0;
}

int kr_opus_encode(kr_opus_encoder *oe, kr_coded_audio *out, kr_audio *in) {
  OpusMSEncoder *ctx;
  int ret;
  int off;
  int nc;
  int nf;
  float is[KR_OPUS_NCHANNELS * 2880];
  nf = oe->info.frame_size;
  nc = oe->info.channels;
  off = 0;
  if (nc != in->channels) {
    printke("Opus Encoder: channel mismatch");
    return -1;
  }
  if (nf != in->count) {
    printke("Opus Encoder: frame count mismatch");
    return -2;
  }
  kr_audio_interleave(is, in);
  ctx = get_ctx(oe);
  ret = opus_multistream_encode_float(ctx, is, nf, out->data + off,
   out->sz - off);
  if (ret < 0) {
    printke("Opus Encoder: %s", opus_strerror(ret));
    return ret;
  }
  out->channels = oe->info.channels;
  out->frames = in->count;
  out->rate = in->rate;
  out->len = ret;
  out->tc = oe->info.total_frames;
  oe->info.total_frames += in->count;
  oe->info.total_bytes += out->len;
  return 0;
}

int kr_opus_encoder_ctl(kr_opus_encoder *oe, kr_patchset *patchset) {
  if (!oe || !patchset) return -1;
  //opus_multistream_encoder_ctl(oe->ctx);
  return 0;
}

int kr_opus_encoder_destroy(kr_opus_encoder *oe) {
  if (!oe) return -1;
  return 0;
}
