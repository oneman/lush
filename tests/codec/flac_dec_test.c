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
#include <krad/codec/audio/flac/decoder.h>
#include <krad/container/flac/unpack.h>

static int flac_dec_test(char *in, char *out) {
  int i;
  int ifd;
  int ofd;
  int ret;
  int eos;
  size_t sz;
  size_t rd;
  size_t len;
  size_t frames;
  uint8_t *buf;
  uint8_t *hdr;
  uint8_t whdr[64];
  kr_flac_demuxer_setup setup;
  kr_flac_demuxer *d;
  kr_flac_decoder *dec;
  kr_flac_decoder_setup dec_setup;
  kr_flac_info info;
  kr_flac_data data;
  kr_wave_info winfo;
  kr_coded_audio coded;
  kr_audio raw;
  float *rawdata;
  eos = 0;
  frames = 0;
  ifd = open(in, O_RDONLY);
  if (ifd < 0) return -1;
  ofd = open(out, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (ofd < 0) return -2;
  sz = 1024 * 1024;
  buf = malloc(sz);
  hdr = malloc(sz);
  ret = read(ifd, buf, sz);
  if (ret <= 0) return -3;
  rd = ret;
  setup.mem = malloc(kr_flac_demuxer_sizeof());
  setup.buf = buf;
  setup.len = rd;
  d = kr_flac_demuxer_init(&setup);
  if (!d) return -4;
  ret = kr_flac_unpack_header(d, hdr, sz);
  if (ret <= 0) {
    printke("Not enough space in buffer, hdr too big!");
    return -4;
  }
  printk("\nRead %d bytes of FLAC header", ret);
  kr_flac_demuxer_get_info(d, &info);
  memmove(buf, buf + ret, rd - ret);
  len = rd - ret;
  memset(&winfo, 0, sizeof(winfo));
  winfo.fmt = KR_WAVE_IEEE_FLOAT;
  winfo.audio.channels = info.channels;
  winfo.audio.sample_rate = info.sample_rate;
  winfo.audio.bit_depth = 32;
  winfo.extensible = 0;
  ret = kr_wave_pack_header(whdr, sizeof(whdr), &winfo);
  if (ret <= 0) return -5;
  ret = write(ofd, whdr, ret);
  if (ret < 0) return -6;
  dec_setup.mem = malloc(kr_flac_decoder_sizeof());
  dec = kr_flac_decoder_init(&dec_setup);
  if (!dec) return -7;
  memset(&raw, 0, sizeof(raw));
  memset(&coded, 0, sizeof(coded));
  raw.samples[0] = malloc(sizeof(float) * 8192);
  raw.samples[1] = malloc(sizeof(float) * 8192);
  rawdata = malloc(sizeof(float) * 8192 * 8);
  size_t tsz = 0;
  for (;;) {
    ret = read(ifd, buf + len, sz - len);
    if (ret < 0) return -8;
    if (ret == 0) eos = 1;
    len += ret;
    memset(&data, 0, sizeof(data));
    data.in.buf = buf;
    data.in.len = len;
    ret = kr_flac_demux(d, &data);
    if (ret != 0) {
      printke("flac demux failed %d.", ret);
      return -9;
    }
    frames += data.out.len;
    for (i = 0; i < data.out.len; i++) {
      printk("FRAME %d %zu %zu", frames + i, data.out.frames[i].len, tsz);
      tsz += data.out.frames[i].len;
      coded.data = data.out.frames[i].buf;
      coded.len = data.out.frames[i].len;
      coded.channels = info.channels;
      coded.rate = info.sample_rate;
      ret = kr_flac_decode(dec, &raw, &coded);
      if (ret < 0) return -9;
      kr_audio_interleave(rawdata, &raw);
      ret = write(ofd, rawdata, raw.count * raw.channels * sizeof(float));
      if (ret < 0) return -10;
    }
    if (eos) break;
    len -= data.read;
    memmove(buf, buf + data.read, len);
  }
  printk("Demuxed %zu FLAC frames.", frames);
  free(hdr);
  free(buf);
  free(setup.mem);
  kr_flac_decoder_destroy(dec);
  free(dec_setup.mem);
  free(rawdata);
  free(raw.samples[0]);
  free(raw.samples[1]);
  close(ifd);
  close(ofd);
  return 0;
}

static void print_usage() {
  printk("  ./flac_dec_test in.flac out.wav");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = flac_dec_test(argv[1], argv[2]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
