#include <stdio.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/image/crop.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/video/gif.h>
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
  printf("Saves the cropped subimage of a given src image.\n");
  printf("Usage: %s dst src w h x y\n",cmd);
  printf("i.e. %s dst.png src.png 320 240 100 50\n",cmd);
}

int main(int argc, char *argv[]) {
  char *src_ext;
  char *sub_ext;
  int src_fmt;
  int sub_fmt;
  int ret;
  kr_rect crop;
  kr_image src_img;
  kr_image sub_img;
  if (argc != 7) {
    print_usage(argv[0]);
    return 1;
  }
  memset(&src_img, 0 , sizeof(src_img));
  memset(&sub_img, 0 , sizeof(sub_img));
  memset(&crop, 0 , sizeof(crop));
  sub_ext = strrchr(argv[1], '.');
  src_ext = strrchr(argv[2], '.');
  if (!sub_ext || !src_ext) {
    printke("Invalid src/dst images extensions.");
    return 1;
  }
  sub_fmt = coded_image_str_to_fmt(&sub_ext[1]);
  if (sub_fmt == 0) {
    printke("Invalid sub image format %s.",&sub_ext[1]);
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

  crop.x = atoi(argv[5]);
  crop.y = atoi(argv[6]);
  crop.w = atoi(argv[3]);
  crop.h = atoi(argv[4]);
  ret = kr_image_crop(&sub_img, &src_img, &crop);

  if (ret != 0) {
    printke("Error!");
    return 1;
  }

  switch (sub_fmt) {
    case KR_CODED_IMAGE_PNG: {
      ret = kr_image_to_png_filename(argv[1], &sub_img);
      break;
    }
    case KR_CODED_IMAGE_JPG: {
      ret = kr_image_to_jpeg_filename(argv[1], &sub_img);
      break;
    }
    default: return 1;
  }

  if (ret != 0) {
    printke("Error!");
    return 1;
  }

  kr_image_pixels_free(&src_img);
  printf("Done!\n");
  return 0;
}
