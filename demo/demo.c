#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include "doc/1a2b3c/stdiov.h"

#define P26GES (PAGE_SZ * 26)

static char *NAMESYSTEM = "lush";
static char *SYSDESC = "cybernetic metafunctional morphonymic reconstructive";
static char *PROGRAMNAME = "demo";
static char *VERSION = "26";

typedef struct {
  char label[32];
  void *b;
  size_t sz;
} lamemory;

typedef struct {
  lamemory mem[26];
  struct utsname uname;
} lush_t;

typedef struct {
 ino_t d_ino; /* 64-bit inode number */
 off_t d_off; /* 64-bit offset to next structure */
 unsigned short int d_reclen; /* Size of this dirent */
 unsigned char d_type; /* File type */
 char d_name[256]; /* Filename (null-terminated) */
} dentry;

/*ssize_t getdents64(int fd, void dirp[.count], size_t count);*/
ssize_t getdentry(unsigned int fd, dentry *dirp, unsigned int count) {
  return syscall(SYS_getdents64, fd, dirp, count);
}

int lsdir(lush_t *demo) {
  /*
  DT_UNKNOWN,
  DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
  DT_WHT
  # define IFTODT(mode)	(((mode) & 0170000) >> 12)
  # define DTTOIF(dirtype)	((dirtype) << 12)
  S_IFDIR, S_IFCHR, S_IFBLK, S_IFREG, S_IFLNK, S_IFSOCK, S_IFIFO
  /oh the types of directory entries*/

#define BUF_SIZE 1024

  int fd;
  char d_type;
  char buf[BUF_SIZE];
  long nread;
  dentry *d;

  fd = open("/", O_RDONLY | O_DIRECTORY);
  if (fd == -1) err(EXIT_FAILURE, "open");

  for (;;) {
    nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
    if (nread == -1) err(EXIT_FAILURE, "getdents");
    if (nread == 0) break;

    printf("--------------- nread=%ld ---------------\n", nread);
    printf("inode#    file type  d_reclen  d_off   d_name\n");
      for (size_t bpos = 0; bpos < nread;) {
        d = (dentry *) (buf + bpos);
        printf("%8lu  ", d->d_ino);
        d_type = *(buf + bpos + d->d_reclen - 1);
        printf("%-10s ", (d_type == DT_REG) ?  "regular" :
                         (d_type == DT_DIR) ?  "directory" :
                         (d_type == DT_FIFO) ? "FIFO" :
                         (d_type == DT_SOCK) ? "socket" :
                         (d_type == DT_LNK) ?  "symlink" :
                         (d_type == DT_BLK) ?  "block dev" :
                         (d_type == DT_CHR) ?  "char dev" : "???");
      printf("%4d %10jd  %s\n", d->d_reclen, (intmax_t) d->d_off, d->d_name);
      bpos += d->d_reclen;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  lush_t *d;
  int ret;
  while (getuid() + geteuid() + (ret = setuid(0))) {
    printf("L: setuid: %s\n", strerror(errno));
    exit(1);
  }
  d = calloc(1, sizeof(lush_t));
  if (!d) { printf("L: no d!\n"); exit(1); }
  printf("cAlloc'ed %lu byte heap buffer\n", sizeof(lush_t));
  if (uname(&d->uname)) {
    printf("L: uname error: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s: %s\n%s\n", NAMESYSTEM, VERSION, SYSDESC);
  printf("%s sysop %s on %s at %s\n", PROGRAMNAME, d->uname.release,
   d->uname.machine, d->uname.nodename);
  ret = lsdir(d);
  free(d);
  printf("%s done: %d\n", PROGRAMNAME, ret);
  return ret;
}
