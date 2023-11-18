#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/container/mkv/pack.h>

static int mkv_mux_test(char *out) {
  int fd;
  ssize_t ret;
  char *title;
  kr_mkv_muxer *muxer;
  kr_mkv_muxer_setup setup;
  kr_mkv_muxer_data data;
  uint8_t buffer[8192];
  memset(&setup, 0, sizeof(setup));
  memset(&data, 0, sizeof(data));
  fd = open(out, O_CREAT | O_EXCL | O_RDWR, 0640);
  if (fd < 0) return -1;
  title = "MKV TEST";
  setup.mem = kr_allocz(1, kr_mkv_muxer_sizeof());
  setup.title = title;
  setup.type = KR_CONTAINER_WEBM;
  setup.info[0].video.width = 1280;
  setup.info[0].video.height = 720;
  setup.info[0].video.fps_num = 30;
  setup.info[0].video.fps_den = 1;
  setup.info[0].codec.type = KR_CODEC_VIDEO;
  setup.info[0].codec.video = KR_VIDEO_CODEC_VPX;
  setup.info[0].type = KR_TRACK_CODED_VIDEO;
  setup.tracks++;
  setup.info[1].audio.sample_rate = 48000;
  setup.info[1].audio.channels = 2;
  setup.info[1].audio.bit_depth = 32;
  setup.info[1].codec.type = KR_CODEC_AUDIO;
  setup.info[1].codec.audio = KR_AUDIO_CODEC_OPUS;
  setup.info[1].type = KR_TRACK_CODED_AUDIO;
  setup.tracks++;
  muxer = kr_mkv_muxer_init(&setup);
  if (!muxer) return -2;
  ret = kr_mkv_muxer_pack_header(muxer, buffer, sizeof(buffer));
  if (ret <= 0) return -3;
  printk("packed %d bytes hdr", ret);
  ret = write(fd, buffer, ret);

  int x;
  for (x = 0; x < 666; x++) {
    data.track = 1;
    data.framing = buffer;
    data.sz = sizeof(buffer);
    data.len = 0;
    data.pkt_len = 10;
    data.tc = 0;
    data.keyframe = 0;
    data.frames = 480;
    ret = kr_mkv_mux(muxer, &data);
    if (ret <= 0) return -3;
    printk("packed %d bytes block framing", ret);
    ret = write(fd, buffer, ret);
    ret = write(fd, buffer + 300, data.pkt_len);
  }

  close(fd);
  free(setup.mem);
  return 0;
}

static void print_usage() {
  printk("  ./mkv_mux_test out.webm");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage();
    return 1;
  }
  ret = mkv_mux_test(argv[1]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
