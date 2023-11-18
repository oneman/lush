#include <stdio.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/video/gif.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
#include <math.h>
#include "../util/image.h"

int coded_image_str_to_fmt(char *str) {
  if (!strncmp(str,"png",3) || !strncmp(str,"PNG",3)) {
    return KR_CODED_IMAGE_PNG;
  } else if (!strncmp(str,"jpg",3) || !strncmp(str,"JPG",3)
    || !strncmp(str,"jpeg",4) || !strncmp(str,"JPEG",4)) {
    return KR_CODED_IMAGE_JPG;
  } else if (!strncmp(str,"gif",3) || !strncmp(str,"GIF",3)) {
    return KR_CODED_IMAGE_GIF;
  }
  return 0;
}

void print_usage(char *cmd) {
  printf("Converts and resizes/scales a src image into a dst image.\n");
  printf("Usage: %s dst src dst_w dst_h\n",cmd);
  printf("i.e. %s test.png test.jpg 1024 768\n",cmd);
  printf("i.e. %s test.png test.jpg 20%%\n",cmd);
}

int frame_from_gif_filename(kr_frame *dst, char *filename) {
  int ret;
  kr_file *file;
  kr_data data;
  kr_gif *gif;
  kr_gif_info info;
  uint8_t *buffer;
  int sz;
  ret = 0;
  file = kr_file_open(filename);
  if (file == NULL) return -1;
  sz = kr_file_size(file);
  buffer = kr_allocz(1, sz);
  ret = kr_file_read(file, buffer, sz);
  data.info.sz = sz;
  data.buf = buffer;
  kr_file_close(&file);
  gif = kr_gif_open(&data);
  if (gif == NULL) {
    printke("reading gif");
    return -1;
  }
  ret = kr_gif_get_info(gif, &info);
  printk("%d x %d gif", info.w, info.h);
  dst->image.info.w = info.w;
  dst->image.info.h = info.h;
  dst->image.ps[0] = info.w * 4;
  kr_image_pixels_alloc(&dst->image);
  ret = kr_gif_get_frame(gif, dst);
  if (ret != 1) return 1;
  kr_gif_close(gif);
  free(buffer);
  return 0;
}

int image_scale_factor(kr_image *dst, kr_image *src, float factor) {
  dst->info.w = src->info.w * factor;
  dst->info.w = ((dst->info.w - 1) | 7) + 1; // nearest 8 multiple
  dst->info.h = src->info.h * factor;
  dst->info.h = ((dst->info.h - 1) | 7) + 1; // nearest 8 multiple
  return 0;
}

int main(int argc, char *argv[]) {
  char *src_ext;
  char *dst_ext;
  int src_fmt;
  int dst_fmt;
  int ret;
  float factor;
  kr_image src_img;
  kr_image dst_img;
  kr_image *src;
  kr_frame src_frame;
  kr_convert conv;
  if (argc != 3 && argc != 4 && argc != 5) {
    print_usage(argv[0]);
    return 1;
  }
  memset(&src_img, 0 , sizeof(kr_image));
  dst_ext = strrchr(argv[1], '.');
  src_ext = strrchr(argv[2], '.');
  if (!dst_ext || !src_ext) {
    printke("Invalid src/dst images extensions.");
    return 1;
  }
  dst_fmt = coded_image_str_to_fmt(&dst_ext[1]);
  if (dst_fmt == 0) {
    printke("Invalid dst image format %s.",&dst_ext[1]);
    return 1;
  }
  src_fmt = coded_image_str_to_fmt(&src_ext[1]);
  if (src_fmt == 0) {
    printke("Invalid src image format %s.",&src_ext[1]);
    return 1;
  }
  switch (src_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_from_png_filename(&src_img, argv[2]);
      src = &src_img;
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_from_jpeg_filename(&src_img, argv[2]);
      src = &src_img;
      break;
    }
    case KR_CODED_IMAGE_GIF: {
      ret = frame_from_gif_filename(&src_frame, argv[2]);
      src = &src_frame.image;
      break;
    }
    default : printke("Sorry, can't handle this src image format"); return 1;
  }
  if (ret != 0) {
    printke("Error!");
    return 1;
  }
  if (argc > 3) {
    memset(&dst_img, 0 , sizeof(kr_image));
    kr_image_convert_init(&conv);
    if (argc == 4) {
      factor = strtof(argv[3], NULL) / 100.00f;
      if (factor == 0) return 1;
      image_scale_factor(&dst_img, src, factor);
      printf("scaling from %dx%d to %dx%d\n",
        src->info.w,src->info.h,dst_img.info.w,dst_img.info.h);
    } else {
      dst_img.info.w = atoi(argv[3]);
      dst_img.info.h = atoi(argv[4]);
    }
    dst_img.info.fmt = KR_PIXELS_ARGB;
    dst_img.ps[0] = dst_img.info.w * 4;
    src->info.fmt = KR_PIXELS_ARGB;
    kr_image_pixels_alloc(&dst_img);
    ret = kr_image_convert(&conv, &dst_img, src);
    if (ret != 0) {
      printke("Error!");
      return 1;
    }
    kr_image_pixels_free(src);
    src = &dst_img;
  }
  switch (dst_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_to_png_filename(argv[1], src);
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_to_jpeg_filename(argv[1], src);
      break;
    }
    default : printke("Sorry, can't handle this dst image format"); return 1;
  }
  if (ret != 0) {
    printke("Error!");
    return 1;
  }
  kr_image_pixels_free(src);
  printf("Done!\n");
  return 0;
}
