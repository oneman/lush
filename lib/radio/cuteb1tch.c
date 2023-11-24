typedef struct {
  uint8_t bytes[4096];
} boat;


int cbev(kr_file_event *ev) {
  /*uint8_t *user_ptr;
  user_ptr = ev->user;
  printf("cbev\n");*/
  return 0;
}

#include "/home/demo/src/rad/doc/1a2b3c/stdiov.h"

static void filelist(uint8_t *bits, uint64_t sz) {
  uint64_t b;
  int wasntext = 0;
  int wasatext = 0;
  uint8_t byte;
  for (b = 0; b < sz; b++) {
    byte = bits[b];
    if ((wasatext == 78) || (wasntext == 78)) {
      printf("b: %lu wasatext: %d wasntext %d\n", b, wasatext, wasntext);
      exit(1);
    }
    if (!isascii_splf(byte)) {
      putchar(byte);
      if (wasntext) {
        wasntext = 0;
      }
      wasatext++;
    } else {
      wasntext++;
      if (wasatext) {
        putchar(LF);
        wasatext = 0;
      }
    }
  }
}

static void sysrecheck(void) {
  //printf("demo running!!!\n");
  int ret;
  static boat demo;
  static kr_fs_setup setup;
  setup.nfiles = 64;
  setup.user = &demo;
  setup.event_cb = cbev;
  static kr_file_set *set;
  set = kr_file_set_create(&setup);
  kr_file2 *in;
  char *path = "/home/demo/src/rad/motizuki/Inter-universal Teichmuller Theory I.txt";
  size_t len;
  len = strlen(path);
  in = kr_file2_open(set, path, len);
  kr_file2_info info;
  ret = kr_file2_get_info(in, &info);
  if (ret) printf("ret: %d\n", ret);
  uint8_t *bits = kr_file2_get_data(in);
  if (!bits) {
    printf("fubar\n");
    exit(1);
  }
  /*printf("path: %s\n", info.path);
  printf("lastmod: %ld\n", info.lastmod);
  printf("sz: %lu\n", info.sz);*/
  filelist(bits, info.sz);
  ret = kr_file2_close(in);
  if (ret) printf("ret: %d\n", ret);
  ret = kr_file_set_destroy(set);
  if (ret) printf("ret: %d\n", ret);
}
