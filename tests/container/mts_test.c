#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
#include <krad/io/file2.h>
#include <krad/container/mpeg/ts/unpack.h>
#include <krad/image/image.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/codec/lavc/decoder.h>
#include "../util/image.h"

static int mts_decode_audio(kr_audio_dec *dec, uint8_t *buf, size_t sz) {
  int ret;
  ret = kr_avc_decode_audio(dec, buf, sz);
  if (ret < 0) {
    printke("AAC Decoder Error: %d", ret);
    return -1;
  }
  if (ret > 0) {
    printk("Decoded AAC frame");
  }
  return 0;
}

static void save_frame(kr_frame *frame) {
  int ret;
  char filename[512];
  kr_frame frm;
  kr_convert conv;
  static int i = 0;
  memset(&frm, 0, sizeof(frm));
  frm.image.info.w = frame->image.info.w;
  frm.image.ps[0] = frm.image.info.w * 4;
  frm.image.info.h = frame->image.info.h;
  frm.image.info.fmt = KR_PIXELS_ARGB;
  kr_image_pixels_alloc(&frm.image);
  memset(frm.image.px[0], 1, frm.image.ps[0] * frm.image.info.h);
  kr_image_convert_init(&conv);
  ret = kr_image_convert(&conv, &frm.image, &frame->image);
  if (ret) {
    printke("convert ret %d", ret);
    exit(1);
  }
  snprintf(filename, sizeof(filename), "%s/test_%d.png", getenv("HOME"), i++);
  kr_image_save_file(filename, &frm.image);
  kr_image_pixels_free(&frm.image);
  kr_image_convert_clear(&conv);
}

static int mts_decode_video(kr_video_dec *dec, uint8_t *buf, size_t sz) {
  kr_coded_image img;
  kr_frame frame;
  int ret;
  memset(&img, 0, sizeof(img));
  memset(&frame, 0, sizeof(frame));
  img.data = buf;
  img.info.sz = sz;
  //fwrite(buf, 1, sz, stderr);
  ret = kr_avc_decode_video(dec, &frame, &img);
  if (ret < 0) {
    printke("H264 Decoder Error: %d", ret);
    return -1;
  }
  if (ret > 0) {
    char text[1024];
    kr_image_info_to_text(text, &frame.image.info, sizeof(text));
    printk("Decoded H264 frame:\n%s", text);
    save_frame(&frame);
  }
  return 0;
}

static int mts_demux(uint8_t *data, size_t sz) {
  int i;
  ssize_t ret;
  char text[4096];
  uint8_t *buffer;
  kr_mts *mts;
  kr_mts_info mts_info;
  kr_mts_packet_info pkt_info;
  kr_mts_setup setup;
  kr_video_dec *vdec;
  kr_audio_dec *adec;
  memset(&pkt_info, 0, sizeof(pkt_info));
  memset(&setup, 0, sizeof(setup));
  setup.mem = kr_alloc(kr_mts_sizeof());
  buffer = calloc(1, 8192 * 64);
  mts = kr_mts_init(&setup);
  if (mts == NULL) return -1;
  vdec = kr_avc_video_decoder_create();
  if (vdec == NULL) return -1;
  adec = kr_avc_audio_decoder_create();
  if (adec == NULL) return -1;
  for (i = 0; i < 512; i++) {
    kr_mts_output_set(mts, buffer, 8192 * 64);
    ret = kr_mts_parse(mts, &pkt_info, data, sz);
    if (ret < 0) return -1;
    if (ret == 0) break;
    kr_mts_packet_info_to_text(text, &pkt_info, sizeof(text));
    printk("\ninfo:\n%s", text);
    if (pkt_info.type == MTS_VIDEO) {
      if (mts_decode_video(vdec, buffer, pkt_info.sz) < 0) return -1;
    } else if (pkt_info.type == MTS_AUDIO) {
      if (mts_decode_audio(adec, buffer, pkt_info.sz) < 0) return -1;
    }
  }
  kr_avc_video_decoder_destroy(vdec);
  kr_avc_audio_decoder_destroy(adec);
  kr_mts_info_get(mts, &mts_info);
  kr_mts_info_to_text(text, &mts_info, sizeof(text));
  printk("\ninfo:\n%s", text);
  free(buffer);
  free(setup.mem);
  return 0;
}

static int file_event(kr_file_event *e) {
  return 0;
}

static int mts_test(char *path) {
  kr_file2 *file;
  kr_file_set *fs;
  kr_file2_info info;
  kr_fs_setup fs_setup;
  fs_setup.nfiles = 1;
  fs_setup.user = NULL;
  fs_setup.event_cb = file_event;
  memset(&info, 0, sizeof(info));
  fs = kr_file_set_create(&fs_setup);
  if (fs == NULL) return -1;
  file = kr_file2_open(fs, path, strlen(path));
  if (file == NULL) return -1;
  kr_file2_get_info(file, &info);
  if (mts_demux(kr_file2_get_data(file), info.sz) != 0) return -1;
  kr_file2_close(file);
  kr_file_set_destroy(fs);
  return 0;
}

void print_usage() {
  printk("Usage\n  ./mpeg_ts_test file.ts");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    print_usage();
    return 1;
  }
  if (mts_test(argv[1]) < 0) {
    fprintf(stderr, "test failed.\n");
  }
  return 0;
}
