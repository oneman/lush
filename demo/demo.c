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
#include <krad/io/file2.h>

#include "cut/deep.c"

int find_files() {
  int ret;
  int opt;
  FTS *fs;
  FTSENT *n;
  opt = FTS_PHYSICAL;
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

void superuser() {
  if (setreuid(0, 0) + setregid(0, 0)) {
    fprintf(stderr, "FAIL: need be superuser\n");
    exit(1);
  }
}

int process(int argc, char *argv[]) {
  char *path;
  size_t sz;
  uint8_t *data;
  kr_file2 *file;
  kr_file2_info info;
  kr_fs_setup setup;
  kr_file_set *fs;
  path = argv[1];
  sz = strlen(path);
  if (!kr_file2_exists(path, sz)) {
    printf("Doesn't exist %s\n", path);
    return 1;
  }
  setup.nfiles = 1;
  setup.user = NULL;
  setup.event_cb = NULL;
  fs = kr_file_set_create(&setup);
  if (!fs) {
    printf("this fs didn't create\n");
    return 1;
  }
  file = kr_file2_open(fs, path, sz);
  if (!file) {
    printf("Can't open %s\n", path);
    return kr_file_set_destroy(fs);
  }
  data = kr_file2_get_data(file);
  if (data && !kr_file2_get_info(file, &info)) {
    printf("teardown %s\n", path);
    deepcut(data, info.sz);
  }
  return kr_file_set_destroy(fs);
}

int main(int argc, char *argv[]) {
  superuser();
  if (argc == 1) return find_files();
  return process(argc, argv);
}
