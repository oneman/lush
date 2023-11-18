#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>
#include <krad/container/wave/pack.h>
#include <krad/container/wave/unpack.h>

static char *wave_fmt_to_str(int fmt) {
  switch (fmt) {
    case KR_WAVE_PCM: return "PCM";
    case KR_WAVE_IEEE_FLOAT: return "IEEE FLOAT";
    case KR_WAVE_ALAW: return "8-bit ITU-T G.711 A-law";
    case KR_WAVE_MULAW: return "8-bit ITU-T G.711 µ-law";
    case KR_WAVE_EXTENSIBLE: return "EXTENSIBLE";
    default: break;
  }
  return NULL;
}

static void print_wave_info(kr_wave_info *info) {
  printk("hdr size: %zu", info->hdr_sz);
  printk("fmt: %s", wave_fmt_to_str(info->fmt));
  printk("channels: %d", info->audio.channels);
  printk("sample rate: %d", info->audio.sample_rate);
  printk("bit depth: %d", info->audio.bit_depth);
  printk("samples data total size: %d", info->data_sz);
}

static int wave_test(char *in, char *out) {
  int fd_out;
  uint8_t buffer[1024];
  uint8_t header[512];
  uint8_t *data;
  size_t size;
  ssize_t ret;
  kr_wave_parser *w;
  kr_wave_parser_setup setup;
  kr_wave_info info;
  data = NULL;
  fd_out = -1;
  if (strcmp(in, "-")) return -1;
  ret = read(0, buffer, sizeof(buffer));
  if (ret <= 0) return -1;
  data = buffer;
  size = ret;
  setup.mem = kr_alloc(kr_wave_parser_sizeof());
  w = kr_wave_parser_init(&setup);
  if (!w) return -1;
  ret = kr_wave_parse(w, data, size);
  if (ret != 0) return -1;
  kr_wave_parser_get_info(w, &info);
  print_wave_info(&info);
  if (out) {
    fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC,
     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_out < 0) return -1;
    ret = kr_wave_pack_header(header, sizeof(header), &info);
    if (ret <= 0) return -2;
    ret = write(fd_out, header, info.hdr_sz);
    if (ret <= 0) return -1;
    ret = write(fd_out, buffer + info.hdr_sz, size - info.hdr_sz);
    if (ret < 0) return -1;
    for (;;) {
      ret = read(0, buffer, sizeof(buffer));
      if (ret < 0) return -1;
      if (ret == 0) break;
      ret = write(fd_out, buffer, ret);
      if (ret < 0) return -1;
      if (ret == 0) break;
    }
  }
  if (fd_out > 0) close(fd_out);
  free(setup.mem);
  return 0;
}

static void print_usage() {
  printk("Usage\n  cat in.wav | ./wave_test - [out.wav]");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2 && argc != 3) {
    print_usage();
    return 1;
  }
  if (argc == 2) ret = wave_test(argv[1], NULL);
  if (argc == 3) ret = wave_test(argv[1], argv[2]);
  if (ret < 0) fprintf(stderr, "Failure\n");
  return 0;
}
