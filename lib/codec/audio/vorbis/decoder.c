#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include <vorbis/vorbisenc.h>
#include "decoder.h"

#define VORBIS_MAX_CHANNELS 8

struct kr_vorbis_decoder {
  kr_vorbis_info info;
  vorbis_dsp_state vdsp;
  vorbis_block vblock;
  vorbis_info vinfo;
  vorbis_comment vc;
  uint64_t pkts;
};

static void hdr_error(int err, int64_t packetno);

static void hdr_error(int err, int64_t packetno) {
  if (err == OV_ENOTVORBIS) {
    printke("Vorbis Decoder init: Not a vorbis on packet %"PRIi64"", packetno);
  }
  if (err == OV_EBADHEADER) {
    printke("Vorbis Decoder init: Bad header on packet %"PRIi64"", packetno);
  }
  if (err == OV_EFAULT) {
    printke("Vorbis Decoder init: fault on packet %"PRIi64"", packetno);
  }
}

size_t kr_vorbis_decoder_sizeof() {
  size_t sz;
  sz = sizeof(kr_vorbis_decoder);
  return sz;
}

kr_vorbis_decoder *kr_vorbis_decoder_init(kr_vorbis_decoder_setup *setup) {
  int i;
  int ret;
  ogg_packet op;
  kr_vorbis_decoder *dec;
  if (!setup || !setup->mem || setup->header.len != 3) return NULL;
  dec = (kr_vorbis_decoder *)setup->mem;
  memset(dec, 0, kr_vorbis_decoder_sizeof());
  vorbis_info_init(&dec->vinfo);
  vorbis_comment_init(&dec->vc);
  op.b_o_s = 1;
  op.packetno = 0;
  for (i = 0; i < 3; i++) {
    op.packetno = i;
    op.packet = setup->header.data[i].buf;
    op.bytes = setup->header.data[i].sz;
    ret = vorbis_synthesis_headerin(&dec->vinfo, &dec->vc, &op);
    if (ret != 0) {
      hdr_error(ret, op.packetno);
      return NULL;
    }
    op.b_o_s = 0;
  }
  ret = vorbis_synthesis_init(&dec->vdsp, &dec->vinfo);
  if (ret != 0) {
    printke("Vorbis Decoder: synthesis init fails!");
    return NULL;
  }
  vorbis_block_init(&dec->vdsp, &dec->vblock);
  printk("Vorbis Decoder init: Version: %d Channels: %d Sample Rate: %ld",
   dec->vinfo.version, dec->vinfo.channels, dec->vinfo.rate);
  if ((dec->vinfo.bitrate_lower == 0 || dec->vinfo.bitrate_lower == 4294967295U)
   && (dec->vinfo.bitrate_upper == 0 || dec->vinfo.bitrate_upper == 4294967295U)) {
   printk("Vorbis Decoder: Nominal Bitrate: %ld", dec->vinfo.bitrate_nominal);
  } else {
    printk("Vorbis Decoder: Bitrate: Low: %ld Nominal: %ld High: %ld",
     dec->vinfo.bitrate_lower, dec->vinfo.bitrate_nominal,
     dec->vinfo.bitrate_upper);
  }
  dec->info.channels = dec->vinfo.channels;
  dec->info.sample_rate = dec->vinfo.rate;
  return dec;
}

int kr_vorbis_decoder_get_info(kr_vorbis_decoder *dec, kr_vorbis_info *info) {
  if (!dec || !info) return -1;
  *info = dec->info;
  return 0;
}

int kr_vorbis_decode(kr_vorbis_decoder *dec, kr_audio *out, kr_coded_audio *in) {
  int ret;
  int32_t c;
  int32_t sample_count;
  float **pcm;
  ogg_packet op;
  memset(&op, 0, sizeof(op));
  op.packet = in->data;
  op.bytes = in->len;
  op.packetno = dec->pkts;
  op.granulepos = in->tc;
  if (!dec || !out || !in) return -1;
  printk("Vorbis Decoder: packet size %zu", in->len);
  ret = vorbis_synthesis(&dec->vblock, &op);
  if (ret != 0) {
    printke("Vorbis Decoder: vorbis_synthesis fail");
    if (ret == OV_ENOTAUDIO) {
    printke("Vorbis Decoder: Not audio packet %"PRIi64" Gpos %"PRIi64""
     " packet size: %zu", op.packetno, op.granulepos, in->len);
    }
    if (ret == OV_EBADPACKET) {
      printke("Vorbis Decoder: Bad packet %"PRIi64" Gpos %"PRIi64""
       " packet size: %zu", op.packetno, op.granulepos, in->len);
    }
    return -1;
  }
  ret = vorbis_synthesis_blockin(&dec->vdsp, &dec->vblock);
  if (ret != 0) {
    printke("Vorbis Decoder: vorbis_synthesis_blockin fail");
    if (ret == OV_EINVAL) {
      printke("Vorbis Decoder: not ready for blockin!");
    }
    return -1;
  }
  sample_count = vorbis_synthesis_pcmout(&dec->vdsp, &pcm);
  if (sample_count > 0) {
    dec->info.total_frames += sample_count;
    printk("Vorbis Decoder: %d samples total %"PRIu64"", sample_count,
     dec->info.total_frames);
    out->channels = dec->info.channels;
    out->count = sample_count;
    for (c = 0; c < dec->info.channels; c++) {
      memcpy(out->samples[c], pcm[c], out->count * sizeof(float));
    }
    ret = vorbis_synthesis_read(&dec->vdsp, sample_count);
    if (ret != 0) {
      printke("Vorbis Decoder: vorbis_synthesis_read fail");
      if (ret == OV_EINVAL) {
        printke("Vorbis Decoder: synth read > buffer sz!");
      }
      return -1;
    }
  }
  printk("Vorbis Decoder: decoded %d samples", ret);
  dec->pkts++;
  return 0;
}

int kr_vorbis_decoder_destroy(kr_vorbis_decoder *dec) {
  if (!dec) return -1;
  vorbis_block_clear(&dec->vblock);
  vorbis_comment_clear(&dec->vc);
  vorbis_info_clear(&dec->vinfo);
  return 0;
}
