#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/web/encoding/gzip.h>

int gzip_test(char *action) {
  int ret;
  kr_gzip *gz;
  kr_gzip_data data;
  kr_gzip_setup setup;
  uint8_t bufs[2][8192];
  setup.type = 0;
  if (!strcmp(action, "deflate")) setup.type = KR_GZIP_DEFLATE;
  if (!strcmp(action, "inflate")) setup.type = KR_GZIP_INFLATE;
  if (!setup.type) return -1;
  setup.mem = kr_alloc(kr_gzip_sizeof());
  gz = kr_gzip_init(&setup);
  if (!gz) return -2;
  data.in = bufs[0];
  data.out = bufs[1];
  data.out_sz = sizeof(bufs[1]);
  for (;;) {
    ret = read(0, bufs[0], 512);
    if (ret < 0) return -4;
    if (ret == 0) {
      do {
        data.in_sz = 0;
        if (setup.type == KR_GZIP_DEFLATE) {
          ret = kr_gzip_deflate(gz, &data, 1);
        } else {
          ret = kr_gzip_inflate(gz, &data, 1);
        }
        if (ret < 0) return -6;
        //printke("flush %d %d %d", ret, data.read, data.wrote);
        ret = write(1, data.out, data.wrote);
        if (ret < 0) return -5;
      } while (data.wrote == data.out_sz);
      break;
    }
    data.in_sz = ret;
    do {
      if (setup.type == KR_GZIP_DEFLATE) {
        ret = kr_gzip_deflate(gz, &data, 0);
      } else {
        ret = kr_gzip_inflate(gz, &data, 0);
      }
      if (ret < 0) return -6;
        //printke("%d %d %d", ret, data.read, data.wrote);
      ret = write(1, data.out, data.wrote);
      if (ret < 0) return -5;
    } while (data.wrote == data.out_sz);
  }
  free(gz);
  return 0;
}

static void print_usage(char *cmd) {
  printk("Usage:");
  printk("  cat test.txt | %s deflate > out.gz", cmd);
  printk("  cat test.gz | %s inflate > out.txt", cmd);
  printk("  curl -H 'Accept-Encoding: gzip' https://www.google.com | %s inflate", cmd);
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage(argv[0]);
    return 1;
  }
  ret = gzip_test(argv[1]);
  if (ret) {
    printke("Failure! %d", ret);
  }
  return ret;
}
