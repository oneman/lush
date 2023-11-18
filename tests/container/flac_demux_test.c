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
#include <krad/container/flac/unpack.h>

static int flac_demux_test(char *in) {
  int fd;
  int ret;
  int eos;
  size_t sz;
  size_t rd;
  size_t len;
  size_t frames;
  uint8_t *buf;
  uint8_t *hdr;
  kr_flac_demuxer_setup setup;
  kr_flac_demuxer *d;
  kr_flac_info info;
  kr_flac_data data;
  eos = 0;
  frames = 0;
  fd = open(in, O_RDONLY);
  if (fd < 0) return -1;
  sz = 1024 * 1024;
  buf = malloc(sz);
  hdr = malloc(sz);
  ret = read(fd, buf, sz);
  if (ret <= 0) return -2;
  rd = ret;
  setup.mem = malloc(kr_flac_demuxer_sizeof());
  setup.buf = buf;
  setup.len = rd;
  d = kr_flac_demuxer_init(&setup);
  if (!d) return -3;
  ret = kr_flac_unpack_header(d, hdr, sz);
  if (ret <= 0) {
    printke("Not enough space in buffer, hdr too big!");
    return -4;
  }
  printk("\nRead %d bytes of FLAC header", ret);
  kr_flac_demuxer_get_info(d, &info);
  printk("  Samplerate: %uHz", info.sample_rate);
  printk("  Channels: %u", info.channels);
  printk("  Bitdepth: %ubit", info.bit_depth);
  memmove(buf, buf + ret, rd - ret);
  len = rd - ret;
  for (;;) {
    ret = read(fd, buf + len, sz - len);
    if (ret < 0) return -5;
    if (ret == 0) eos = 1;
    len += ret;
    memset(&data, 0, sizeof(data));
    data.in.buf = buf;
    data.in.len = len;
    ret = kr_flac_demux(d, &data);
    if (ret != 0) {
      printke("flac demux failed %d.", ret);
      return -6;
    }
    frames += data.out.len;
    if (eos) break;
    len -= data.read;
    memmove(buf, buf + data.read, len);
  }
  printk("Demuxed %zu FLAC frames.", frames);
  free(hdr);
  free(buf);
  free(setup.mem);
  close(fd);
  return 0;
}

static void print_usage() {
  printk("  ./flac_demux_test in.flac");
  printk("  find ~/Music/ -type f -name \"*.flac\" "
   "-print0 -exec ./tests/flac_demux_test {} \\;");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage();
    return 1;
  }
  ret = flac_demux_test(argv[1]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
