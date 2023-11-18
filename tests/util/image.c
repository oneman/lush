#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <cairo/cairo.h>
#include <turbojpeg.h>
#include <krad/image/image.h>
#include <krad/image/jpeg.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "image.h"

static int coded_image_str_to_fmt(char *str) {
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

static int kr_image_to_jpeg_file(int fd, kr_image *image) {
  int ret;
  kr_coded_image cimage;
  if (fd < 0 || image == NULL) return 1;
  memset(&cimage, 0 , sizeof(cimage));
  kr_coded_image_data_alloc(&cimage, image->info.w * image->info.h * 4);
  ret = kr_image_to_jpeg(&cimage, image);
  ret = kr_coded_image_to_file(fd, &cimage);
  kr_coded_image_data_free(&cimage);
  return ret;
}

static int kr_image_from_jpeg_file(kr_image *image, int fd, char *path) {
  kr_coded_image *cimage;
  int res;
  if (fd < 0 || image == NULL) return 1;
  cimage = kr_allocz(1, sizeof(*cimage));
  res = kr_coded_image_from_file(cimage, fd, path);
  if (res != 0) {
    free(cimage);
    return 1;
  }
  kr_image_from_jpeg(image, cimage);
  kr_coded_image_data_free(cimage);
  free(cimage);
  return 0;
}

static int kr_image_to_png_file(int fd, kr_image *image) {
  int ret;
  kr_coded_image cimage;
  if (fd < 0 || image == NULL) return 1;
  memset(&cimage, 0 , sizeof(cimage));
  kr_coded_image_data_alloc(&cimage, image->info.w * image->info.h * 4);
  ret = kr_image_to_png(&cimage, image);
  ret = kr_coded_image_to_file(fd, &cimage);
  kr_coded_image_data_free(&cimage);
  return ret;
}

static int kr_image_from_png_file(kr_image *image, int fd, char *path) {
  kr_coded_image *cimage;
  int res;
  if (fd < 0 || image == NULL) return 1;
  cimage = kr_allocz(1, sizeof(*cimage));
  res = kr_coded_image_from_file(cimage, fd, path);
  if (res != 0) {
    free(cimage);
    return 1;
  }
  kr_image_from_png(image, cimage);
  kr_coded_image_data_free(cimage);
  free(cimage);
  return 0;
}

int kr_image_to_jpeg_filename(char *filename, kr_image *image) {
  int ret;
  int fd;
  if (image == NULL || filename == NULL) return 1;
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return 1;
  ret = kr_image_to_jpeg_file(fd, image);
  close(fd);
  return ret;
}

int kr_image_from_jpeg_filename(kr_image *image, char *filename) {
  int ret;
  int fd;
  if (image == NULL || filename == NULL) return 1;
  fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return 1;
  ret = kr_image_from_jpeg_file(image, fd, filename);
  close(fd);
  return ret;
}

int kr_image_to_png_filename(char *filename, kr_image *image) {
  int ret;
  int fd;
  if (image == NULL || filename == NULL) return 1;
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return 1;
  ret = kr_image_to_png_file(fd, image);
  close(fd);
  return ret;
}

int kr_image_from_png_filename(kr_image *image, char *filename) {
  int ret;
  int fd;
  if (image == NULL || filename == NULL) return 1;
  fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return 1;
  ret = kr_image_from_png_file(image, fd, filename);
  close(fd);
  return ret;
}

int kr_image_load_file(kr_image *img, char *filename) {
  int fmt;
  int ret;
  char *ext;
  if (img == NULL || filename == NULL) return 1;
  ext = strrchr(filename, '.');
  if (ext == NULL) return 1;
  fmt = coded_image_str_to_fmt(&ext[1]);
  if (fmt == 0) return 1;
  switch (fmt) {
    case KR_CODED_IMAGE_PNG:
      ret = kr_image_from_png_filename(img, filename);
      break;
    case KR_CODED_IMAGE_JPG:
      ret = kr_image_from_jpeg_filename(img, filename);
      break;
    default: return 1;
  }
  return ret;
}

int kr_image_save_file(char *filename, kr_image *img) {
  int fmt;
  int ret;
  char *ext;
  if (img == NULL || filename == NULL) return 1;
  ext = strrchr(filename, '.');
  if (ext == NULL) return 1;
  fmt = coded_image_str_to_fmt(&ext[1]);
  if (fmt == 0) return 1;
  switch (fmt) {
    case KR_CODED_IMAGE_PNG:
      ret = kr_image_to_png_filename(filename, img);
      break;
    case KR_CODED_IMAGE_JPG:
      ret = kr_image_to_jpeg_filename(filename, img);
      break;
    default: return 1;
  }
  return ret;
}

int kr_coded_image_from_file(kr_coded_image *cimage, int fd, char *path) {
  struct stat st;
  size_t sz;
  size_t pos;
  ssize_t res;
  char *ext;
  if (fd < 0 || cimage == NULL || path == NULL) return -1;
  fstat(fd, &st);
  sz = st.st_size;
  if (sz <= 0) return -1;
  ext = strrchr(path, '.');
  if (ext == NULL) return -1;
  kr_coded_image_data_alloc(cimage, sz);
  pos = 0;
  do {
    res = read(fd, cimage->data + pos, sz - pos);
    if (res < 0) {
      kr_coded_image_data_free(cimage);
      return -1;
    }
    pos += res;
  } while (res > 0);
  cimage->info.sz = pos;
  cimage->info.image.fmt = coded_image_str_to_fmt(&ext[1]);
  if (cimage->info.image.fmt == 0) {
    kr_coded_image_data_free(cimage);
    return -1;
  }
  return 0;
}

int kr_coded_image_to_file(int fd, kr_coded_image *coded_image) {
  ssize_t res;
  int total;
  int pos;
  if (fd < 0) return 1;
  pos = 0;
  total = coded_image->info.sz;
  while (total > 0) {
    res = write(fd, &coded_image->data[pos], total);
    total -= res;
    pos += res;
    if (res < 0) return 1;
  }
  return 0;
}

int kr_coded_image_load_file(kr_coded_image *cimage, char *filename) {
  int fd;
  int res;
  if (filename == NULL) return -1;
  fd = open(filename, O_RDONLY);
  if (fd < 0) return -1;
  res = kr_coded_image_from_file(cimage, fd, filename);
  if (res != 0) return -1;
  close(fd);
  return 0;
}

int kr_coded_image_save_file(char *filename, kr_coded_image *cimage) {
  int res;
  int fd;
  if (filename == NULL || cimage == NULL) return -1;
  fd = open(filename, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) return -1;
  res = kr_coded_image_to_file(fd, cimage);
  if (res != 0) return -1;
  close(fd);
  return 0;
}
