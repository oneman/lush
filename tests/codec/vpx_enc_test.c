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
#include <krad/codec/video/vpx/encoder.h>
#include "../util/image.h"

#define fourcc 0x30385056
#define IVF_FRAME_HDR_SZ (12)

static void mem_put_le16(char *mem, unsigned int val) {
  mem[0] = val;
  mem[1] = val>>8;
}

static void mem_put_le32(char *mem, unsigned int val) {
  mem[0] = val;
  mem[1] = val>>8;
  mem[2] = val>>16;
  mem[3] = val>>24;
}

static void write_ivf_file_header(int fd, kr_frame *in, int frames) {
  int ret;
  char header[32];
  header[0] = 'D';
  header[1] = 'K';
  header[2] = 'I';
  header[3] = 'F';
  mem_put_le16(header+4,  0);                   /* version */
  mem_put_le16(header+6,  32);                  /* headersize */
  mem_put_le32(header+8,  fourcc);              /* headersize */
  mem_put_le16(header+12, in->image.info.w);            /* width */
  mem_put_le16(header+14, in->image.info.h);            /* height */
  mem_put_le32(header+16, 30); /* rate */
  mem_put_le32(header+20, 1); /* scale */
  mem_put_le32(header+24, frames);           /* length */
  mem_put_le32(header+28, 0);                   /* unused */
  ret = write(fd, header, 32);
  if (ret != 32) return;
}

static void write_ivf_frame_header(int fd, kr_frame *out) {
  int ret;
  char header[12];
  mem_put_le32(header, out->coded.info.sz);
  mem_put_le32(header+4, out->info.tc & 0xFFFFFFFF);
  mem_put_le32(header+8, out->info.tc >> 32);
  ret = write(fd, header, 12);
  if (ret != 12) return;
}

static int vpx_enc_test(char *fout, char *fin) {
  int i;
  size_t offset;
  ssize_t ret;
  int fd;
  kr_vpx_encoder_setup setup;
  kr_vpx_encoder *enc;
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
  setup.mem = kr_alloc(kr_vpx_encoder_sizeof());
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
  enc = kr_vpx_encoder_init(&setup);
  if (!enc) return -1;
  in_frame.image = raw_yuv;
  in_frame.type = KR_IMAGE_RAW;
  offset = 0;
  write_ivf_file_header(fd, &in_frame, 300);
  offset += 32;
  out_frame.coded.data = kr_alloc(raw_yuv.info.w * raw_yuv.info.h * 3);
  // 10 seconds still image video
  for (i = 0; i < 300; i++) {
    ret = kr_vpx_encode(enc, &out_frame, &in_frame);
    if (ret < 0) {
      printk("encode failed %d", ret);
      return -1;
    }
    write_ivf_frame_header(fd, &out_frame);
    offset += 12;
    lseek(fd, offset, SEEK_SET);
    ret = write(fd, out_frame.coded.data, out_frame.coded.info.sz);
    offset += out_frame.coded.info.sz;
    lseek(fd, offset, SEEK_SET);
    if (ret < 0) return -1;
  }
  kr_vpx_encoder_destroy(enc);
  kr_image_convert_clear(&conv);
  close(fd);
  free(out_frame.coded.data);
  free(raw_yuv.px[0]);
  free(enc);
  return 0;
}

static void print_usage() {
  printk("  ./vpx_enc_test in.png out.ivf");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) {
    print_usage();
    return 1;
  }
  ret = vpx_enc_test(argv[2], argv[1]);
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
