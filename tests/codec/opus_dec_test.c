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
#include <krad/codec/audio/opus/decoder.h>

static int opus_dec_test(char *in, char *out) {
  uint8_t buf[512];
  uint8_t hdr[512];
  ssize_t ret;
  uint8_t buffer[8192 * 8];
  size_t buffer_len;
  size_t bytes;
  size_t len;
  int ifd;
  int ofd;
  kr_ogg_unpacker_setup osetup;
  kr_ogg_unpacker *u;
  kr_codec_packet pkt;
  kr_ogg_pkt_info opkt_info;
  kr_opus_decoder *dec;
  kr_opus_decoder_setup setup;
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
  setup.mem = malloc(kr_opus_decoder_sizeof());
  setup.info.sample_rate = 48000;
  dec = kr_opus_decoder_init(&setup);
  if (!dec) return -3;
  pkt.sz = 8192;
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
  winfo.audio.sample_rate = 48000;
  winfo.audio.bit_depth = 32;
  winfo.extensible = 0;
  ret = kr_wave_pack_header(hdr, sizeof(hdr), &winfo);
  if (ret <= 0) return -6;
  ret = write(ofd, hdr, ret);
  if (ret < 0) return -7;
  kr_ogg_unpacker_output_set(u, &pkt, 1);
  buffer_len = 0;
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
      printk("Demux OGG pkt %zu", pkt.len);
      coded.data = pkt.buf;
      coded.len = pkt.len;
      coded.channels = 2;
      coded.rate = 48000;
      ret = kr_opus_decode(dec, &raw, &coded);
      if (ret < 0) {
        printk("OPUS decoding failed on pkt %d", opkt_info.num);
        continue;
      }
      kr_audio_interleave(data, &raw);
      ret = write(ofd, data, raw.count * raw.channels * sizeof(float));
      if (ret < 0) return -8;
    }
  }
  kr_opus_decoder_destroy(dec);
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
  printk("  ./opus_dec_test in.opus out.wav");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = opus_dec_test(argv[1], argv[2]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
