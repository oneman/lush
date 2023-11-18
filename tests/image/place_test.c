#include <stdio.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/video/gif.h>
#include <krad/image/convert.h>
#include <krad/image/insert.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
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
  printf("Places src img into dst img at x/y coordinates.\n");
  printf("Usage: %s dst src x y\n",cmd);
  printf("i.e. %s dst.png src.png 100 150 out.png\n",cmd);
}

int main(int argc, char *argv[]) {
  char *src_ext;
  char *dst_ext;
  char *out_ext;
  kr_image src_img;
  kr_image dst_img;
  int src_fmt;
  int dst_fmt;
  int out_fmt;
  kr_rect crop;
  kr_rect area;
  int ret;
  float alpha;
  float rot;
  rot = 0;
  alpha = 1.0f;
  if (argc < 6) {
    print_usage(argv[0]);
    return 1;
  }
  memset(&src_img, 0 , sizeof(kr_image));
  memset(&dst_img, 0 , sizeof(kr_image));
  dst_ext = strrchr(argv[1], '.');
  src_ext = strrchr(argv[2], '.');
  out_ext = strrchr(argv[5], '.');
  area.x = atoi(argv[3]);
  area.y = atoi(argv[4]);
  if (argc > 6) alpha = atof(argv[6]);
  if (argc > 7) rot = atof(argv[7]);
  if (!dst_ext || !src_ext || !out_ext) {
    printke("Invalid src/dst/out images extensions.");
  }
  dst_fmt = coded_image_str_to_fmt(&dst_ext[1]);
  src_fmt = coded_image_str_to_fmt(&src_ext[1]);
  out_fmt = coded_image_str_to_fmt(&out_ext[1]);
  if (dst_fmt == 0 || src_fmt == 0 || out_fmt == 0) {
    return 1;
  }
  switch (src_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_from_png_filename(&src_img, argv[2]);
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_from_jpeg_filename(&src_img, argv[2]);
      break;
    }
    default: return 1;
  }
  if (ret != 0) {
    printke("Error!");
    return 1;
  }
  switch (dst_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_from_png_filename(&dst_img, argv[1]);
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_from_jpeg_filename(&dst_img, argv[1]);
      break;
    }
    default: return 1;
  }
  if (ret != 0) {
    printke("Error!");
    return 1;
  }
  kr_insert insert;
  crop.x = 0 * 2;
  crop.y = 0 * 2;
  crop.w = src_img.info.w - 0;
  crop.h = src_img.info.h - 0;
  area.w = dst_img.info.w;
  area.h = dst_img.info.h;
  insert.src_crop = crop;
  insert.dst_area = area;
  insert.rotation = rot;
  insert.opacity = alpha;
  ret = kr_image_insert_adv(&dst_img, &src_img, &insert);
  //ret = kr_image_insert(&dst_img, &src_img, &area);
  if (ret != 0) {
    printke("Error placing image! %d", ret);
    return 1;
  }
  switch (out_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_to_png_filename(argv[5], &dst_img);
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_to_jpeg_filename(argv[5], &dst_img);
      break;
    }
    default: return 1;
  }
  if (ret != 0) {
    printke("Error writing dst img!");
    return 1;
  }
  kr_image_pixels_free(&src_img);
  kr_image_pixels_free(&dst_img);
  printf("All done!\n");
  return 0;
}
