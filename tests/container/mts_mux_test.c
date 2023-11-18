#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/audio/convert.h>
#include <krad/container/mpeg/ts/pack.h>
#include <krad/container/wave/unpack.h>
#include <krad/codec/audio/aac/encoder.h>

int mts_mux_test() {
  int i;
  int ret;
  size_t len;
  uint8_t buf[8192];
  uint8_t hdr[512];
  kr_aac_encoder_setup aac_setup;
  kr_aac_encoder *aac;
  kr_audio audio;
  kr_coded_audio coded;
  kr_wave_parser *wave;
  kr_wave_info wave_info;
  kr_wave_parser_setup wave_setup;
  kr_mts_muxer *ts;
  kr_mts_muxer_data ts_data;
  kr_mts_muxer_setup ts_setup;
  memset(&ts_setup, 0, sizeof(ts_setup));
  memset(&wave_setup, 0, sizeof(wave_setup));
  memset(&aac_setup, 0, sizeof(aac_setup));
  wave_setup.mem = kr_alloc(kr_wave_parser_sizeof());
  wave = kr_wave_parser_init(&wave_setup);
  if (!wave) return -1;
  ret = read(0, buf, sizeof(buf));
  if (ret <= 0) return -2;
  len = ret;
  ret = kr_wave_parse(wave, buf, len);
  if (ret != 0) return -3;
  kr_wave_parser_get_info(wave, &wave_info);
  if (wave_info.fmt != KR_WAVE_PCM || wave_info.audio.bit_depth != 16 ||
    wave_info.audio.channels != 2) {
    printke("Need samples to be 2 channels 16bit PCM");
    return -4;
  }
  len -= wave_info.hdr_sz;
  memmove(buf, buf + wave_info.hdr_sz, len);
  ret = read(0, buf + len, sizeof(buf) - len);
  if (ret <= 0) return -5;
  len += ret;
  if (len != sizeof(buf)) return -6;
  ts_setup.mem = kr_alloc(kr_mts_muxer_sizeof());
  ts_setup.tracks = 1;
  ts_setup.track_info[0].type = KR_TRACK_CODED_AUDIO;
  ts_setup.track_info[0].codec.type = KR_CODEC_AUDIO;
  ts_setup.track_info[0].codec.audio = KR_AUDIO_CODEC_AAC;
  ts_setup.track_info[0].audio = wave_info.audio;
  ts_setup.track_info[0].audio.samples = len / (wave_info.audio.channels * sizeof(int16_t));
  ts = kr_mts_muxer_init(&ts_setup);
  if (!ts) return -12;
  memset(&audio, 0, sizeof(audio));
  audio.samples[0] = kr_alloc(8192);
  audio.samples[1] = kr_alloc(8192);
  audio.channels = wave_info.audio.channels;
  audio.count = len / (audio.channels * sizeof(int16_t));
  aac_setup.mem = kr_alloc(kr_aac_encoder_sizeof());
  aac_setup.info.channels = wave_info.audio.channels;
  aac_setup.info.sample_rate = wave_info.audio.sample_rate;
  aac_setup.info.bit_rate = 64000;
  aac = kr_aac_encoder_init(&aac_setup);
  if (!aac) return -7;
  memset(&coded, 0, sizeof(coded));
  coded.sz = 8192;
  coded.data = kr_alloc(coded.sz);
  ret = kr_mts_muxer_pack_header(ts, hdr, sizeof(hdr));
  if (ret <= 0) return -13;
  ret = write(1, hdr, ret);
  if (ret <= 0) return -14;
  for (;;) {
    wave_info.data_sz -= len;
    ret = kr_audio_deinterleave_16bit(&audio, (int16_t *)buf);
    if (ret != 0) return -8;
    ret = kr_aac_encode(aac, &coded, &audio);
    if (ret != 0) return -9;
    memset(&ts_data, 0, sizeof(ts_data));
    ts_data.track = 0;
    ts_data.input = coded.data;
    ts_data.sz = coded.len;
    ret = kr_mts_mux(ts, &ts_data);
    if (ret != 0) {
      printke("mts mux failed %d", ret);
      return -15;
    }
    for (i = 0; i < ts_data.npkts; i++) {
      ret = write(1, ts_data.pkts[i].hdr, ts_data.pkts[i].hdr_sz);
      if (ret != ts_data.pkts[i].hdr_sz) return -16;
      ret = write(1, ts_data.pkts[i].buf, ts_data.pkts[i].sz);
      if (ret != ts_data.pkts[i].sz) return -17;
    }
    if (wave_info.data_sz == 0) break;
    ret = read(0, buf, sizeof(buf));
    if (ret <= 0) return -11;
    len = ret;
    audio.count = len / (audio.channels * sizeof(int16_t));
  }
  kr_aac_encoder_destroy(aac);
  free(ts_setup.mem);
  free(wave_setup.mem);
  free(aac_setup.mem);
  free(coded.data);
  return 0;
}

void print_usage() {
  printk("Usage\n cat in.wav | ./mts_mux_test > out.ts");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 1) {
    print_usage();
    return 1;
  }
  ret = mts_mux_test();
  if (ret != 0) {
    fprintf(stderr, "Failure %d\n", ret);
  }
  return 0;
}
