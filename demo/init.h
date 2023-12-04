#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "doc/1a2b3c/stdiov.h"

typedef struct {
  char label[256];
  char name[256];
  struct iovec iovec[256];
} labeled_iovec;

static char *V = "lush model 26 system demo";

typedef struct {
  labeled_iovec mem[26];
  struct utsname uname;
} lush_t;

typedef struct {
 ino_t        d_ino;    /* 64-bit inode number */
 off_t        d_off;    /* 64-bit offset to next structure */
 unsigned short int d_reclen; /* Size of this dirent */
 unsigned char  d_type;   /* File type */
 char           d_name[256]; /* Filename (null-terminated) */
} dir_entry;

/*ssize_t getdents64(int fd, void dirp[.count], size_t count);*/
ssize_t get_dir_ls(unsigned int fd, dir_entry *dirp,
                   unsigned int count) {
  return syscall(SYS_getdents64, fd, dirp, count);
}

int scan(lush_t *d) {
  if (!d) return -1;
  int ret;
  ret = 0;
  int uid;
  int euid;
  ret = uname(&d->uname);
  if (ret) {
    printf("wtf uname error: %s\n", strerror(errno));
    exit(1);
  }
  /*
  printf("sysname: %s\n", d->uname.sysname);
  printf("nodename: %s\n", d->uname.nodename);
  printf("release: %s\n", d->uname.release);
  printf("version: %s\n", d->uname.version);
  printf("machine: %s\n", d->uname.machine);*/
  uid = getuid();
  //printf("uid: %d\n", uid);
  euid = geteuid();
  //printf("euid: %d\n", euid);
  if (uid + euid != 0) {
    ret = setuid(0);
    if (ret) {
      printf("setuid: %s\n", strerror(errno));
      exit(1);
    }
  }
  ret = chdir("/");
  if (ret) {
    printf("chdir: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s operating LK %s on %s\n", V, d->uname.release, d->uname.machine);
  return 0;
}

int init_demo(int argc, char *argv[]) {
  lush_t *d;
  int ret;
  d = calloc(1, sizeof(lush_t));
  if (d) printf("cAlloc'ed %lu byte mistake buffer\n", sizeof(lush_t));
  ret = 0;
  ret = scan(d);
  if (ret) return ret;
  free(d);
  return ret;
}

int run_demo(int argc, char *argv[]) {
  int ret;
  ret = 0;
  if (1) {
    ret = init_demo(argc, argv);
  }
  printf("%s ran proper: %d\n", V, ret);
  return ret;
}
