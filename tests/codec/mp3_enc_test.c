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
#include "util/tone.h"
#include <krad/audio/noise.h>
#include <krad/codec/audio/mp3/encoder.h>

static int mp3_enc_test(char *filename) {
  int fd;
  int i;
  int c;
  int ret;
  kr_mp3_encoder *mp;
  kr_mp3_encoder_setup setup;
  kr_mp3_info info;
  kr_audio audio;
  kr_coded_audio coded_audio;
  krad_tone_t *tone;
  kr_noise *n;
  n = malloc(kr_noise_sizeof());
  kr_noise_init(n);
  memset(&info, 0, sizeof(info));
  info.channels = 2;
  info.sample_rate = 48000;
  info.bit_rate = 128000;
  coded_audio.sz = 8192 * 8;
  coded_audio.len = 0;
  coded_audio.data = calloc(1, coded_audio.sz);
  audio.rate = info.sample_rate;
  audio.channels = info.channels;
  for (c = 0; c < info.channels; c++) {
    audio.samples[c] = calloc(1, 8192 * sizeof(float));
  }
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  setup.mem = calloc(1, kr_mp3_encoder_sizeof());
  setup.info = info;
  mp = kr_mp3_encoder_init(&setup);
  audio.count = 2048;
  tone = krad_tone_create(info.sample_rate);
  for (i = 0; i < 45 * 22; i++) {
    memset(audio.samples[0], 0, audio.count * sizeof(float));
    if (i == 20) {
      krad_tone_add_preset(tone, "1");
    }
    if (i == 40) {
      krad_tone_add_preset(tone, "4");
    }
    if (i == 60) {
      krad_tone_add_preset(tone, "5");
    }
    if (i == 80) {
      krad_tone_add_preset(tone, "6");
    }
    if (i == 130) {
      krad_tone_add_preset(tone, "7");
    }
    if (i == 200) {
      krad_tone_add_preset(tone, "8");
    }
    if (i == 260) {
      krad_tone_add_preset(tone, "A");
    }
    if (i == 310) {
      krad_tone_add_preset(tone, "B");
    }
    if (i == 360) {
      krad_tone_add_preset(tone, "C");
    }
    if (i == 420) {
      krad_tone_add_preset(tone, "D");
    }
    krad_tone_run(tone, audio.samples[0], audio.count);
    for (c = 1; c < info.channels; c++) {
      memcpy(audio.samples[c], audio.samples[0], audio.count * sizeof(float));
    }
    ret = kr_noise_add(n, &audio);
    if (ret != 0) return -1;
    ret = kr_mp3_encode(mp, &coded_audio, &audio);
    if (ret != 0) return -1;
    ret = write(fd, coded_audio.data, coded_audio.len);
    if (ret < 0) return -1;
  }
  kr_mp3_encoder_destroy(mp);
  krad_tone_destroy(tone);
  free(n);
  close(fd);
  free(coded_audio.data);
  for (i = 0; i < info.channels; i++) {
    free(audio.samples[i]);
  }
  free(setup.mem);
  return 0;
}

static void print_usage() {
  printk("  ./mp3_enc_test out.mp3");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage();
    return 1;
  }
  ret = mp3_enc_test(argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
