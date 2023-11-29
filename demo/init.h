#include <krad/io/file2.h>
#include "doc/1a2b3c/stdiov.h"

static struct iovec demomem[(26 * 10 * 10) + 1];

int cbev(kr_file_event *ev) {
  /*uint8_t *user_ptr;
  user_ptr = ev->user;
  printf("cbev\n");*/
  return 0;
}

static void uwls(uint8_t *bits, uint64_t sz) {
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

static int checkpath(kr_file_set *fs, char *path) {
  int ret;
  kr_file2 *in;
  size_t len;
  len = strlen(path);
  in = kr_file2_open(fs, path, len);
  if (!in) {
    printf("We could not open a file: %s\n", path);
    ret = 1;
  }
  if (ret == 0) { 
    kr_file2_info info;
    ret = kr_file2_get_info(in, &info);
    if (ret) {
      printf("Could not get file info: %d\n", ret);
      return ret;
    }
    uint8_t *bits = kr_file2_get_data(in);
    if (!bits) {
      printf("Could not get the bits for: %s\n", info.path);
      ret += kr_file2_close(in);
      ret++;
      if (ret) printf("ret: %d\n", ret);
    }
  }
  return ret;
}

/*
 *
 * How many files out of all the files I can read
 * have 1 partition when partitioned by byte LF
 * and are larger than PATH_MAX ie 4096
 *
 */

/* explain everything in the universe from the context
 * of this one program parsing all the files ;) */
int discover_environment(void) {
  int ret;
  ret = 0;
  static kr_fs_setup setup;
  setup.nfiles = 64;
  setup.user = &demomem;
  setup.event_cb = cbev;
  static kr_file_set *fs;
  fs = kr_file_set_create(&setup);
  checkpath(fs, "/");
  checkpath(fs, "/proc/self/environ");
  checkpath(fs, "/proc/self/mounts");
  checkpath(fs, "/dev/null");
  checkpath(fs, "/bin/ls");
  checkpath(fs, "/usr/bin/demo");  
  ret = kr_file_set_destroy(fs);
  if (ret) printf("ret: %d\n", ret);
  return ret;
}

int seek_lush_source(void) {
  int ret;
  ret = 0;
  /*reconstruct_argc?*/
  /* Find lush src dir cd cwd $HOME/src/lush" *
   [~/src/lush]% pwd
/home/demo/src/lush
[~/src/lush]% cat README.md | grep "# LUSH" | wc -l
1
[~/src/lush]%
   */
  return ret;
}

/* do i have the lush src?
 * what username what cpu how much ram how much
 * disk space what kernel version lib uring? *
 * what date and time is it?
 */

int init_demo(int argc, char *argv[]) {
  int ret;
  ret = 0;
  ret = discover_environment();
  if (ret) return ret;
  ret = seek_lush_source();
  return ret;
}

int run_demo(int argc, char *argv[]) {
  int ret;
  ret = 0;
  if (1) {
    ret = init_demo(argc, argv);
  }
  printf("Demo runs proper!\n");
  return ret;
}
