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
#include <krad/container/ogg/pack.h>
#include <krad/codec/video/daala/encoder.h>
#include "../util/image.h"

#define BUFSIZE 8192 * 8 * 32

static int daala_enc_test(char *outfile, char *infile) {
  int i;
  ssize_t ret;
  int fd;
  uint8_t *buf;
  uint8_t *coded_buf;
  kr_ogg_muxer *ogg;
  kr_ogg_muxer_setup ogg_setup;
  kr_ogg_muxer_data od;
  kr_codec_header daala_headers;
  kr_image raw_rgb;
  kr_image raw_yuv;
  kr_image_setup img_setup;
  kr_frame in_frame;
  kr_frame out_frame;
  kr_convert conv;
  kr_daala_encoder *enc;
  kr_daala_encoder_setup setup;
  memset(&in_frame, 0, sizeof(in_frame));
  memset(&out_frame, 0, sizeof(out_frame));
  memset(&od, 0, sizeof(od));
  memset(&daala_headers, 0, sizeof(daala_headers));
  memset(&ogg_setup, 0, sizeof(ogg_setup));
  memset(&setup, 0, sizeof(setup));
  memset(&raw_rgb, 0, sizeof(raw_rgb));
  memset(&raw_yuv, 0, sizeof(raw_yuv));
  buf = kr_alloc(BUFSIZE);
  coded_buf = kr_alloc(BUFSIZE);
  fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return -1;
  ret = kr_image_from_png_filename(&raw_rgb, infile);
  if (ret != 0) return -1;
  kr_image_convert_init(&conv);
  raw_yuv.info.w = raw_rgb.info.w - raw_rgb.info.w % 16;
  raw_yuv.info.h = raw_rgb.info.h - raw_rgb.info.h % 16;
  raw_yuv.info.fmt = KR_PIXELS_YUV420;
  img_setup.mem = kr_alloc(kr_image_sizeof(&raw_yuv.info));
  img_setup.info = raw_yuv.info;
  img_setup.img = &raw_yuv;
  kr_image_init2(&img_setup);
  ret = kr_image_convert(&conv, &raw_yuv, &raw_rgb);
  if (ret != 0) return -1;
  kr_image_pixels_free(&raw_rgb);
  setup.mem = kr_alloc(kr_daala_encoder_sizeof());
  setup.info.width = raw_yuv.info.w;
  setup.info.height = raw_yuv.info.h;
  setup.info.pixel_fmt = raw_yuv.info.fmt;
  setup.info.fps_num = 30;
  setup.info.fps_den = 1;
  /*setup.info.quality = 33;
  setup.info.eos = 0;*/
  enc = kr_daala_encoder_init(&setup);
  if (enc == NULL) return -1;
  in_frame.image = raw_yuv;
  in_frame.type = KR_IMAGE_RAW;
  ret = kr_daala_encoder_get_headers(enc, &daala_headers);
  if (ret < 0) return -1;
  ogg_setup.tracks = 1;
  ogg_setup.mem = kr_alloc(kr_ogg_muxer_sizeof());
  ogg_setup.header[0] = daala_headers;
  printk("header 0 sz %d", ogg_setup.header[0].data[0].sz);
  printk("header 1 sz %d", ogg_setup.header[0].data[1].sz);
  printk("header 2 sz %d", ogg_setup.header[0].data[2].sz);
  ogg = kr_ogg_muxer_init(&ogg_setup);
  if (!ogg) return -1;
  ret = kr_ogg_muxer_read_header(ogg, buf, BUFSIZE);
  printk("Ogg muxed header size: %zd", ret);
  if (ret < 0) return -1;
  ret = write(fd, buf, ret);
  if (ret <= 0) return -1;
  out_frame.coded.data = coded_buf;
  ret = kr_daala_encode(enc, &out_frame, &in_frame);
  if (ret != 0) {
    printk("encode failed %d", ret);
    return -1;
  }
  od.framing_buf = buf;
  od.sz = BUFSIZE;
  od.track = 0;
  od.in[0].granule_pos = 0;
  od.in[0].buf = out_frame.coded.data;
  od.in[0].sz = out_frame.coded.info.sz;
  od.in_len = 1;
  ret = kr_ogg_mux(ogg, &od);
  printk("og mux ret is %zu - vec len is %d", ret, od.out_len);
  for (i = 0; i < od.out_len; i++) {
    printk("part %d writing %zu bytes", i, od.out[i].sz);
    ret = write(fd, od.out[i].buf, od.out[i].sz);
  }
  kr_image_convert_clear(&conv);
  kr_daala_encoder_destroy(enc);
  close(fd);
  free(img_setup.mem);
  free(enc);
  free(buf);
  free(coded_buf);
  free(ogg);
  return 0;
}

static void print_usage() {
  printk("  ./daala_enc_test in.png out.ogg");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = daala_enc_test(argv[2], argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
