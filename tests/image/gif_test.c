#include <stdio.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/video/gif.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/image/png.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
#include "../util/image.h"

int run_test(char *filename) {
  char outfilename[1024];
  int ret;
  kr_file *file;
  kr_frame frame;
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
  printk("Opened %s %zu bytes", filename, sz);
  ret = kr_file_read(file, buffer, sz);
  printk("Read %d bytes", ret);
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
  frame.image.info.w = info.w;
  frame.image.info.h = info.h;
  frame.image.ps[0] = info.w * 4;
  kr_image_pixels_alloc(&frame.image);
  int i = 0;
  do {
    memset(frame.image.px[0], 0, info.w * info.h * 4);
    if (1) { //(!(i % 5)) {
      ret = kr_gif_get_frame(gif, &frame);
      snprintf(outfilename, sizeof(outfilename), "%s_%"PRIi64".png", filename, frame.info.tc);
      kr_image_to_png_filename(outfilename, &frame.image);
    } else {
      ret = kr_gif_skip_frame(gif);
    }
    i++;
  } while (ret == 1);
  kr_gif_close(gif);
  free(buffer);
  kr_image_pixels_free(&frame.image);
  return ret;
}

int main (int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    printke("need gif filename");
    return 1;
  }
  ret = run_test(argv[1]);
  return ret;
}
