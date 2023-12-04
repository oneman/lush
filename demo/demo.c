#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
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
} entry;

/*ssize_t getdents64(int fd, void dirp[.count], size_t count);*/
ssize_t get_dir_ls(unsigned int fd, entry *dirp, unsigned int count) {
  return syscall(SYS_getdents64, fd, dirp, count);
}
int playlist(lush_t *d) {
/*
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };
Convert between stat structure types and directory types.
# define IFTODT(mode)	(((mode) & 0170000) >> 12)
# define DTTOIF(dirtype)	((dirtype) << 12)
S_IFDIR
    This is the file type constant of a directory file.
S_IFCHR
    This is the file type constant of a character-oriented device file.
S_IFBLK
    This is the file type constant of a block-oriented device file.
S_IFREG
    This is the file type constant of a regular file.
S_IFLNK
    This is the file type constant of a symbolic link.
S_IFSOCK
    This is the file type constant of a socket.
S_IFIFO
*/
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
  ret = playlist(d);
  free(d);
  printf("%s done: %d\n", PROGRAMNAME, ret);
  return ret;
}
