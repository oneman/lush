#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/video/gif.h>
#include <krad/mem/mem.h>
#include <krad/codec/video/h264/encoder.h>
#include "../util/image.h"

static int h264_enc_test(char *fout, char *fin) {
  int i;
  size_t offset;
  ssize_t ret;
  int fd;
  kr_h264_encoder_setup setup;
  kr_h264_encoder *enc;
  kr_image raw_rgb;
  kr_image raw_yuv;
  kr_image_setup img_setup;
  kr_frame in_frame;
  kr_frame out_frame;
  kr_convert conv;
  memset(&in_frame, 0, sizeof(in_frame));
  memset(&out_frame, 0, sizeof(out_frame));
  memset(&setup, 0, sizeof(setup));
  memset(&raw_rgb, 0, sizeof(raw_rgb));
  memset(&raw_yuv, 0, sizeof(raw_yuv));
  fd = open(fout, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return -1;
  setup.mem = kr_alloc(kr_h264_encoder_sizeof());
  ret = kr_image_from_png_filename(&raw_rgb, fin);
  if (ret != 0) return -1;
  kr_image_convert_init(&conv);
  raw_yuv.info.w = raw_rgb.info.w;
  raw_yuv.info.h = raw_rgb.info.h;
  raw_yuv.info.fmt = KR_PIXELS_YUV420;
  img_setup.mem = kr_alloc(kr_image_sizeof(&raw_yuv.info));
  img_setup.info = raw_yuv.info;
  img_setup.img = &raw_yuv;
  kr_image_init2(&img_setup);
  ret = kr_image_convert(&conv, &raw_yuv, &raw_rgb);
  if (ret != 0) return -1;
  kr_image_pixels_free(&raw_rgb);
  setup.info.width = raw_yuv.info.w;
  setup.info.height = raw_yuv.info.h;
  setup.info.pixel_fmt = raw_yuv.info.fmt;
  setup.info.fps_num = 30;
  setup.info.fps_den = 1;
  setup.info.bit_rate = 450;
  setup.info.key_rate = 30;
  enc = kr_h264_encoder_init(&setup);
  if (!enc) return -1;
  in_frame.image = raw_yuv;
  in_frame.type = KR_IMAGE_RAW;
  // 10 seconds still image video
  for (i = offset = 0; i < 300; i++) {
    ret = kr_h264_encode(enc, &out_frame, &in_frame);
    if (ret < 0) {
      printk("encode failed %d", ret);
      return -1;
    }
    ret = write(fd, out_frame.coded.data, ret);
    offset += ret;
    lseek(fd, offset, SEEK_SET);
    if (ret <= 0) return -1;
  }
  kr_h264_encoder_destroy(enc);
  kr_image_convert_clear(&conv);
  close(fd);
  free(img_setup.mem);
  free(enc);
  return 0;
}

static void print_usage() {
  printk("  ./h264_enc_test in.png out.h264");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = h264_enc_test(argv[2], argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
