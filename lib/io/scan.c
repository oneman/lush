#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <krad/mem/mem.h>
#include "scan.h"

#define KR_SCAN_MAXDEPTH 42

struct kr_scan {
  char path[PATH_MAX];
  size_t len;
  int depth; 
  kr_dir dir[KR_SCAN_MAXDEPTH];
};

size_t kr_scan_path(kr_scan *scan, char **path) {
  if (!scan || !path) return 0;
  *path = scan->path;
  return scan->len;
}

int kr_scan_probe(kr_scan *scan, char *subdir) {
  int ret;
  if (!scan) return -1;
  scan->len += snprintf(scan->path + scan->len, sizeof(scan->path) - scan->len, "/%s",
   subdir);
  ret = kr_dir_open(&scan->dir[++scan->depth], scan->path, scan->len);
  return ret;
}

int kr_scan_iter(kr_scan *scan, kr_dir_entry *entry) {
  int ret;
  ret = 0;
  if (!scan || !entry) return -1;
  for (;;) {
    ret = kr_dir_get_entry(&scan->dir[scan->depth], entry);
    if (ret == 1)  {
      kr_dir_iter(&scan->dir[scan->depth]);
      break;
    }
    if (scan->depth > -1) {
      ret = kr_dir_close(&scan->dir[scan->depth]);
      while (scan->path[scan->len] != '/' && scan->len > 0) scan->len--;
      scan->path[scan->len] = '\0';
      if (scan->depth > 0) {
        scan->depth--;
        continue;
      }
      ret = 0;
      break;
    }
  }
  return ret;
}

int kr_scan_destroy(kr_scan *scan) {
  int i;
  int ret;
  ret = 0;
  for (i = 0; i < KR_SCAN_MAXDEPTH; i++) {
    if (kr_dir_is_open(&scan->dir[i])) {
      ret += kr_dir_close(&scan->dir[i]);
    } 
  }
  return ret;
}

kr_scan *kr_scan_create(kr_scan_setup *setup) {
  int ret;
  kr_scan *scan;
  if (!setup || !setup->mem) return NULL;
  scan = (kr_scan *)setup->mem;
  scan->len = setup->len;
  memcpy(scan->path, setup->path, sizeof(scan->path));
  scan->depth = 0;
  memset(scan->dir, 0, sizeof(scan->dir));
  ret = kr_dir_open(&scan->dir[0], scan->path, scan->len);
  if (ret) return NULL;
  return scan;
}

size_t kr_scan_sizeof() {
  return sizeof(kr_scan);
}
