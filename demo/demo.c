#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include "doc/1a2b3c/stdiov.h"

int ls_fs() {
  int ret;
  int opt;
  FTS *fs;
  FTSENT *n;
  opt = FTS_PHYSICAL | FTS_XDEV;
  char * const paths[] = {"/", NULL}; 
  fs = fts_open(paths, opt, NULL);
  if (!fs) exit(1);
  for (;;) {
    n = fts_read(fs);
    if (!n) break;
    if (n->fts_info == FTS_F) {
      printf("%s/%s\n", n->fts_path, n->fts_name);
    }
  }
  ret = fts_close(fs);
  return ret;
}

int main(int argc, char *argv[]) {
  int ret;
  /* Must be a superuser */
  while (setreuid(0, 0) + setregid(0, 0)) {
    printf("L: setreg/uid: %s\n", strerror(errno));
    exit(1);
  }
  ret = ls_fs();
  return ret;
}
