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
#include <krad/container/y4m/pack.h>
#include <krad/codec/video/y4m/encoder.h>
#include "../util/image.h"

#define BUFSIZE 1024 * 1024 * 8

static int y4m_enc_test(char *outfile, char *infile) {
  ssize_t ret;
  int fd;
  uint8_t *buf;
  uint8_t *coded_buf;
  kr_y4m_muxer *y4m;
  kr_y4m_muxer_setup y4m_setup;
  kr_codec_header y4m_headers;
  kr_image raw_rgb;
  kr_image raw_yuv;
  kr_image_setup img_setup;
  kr_frame in_frame;
  kr_frame out_frame;
  kr_convert conv;
  kr_y4m_encoder *enc;
  kr_y4m_encoder_setup setup;
  memset(&in_frame, 0, sizeof(in_frame));
  memset(&out_frame, 0, sizeof(out_frame));
  memset(&y4m_headers, 0, sizeof(y4m_headers));
  memset(&y4m_setup, 0, sizeof(y4m_setup));
  memset(&setup, 0, sizeof(setup));
  memset(&raw_rgb, 0, sizeof(raw_rgb));
  memset(&raw_yuv, 0, sizeof(raw_yuv));
  buf = kr_alloc(BUFSIZE);
  coded_buf = kr_alloc(BUFSIZE);
  out_frame.coded.data = coded_buf;
  fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return -1;
  ret = kr_image_from_png_filename(&raw_rgb, infile);
  if (ret != 0) return -1;
  kr_image_convert_init(&conv);
  raw_yuv.info.w = raw_rgb.info.w;// - raw_rgb.info.w % 16;
  raw_yuv.info.h = raw_rgb.info.h;// - raw_rgb.info.h % 16;
  raw_yuv.info.fmt = KR_PIXELS_YUV420;
  img_setup.mem = kr_alloc(kr_image_sizeof(&raw_yuv.info));
  img_setup.info = raw_yuv.info;
  img_setup.img = &raw_yuv;
  kr_image_init2(&img_setup);
  ret = kr_image_convert(&conv, &raw_yuv, &raw_rgb);
  if (ret != 0) return -1;
  kr_image_pixels_free(&raw_rgb);
  setup.mem = kr_alloc(kr_y4m_encoder_sizeof());
  setup.info.width = raw_yuv.info.w;
  setup.info.height = raw_yuv.info.h;
  setup.info.pixel_fmt = raw_yuv.info.fmt;
  setup.info.fps_num = 30;
  setup.info.fps_den = 1;
  enc = kr_y4m_encoder_init(&setup);
  if (enc == NULL) return -1;
  in_frame.image = raw_yuv;
  in_frame.type = KR_IMAGE_RAW;
  ret = kr_y4m_encoder_get_headers(enc, &y4m_headers);
  if (ret < 0) return -1;
  y4m_setup.header = y4m_headers.data[0].buf;
  y4m_setup.header_sz = y4m_headers.data[0].sz;
  y4m_setup.mem = kr_alloc(kr_y4m_muxer_sizeof());
  printk("Muxer: Y4M codec header is %zu bytes", y4m_headers.data[0].sz);
  y4m = kr_y4m_muxer_init(&y4m_setup);
  if (y4m) {
    printk("Muxer: YAY y4m!!");
  } else {
    printk("Muxer: YAY oh noe :(");
  }
  ret = kr_y4m_pack_header(y4m, buf, BUFSIZE);
  printk("Muxer: Y4M muxer header is %zu bytes", ret);
  ret = write(fd, buf, ret);
  ret = kr_y4m_encode(enc, &out_frame, &in_frame);
  if (ret != 0) {
    printk("encode failed %d", ret);
    return -1;
  }
  ret = kr_y4m_pack_frame_header(y4m, buf, BUFSIZE);
  printk("Muxer: Y4M muxer frame header is %zu bytes", ret);
  ret = write(fd, buf, ret);
  ret = write(fd, out_frame.coded.data, out_frame.coded.info.sz);
  printk("Muxer: Y4M muxer frame is %zu bytes", ret);
  kr_image_convert_clear(&conv);
  close(fd);
  free(img_setup.mem);
  free(enc);
  free(buf);
  free(coded_buf);
  free(y4m);
  return 0;
}

static void print_usage() {
  printk("  ./y4m_enc_test in.png out.y4m");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = y4m_enc_test(argv[2], argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
