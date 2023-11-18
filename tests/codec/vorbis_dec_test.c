#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <krad/app/debug.h>
#include <krad/audio/audio.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
#include <krad/audio/noise.h>
#include <krad/container/wave/pack.h>
#include <krad/container/ogg/unpack.h>
#include <krad/codec/audio/vorbis/decoder.h>

static int vorbis_dec_test(char *in, char *out) {
  uint8_t buf[512];
  uint8_t hdr[512];
  uint8_t vorbis_hdr[8192 * 6];
  ssize_t ret;
  uint8_t buffer[8192 * 8];
  size_t buffer_len;
  size_t bytes;
  size_t len;
  int ifd;
  int ofd;
  size_t npkts;
  kr_ogg_unpacker_setup osetup;
  kr_ogg_unpacker *u;
  kr_codec_packet pkt;
  kr_ogg_pkt_info opkt_info;
  kr_vorbis_decoder *dec;
  kr_vorbis_decoder_setup setup;
  kr_coded_audio coded;
  kr_audio raw;
  float *data;
  kr_wave_info winfo;
  ifd = open(in, O_RDONLY);
  if (ifd < 0) return -1;
  ofd = open(out, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (ofd < 0) return -2;
  memset(&setup, 0, sizeof(setup));
  setup.mem = malloc(kr_vorbis_decoder_sizeof());
  dec = NULL;
  pkt.sz = 8192 * 2;
  pkt.buf = kr_alloc(pkt.sz);
  pkt.len = 0;
  osetup.mem = kr_alloc(kr_ogg_unpacker_sizeof());
  u = kr_ogg_unpacker_init(&osetup);
  if (!u) return -4;
  memset(&raw, 0, sizeof(raw));
  memset(&coded, 0, sizeof(coded));
  memset(&winfo, 0, sizeof(winfo));
  raw.samples[0] = malloc(sizeof(float) * 8192 * 8);
  raw.samples[1] = malloc(sizeof(float) * 8192 * 8);
  data = malloc(8192 * sizeof(float) * 8);
  winfo.fmt = KR_WAVE_IEEE_FLOAT;
  winfo.audio.channels = 2;
  winfo.audio.sample_rate = 44100;
  winfo.audio.bit_depth = 32;
  winfo.extensible = 0;
  ret = kr_wave_pack_header(hdr, sizeof(hdr), &winfo);
  if (ret <= 0) return -6;
  ret = write(ofd, hdr, ret);
  if (ret < 0) return -7;
  kr_ogg_unpacker_output_set(u, &pkt, 1);
  buffer_len = 0;
  npkts = 0;
  for (;;) {
    ret = read(ifd, buf, sizeof(buf));
    if (ret < 0) return -4;
    if (ret == 0) break;
    kr_ogg_unpacker_resync(u);
    bytes = ret;
    memcpy(buffer + buffer_len, buf, bytes);
    buffer_len += bytes;
    ret = kr_ogg_valid(buffer, buffer_len);
    if (ret < 0) {
      printkd("not valid stuff, returning");
      continue;
    }
    len = buffer_len;
    for (;;) {
      ret = kr_ogg_unpack(u, &opkt_info, buffer, len);
      if (ret < 0) {
        printke("Ogg unpack failed! %d", ret);
        return -9;
      }
      if (ret == 1) {
        printk("OGG: end of buffer");
        break;
      }
      if (opkt_info.truncated) {
        printk("truncated");
        memmove(buffer, buffer + len - opkt_info.truncated,
         opkt_info.truncated);
        buffer_len = opkt_info.truncated;
        break;
      }
      buffer_len = 0;
      printk("Demux OGG pkt %zu of size %zu", npkts++, pkt.len);
      if (npkts <= 3) {
        setup.header.data[npkts - 1].buf = vorbis_hdr + 8192 * 2 * (npkts - 1);
        setup.header.data[npkts - 1].sz = 8192 * 2;
        memcpy(setup.header.data[npkts - 1].buf, pkt.buf, pkt.len);
      }
      if (npkts == 3) {
        setup.header.len = 3;
        dec = kr_vorbis_decoder_init(&setup);
        if (!dec) return -45;
      }
      if (npkts > 3) {
        coded.data = pkt.buf;
        coded.len = pkt.len;
        coded.channels = 2;
        coded.rate = 44100;
        coded.tc = opkt_info.granule_pos;
        ret = kr_vorbis_decode(dec, &raw, &coded);
        if (ret < 0) {
          printk("VORBIS decoding failed on ogg pkt %d", npkts);
          return -9;
        }
        kr_audio_interleave(data, &raw);
        ret = write(ofd, data, raw.count * raw.channels * sizeof(float));
        if (ret < 0) return -8;
      }
    }
  }
  kr_vorbis_decoder_destroy(dec);
  free(osetup.mem);
  free(setup.mem);
  free(data);
  free(raw.samples[0]);
  free(raw.samples[1]);
  close(ifd);
  close(ofd);
  return 0;
}

static void print_usage() {
  printk("  ./vorbis_dec_test in.vorbis out.wav");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = vorbis_dec_test(argv[1], argv[2]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}

