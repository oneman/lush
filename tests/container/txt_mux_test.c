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
#include <krad/container/txt/pack.h>

static void setup_fill(kr_txt_muxer_setup *s) {
  s->comment_len = snprintf(s->comment, sizeof(s->comment),
   "KRAD RADIO TXT MUXER TEST");
  s->tracks = 2;
  s->track[0].header.len = 666;
  s->track[0].info.type = KR_TRACK_CODED_VIDEO;
  s->track[0].info.video.width = 1280;
  s->track[0].info.video.height = 720;
  s->track[0].info.video.fps_num = 30;
  s->track[0].info.video.fps_den = 1;
  s->track[0].info.codec.type = KR_CODEC_VIDEO;
  s->track[0].info.codec.video = KR_VIDEO_CODEC_VPX;
  s->track[0].info.type = KR_TRACK_CODED_VIDEO;
  s->track[0].info.mode = KR_TRACK_FINITE;
  s->track[0].info.duration = 1 * 45 * 666;
  s->track[0].info.track.len = snprintf(
    s->track[0].info.track.name,
    sizeof(s->track[1].info.track.name), "MasterVideo");

  s->track[1].header.len = 45;
  s->track[1].info.type = KR_TRACK_CODED_AUDIO;
  s->track[1].info.audio.sample_rate = 48000;
  s->track[1].info.audio.channels = 2;
  s->track[1].info.audio.bit_depth = 16;
  s->track[1].info.codec.type = KR_CODEC_AUDIO;
  s->track[1].info.codec.audio = KR_AUDIO_CODEC_FLAC;
  s->track[1].info.type = KR_TRACK_CODED_AUDIO;
  s->track[1].info.mode = KR_TRACK_FINITE;
  s->track[1].info.duration = 1 * 45 * 666;
  s->track[1].info.track.len = snprintf(
    s->track[1].info.track.name,
    sizeof(s->track[1].info.track.name), "MasterAudio");
}

static int txt_mux_test(char *out) {
  int i;
  int fd;
  ssize_t ret;
  kr_txt_muxer *muxer;
  kr_txt_muxer_setup setup;
  kr_txt_muxer_data data;
  uint8_t buffer[8192];
  memset(&setup, 0, sizeof(setup));
  fd = open(out, O_CREAT | O_EXCL | O_RDWR, 0640);
  if (fd < 0) return -1;
  setup.mem = kr_alloc(kr_txt_muxer_sizeof());
  setup_fill(&setup);
  muxer = kr_txt_muxer_init(&setup);
  if (!muxer) return -2;
  ret = kr_txt_muxer_pack_header(muxer, buffer, sizeof(buffer));
  if (ret <= 0) return -3;
  printk("packed %d bytes hdr", ret);
  ret = write(fd, buffer, ret);
  for (i = 0; i < 45; i++) {
    memset(&data, 0, sizeof(data));
    if (!(i % 2)) {
      data.track = 0;
      data.in.sz = 666 + (i % 45);
      if (!(i % 8)) data.in.keyframe = 1;
    } else {
      data.track = 1;
      data.in.sz = 45 + (i % 45);
      data.in.nframes = 480;
    }
    data.out.framing = buffer;
    data.out.space = sizeof(buffer);
    ret = kr_txt_mux(muxer, &data);
    if (ret < 0) return -4;
    ret = write(fd, buffer, ret);
  }
  close(fd);
  free(setup.mem);
  return 0;
}

static void print_usage() {
  printk("  ./txt_mux_test out.txt");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage();
    return 1;
  }
  ret = txt_mux_test(argv[1]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
