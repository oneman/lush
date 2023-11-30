#include <krad/io/file2.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <errno.h>
#include "doc/1a2b3c/stdiov.h"

typedef struct {
  char label[256];
  char name[256];
  struct iovec iovec[256];
} labeled_iovec;

/*
struct iovec {
  void   *iov_base;
  size_t  iov_len;
};
*/

typedef struct {
  labeled_iovec mem[2601];
  struct utsname uname;
} demo_t;

static demo_t demo_heap;

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
  demo_t *d;
  d = &demo_heap;
  int ret;
  ret = 0;
  int len;
  len = 0;
  int uid;
  int euid;
  char *path;
  path = NULL;
  kr_file2 *file;
  file = NULL;
  static kr_fs_setup setup;
  setup.nfiles = 64;
  setup.user = d;
  setup.event_cb = cbev;
  static kr_file_set *fs;
  fs = kr_file_set_create(&setup);
  checkpath(fs, "/dev/mei0"); /* LOL */
  ret = uname(&d->uname);
  if (ret) {
    printf("wtf uname error: %s\n", strerror(errno));
    exit(1);
  }
  printf("sysname: %s\n", d->uname.sysname);
  printf("nodename: %s\n", d->uname.nodename);
  printf("release: %s\n", d->uname.release);
  printf("version: %s\n", d->uname.version);
  printf("machine: %s\n", d->uname.machine);
  uid = getuid();
  printf("uid: %d\n", uid);
  euid = geteuid();
  printf("euid: %d\n", euid);
  if (uid + euid != 0) {
    ret = setuid(0);
    if (ret) {
      printf("setuid: %s\n", strerror(errno));
      exit(1);
    }
  }
  /*
  checkpath(fs, "/");
  checkpath(fs, "/proc/self/environ");
  checkpath(fs, "/proc/self/mounts");
  checkpath(fs, "/dev/null");
  checkpath(fs, "/bin/ls");
  checkpath(fs, "/usr/bin/demo");
  checkpath(fs, "/home/demo/src/lush/lush.symlink");
  checkpath(fs, "/home/demo/src/lush/bad.symlink");
  checkpath(fs, "/home/demo/src/lush/demo/.libs/demo");
  checkpath(fs, "/home/demo/src/lush/demo/demo");
  uint8_t some_page[4096 * 2];
  path = "/proc/self/environ";
  len = strlen(path);
  file = kr_file2_open(fs, path, len);
  ret = kr_file2_read(file, some_page, sizeof(some_page));
  printf("Read %d bytes from %s\n", ret, path);
  if (ret == sizeof(some_page)) {
    printf("wtf ur environment is to fking big\n");
    exit(1);
  }
  */
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
