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
#include <krad/audio/signal.h>
#include <krad/audio/noise.h>
#include <krad/codec/audio/flac/encoder.h>

static int flac_enc_test(char *filename) {
  int fd;
  int i;
  int c;
  int ret;
  kr_flac_encoder *fe;
  kr_flac_encoder_setup setup;
  kr_flac_info info;
  kr_audio audio;
  kr_coded_audio coded_audio;
  kr_signal_gen_setup sg_setup;
  kr_signal_gen *sg;
  kr_signal sig;
  kr_codec_header flac_header;
  kr_noise *n;
  n = malloc(kr_noise_sizeof());
  kr_noise_init(n);
  memset(&info, 0, sizeof(info));
  info.channels = 2;
  info.sample_rate = 48000;
  info.bit_depth = 24;
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
  ret = write(fd, flac_header.data[0].buf, flac_header.data[0].sz);
  audio.count = 2048;
  kr_signal_gen_setup_init(&sg_setup);
  sg_setup.mem = kr_allocz(1, kr_signal_gen_sizeof());
  sg = kr_signal_gen_init(&sg_setup);
  sig.type = KR_SIGNAL_GEN_DTMF;
  sig.len = snprintf(sig.str, sizeof(sig.str), "17572200044");
  kr_signal_gen_put(sg, &sig);
  for (i = 0; i < 48 * 5; i++) {
    ret = kr_noise_fill(n, &audio);
    //kr_audio_zero(&audio);
    kr_signal_emit(sg, &audio);
    ret = kr_flac_encode(fe, &coded_audio, &audio);
    ret = write(fd, coded_audio.data, coded_audio.len);
    if (ret <= 0) {
      //return -1;
    }
  }
  kr_flac_encoder_destroy(fe);
  free(sg_setup.mem);
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
  printk("  ./flac_enc_test out.flac");
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
