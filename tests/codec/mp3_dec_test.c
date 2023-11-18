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
#include <krad/audio/resample.h>
#include <krad/container/wave/pack.h>
#include <krad/codec/audio/mp3/decoder.h>

static int mp3_dec_test(char *in, char *out) {
  uint8_t buf[512];
  uint8_t hdr[512];
  ssize_t ret;
  int ifd;
  int ofd;
  kr_mp3_decoder *dec;
  kr_mp3_decoder_setup setup;
  kr_coded_audio coded;
  kr_audio raw;
  kr_audio resampled;
  kr_audio *audio;
  kr_resampler_setup rsetup;
  kr_resampler *resampler;
  float *data;
  kr_wave_info winfo;
  ifd = open(in, O_RDONLY);
  if (ifd < 0) return -1;
  ofd = open(out, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (ofd < 0) return -2;
  memset(&rsetup, 0, sizeof(rsetup));
  rsetup.mem = malloc(kr_resampler_sizeof());
  rsetup.channels = 2;
  resampler = kr_resampler_init(&rsetup);
  if (!resampler) return -9;
  setup.mem = malloc(kr_mp3_decoder_sizeof());
  dec = kr_mp3_decoder_init(&setup);
  if (!dec) return -3;
  memset(&raw, 0, sizeof(raw));
  memset(&resampled, 0, sizeof(resampled));
  memset(&coded, 0, sizeof(coded));
  memset(&winfo, 0, sizeof(winfo));
  raw.samples[0] = malloc(sizeof(float) * 8192 * 2);
  raw.samples[1] = malloc(sizeof(float) * 8192 * 2);
  resampled.samples[0] = malloc(sizeof(float) * 8192 * 2);
  resampled.samples[1] = malloc(sizeof(float) * 8192 * 2);
  data = malloc(8192 * sizeof(float) * 4);
  winfo.fmt = KR_WAVE_IEEE_FLOAT;
  winfo.audio.channels = 2;
  winfo.audio.sample_rate = 48000;
  winfo.audio.bit_depth = 32;
  winfo.extensible = 0;
  ret = kr_wave_pack_header(hdr, sizeof(hdr), &winfo);
  if (ret <= 0) return -6;
  ret = write(ofd, hdr, ret);
  if (ret < 0) return -7;
  for (;;) {
    ret = read(ifd, buf, sizeof(buf));
    if (ret < 0) return -4;
    if (ret == 0) break;
    coded.data = buf;
    coded.len = ret;
    coded.channels = 2;
    raw.count = 0;
    ret = kr_mp3_decode(dec, &raw, &coded);
    if (ret < 0) {
      printke("mp3 decode failed %d", ret);
      return -5;
    }
    if (raw.count == 0) continue;
    if (raw.rate != 48000) {
      resampled.count = 8192 * 2;
      resampled.channels = 2;
      ret = kr_resampler_process(resampler, &resampled, &raw);
      if (ret != 0) return -10;
      audio = &resampled;
    } else {
      audio = &raw;
    }
    kr_audio_interleave(data, audio);
    ret = write(ofd, data, audio->count * audio->channels * sizeof(float));
    if (ret < 0) return -8;
  }
  kr_resampler_destroy(resampler);
  kr_mp3_decoder_destroy(dec);
  free(rsetup.mem);
  free(setup.mem);
  free(data);
  free(raw.samples[0]);
  free(raw.samples[1]);
  close(ifd);
  close(ofd);
  return 0;
}

static void print_usage() {
  printk("  ./mp3_dec_test in.mp3 out.wav");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = mp3_dec_test(argv[1], argv[2]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
