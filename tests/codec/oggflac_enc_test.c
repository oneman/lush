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
#include <krad/container/ogg/pack.h>
#include <krad/audio/noise.h>
#include "../util/tone.h"
#include <krad/codec/audio/flac/encoder.h>

#define BUFSIZE 8192 * 3
#define TEST_CHANNELS 2
#define TEST_SAMPLERATE 48000
#define TEST_BIT_DEPTH 24
#define TEST_SECONDS 30

static int flac_enc_test(char *filename) {
  int fd;
  int i;
  int c;
  int v;
  int ret;
  int s;
  uint8_t *buf;
  kr_noise *n;
  kr_flac_encoder *fe;
  kr_flac_encoder_setup setup;
  kr_flac_info info;
  kr_audio audio;
  kr_coded_audio coded_audio;
  krad_tone_t *tone;
  kr_codec_header flac_header;
  kr_ogg_muxer *ogg;
  kr_ogg_muxer_data od;
  kr_ogg_muxer_setup ogg_setup;
  n = malloc(kr_noise_sizeof());
  kr_noise_init(n);
  ogg_setup.mem = kr_alloc(kr_ogg_muxer_sizeof());
  ogg_setup.tracks = 1;
  buf = kr_alloc(BUFSIZE);
  memset(&info, 0, sizeof(info));
  info.channels = TEST_CHANNELS;
  info.sample_rate = TEST_SAMPLERATE;
  info.bit_depth = TEST_BIT_DEPTH;
  tone = krad_tone_create(info.sample_rate);
  krad_tone_add_preset(tone, "dialtone");
  coded_audio.sz = 8192 * 8;
  coded_audio.data = calloc(1, coded_audio.sz);
  audio.rate = info.sample_rate;
  audio.channels = info.channels;
  for (c = 0; c < info.channels; c++) {
    audio.samples[c] = calloc(1, 8192 * sizeof(float));
  }
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  setup.mem = calloc(1, kr_flac_encoder_sizeof());
  setup.info = info;
  fe = kr_flac_encoder_init(&setup);
  ret = kr_flac_encoder_get_headers(fe, &flac_header);
  if (ret) return -1;
  ogg_setup.header[0] = flac_header;
  ogg = kr_ogg_muxer_init(&ogg_setup);
  ret = kr_ogg_muxer_read_header(ogg, buf, BUFSIZE);
  if (ret <= 0) {
    printk("ogg muxing failed");
    return -1;
  }
  printk("OggFLAC Header SZ: %d", ret);
  ret = write(fd, buf, ret);
  if (ret <= 0) return -1;
  audio.count = 2048;
  for (i = 0; TEST_SECONDS * TEST_SAMPLERATE > info.total_frames; i++) {
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
      krad_tone_add_preset(tone, "6");
    }
    if (i == 150) {
      krad_tone_add_preset(tone, "6");
    }
    if (i == 180) {
      krad_tone_add_preset(tone, "A");
    }
    if (i == 210) {
      krad_tone_add_preset(tone, "B");
    }
    if (i == 250) {
      krad_tone_add_preset(tone, "C");
    }
    if (i == 270) {
      krad_tone_add_preset(tone, "D");
    }
    if (i == 370) {
      krad_tone_add_preset(tone, "dialtone_eu");
    }
    if (i == 470) {
      krad_tone_add_preset(tone, "dialtone_us");
    }
    if (i == 570) {
      krad_tone_add_preset(tone, "dialtone_uk");
    }
    krad_tone_run(tone, audio.samples[0], audio.count);
    for (s = 0; s < audio.count; s++) {
      audio.samples[0][s] += kr_noise_gen(n);
      if (audio.samples[0][s] > 1.0f || audio.samples[0][s] < -1.0f) {
        printk("cliped");
      }
    }
    for (c = 1; c < info.channels; c++) {
      memcpy(audio.samples[c], audio.samples[0], audio.count * sizeof(float));
    }
    ret = kr_flac_encode(fe, &coded_audio, &audio);
    if (coded_audio.len < 1) continue;
    kr_flac_encoder_get_info(fe, &info);
    od.framing_buf = buf;
    od.sz = BUFSIZE;
    od.track = 0;
    od.in[0].granule_pos = info.total_frames;
    od.in[0].buf = coded_audio.data;
    od.in[0].sz = coded_audio.len;
    od.in_len = 1;
    ret = kr_ogg_mux(ogg, &od);
    printk("og mux ret is %zu - vec len is %d", ret, od.out_len);
    for (v = 0; v < od.out_len; v++) {
      printk("part %d writing %zu bytes", v, od.out[v].sz);
      ret = write(fd, od.out[v].buf, od.out[v].sz);
    }
  }
  kr_flac_encoder_destroy(fe);
  krad_tone_destroy(tone);
  close(fd);
  free(coded_audio.data);
  for (i = 0; i < info.channels; i++) {
    free(audio.samples[i]);
  }
  free(n);
  free(buf);
  free(ogg_setup.mem);
  free(setup.mem);
  return 0;
}

static void print_usage() {
  printk("  ./flac_enc_test out.ogg");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage();
    return 1;
  }
  ret = flac_enc_test(argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
