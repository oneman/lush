#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/image/png.h>
#include <krad/image/jpeg.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/mem/mem.h>

#include "../util/image.h"

static void align_test() {
  int i;
  int p;
  printk("Free align_test() with your usage information!");
  for (i = 0; i < 4096; i += 8) {
    p = kr_image_axis_padded(i);
    printk("Width: %d Padded is %d", i, p);
  }
}

static int kmb(kr_image *img) {
  double x, xx, y, cx, cy;
  int i;
  int hx, hy;
  int itermax = 100;      /* how many iterations to do */
  double magnify = 1.0;   /* no magnification */
  for (hy = 1; hy < img->info.h; hy++) {
    for (hx = 1; hx < img->info.w; hx++) {
      cx = (((float)hx)/((float)img->info.w) - 0.5) / magnify * 3.0 - 0.7;
      cy = (((float)hy)/((float)img->info.h) - 0.5) / magnify * 3.0;
      x = 0.0;
      y = 0.0;
      for (i = 1; i < itermax; i++) {
        xx = x * x - y * y + cx;
        y = 2.0 * x * y + cy;
        x = xx;
        if (x * x + y * y > 100.0) {
          /* anti-hit */
          img->px[0][(img->ps[0] * hy) + (hx * 4)] = 180;
        //img->px[0][hx * hy * 4] = 0;
        //img->px[0][hx * hy * 4 + 1] = 0;
        //img->px[0][hx * hy * 4 + 2] = 255;
       // img->px[0][hx * hy * 4 + 3] = 255;
          break;
        }
      }
      if (i == itermax) {
        /* hit */
        img->px[0][(img->ps[0] * hy) + (hx * 4) + 2] = 90;
        //img->px[0][hx * hy * 4] = 0;
        //img->px[0][(img->ps[0] * hy) + (hx * 4) + 2] = 110;
        //img->px[0][(img->ps[0] * hy) + (hx * 4) + 2] = 110;
        if (img->px[0][hx * hy * 4] < 130) {
          img->px[0][hx * hy * 4 + 2] = 130;
        } else {
          img->px[0][hx * hy * 4 + 2] += 10;
        }
        //img->px[0][hx * hy * 4 + 2] = 0;
        //img->px[0][hx * hy * 4 + 3] = 0;
      }
    }
  }
  return 0;
}

static int image_color_test(char **args) {
  ssize_t ret;
  kr_image_info img_info;
  kr_image_setup img_setup;
  kr_image img;
  memset(&img_info, 0, sizeof(img_info));
  memset(&img_setup, 0, sizeof(img_setup));
  img_info.w = atoi(args[1]);
  img_info.h = atoi(args[2]);
  img_info.fmt = KR_PIXELS_ARGB;
  ret = kr_image_sizeof(&img_info);
  if (ret <= 0) return -2;
  img_setup.mem = kr_alloc(ret);
  img_setup.img = &img;
  img_setup.info = img_info;
  ret = kr_image_init2(&img_setup);
  if (ret != 0) return -3;
  ret = kr_image_fill(&img, atoi(args[3]), atoi(args[4]), atoi(args[5]));
  if (ret != 0) return -4;
  ret = kmb(&img);
  if (ret != 0) return -5;
  ret = kr_image_save_file(args[0], &img);
  return ret;
}

static void print_usage() {
  printk("  ./image_color_test name.[png|jpg] width height r g b");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 7) {
    print_usage();
    align_test();
    return 1;
  }
  ret = image_color_test(&argv[1]);
  if (ret) {
    printk("Failure! %d", ret);
  }
  return ret;
}
