#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "dir.h"

#define KR_DIR_PATH_SZ 4096

int kr_dir_exists(char *dir) {
  int err;
  struct stat s;
  err = stat(dir, &s);
  if (err == -1) {
    if (ENOENT == errno) {
      // does not exist
      return 0;
    } else {
      // another error
      return 0;
    }
  } else {
    if (S_ISDIR(s.st_mode)) {
      // it's a directory
      return 1;
    } else {
      // exists but is no dir
      return 0;
    }
  }
  return 0;
}

int kr_dir_create(char *dirname) {
  mode_t mode;
  if (dirname == NULL) return -1;
  if (kr_dir_exists(dirname)) return -2;
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  return mkdir(dirname, mode);
}

int kr_dir_get_entry(kr_dir *dir, kr_dir_entry *entry) {
  int ret;
  struct dirent *res;
  if (!dir || !dir->dh || !entry) return -1;
  if (dir->pos) {
    seekdir(dir->dh, dir->pos);
  }
  for (;;) {
    dir->pos = telldir(dir->dh);
    if (dir->pos == -1) dir->pos = 0;
    res = readdir(dir->dh);
    if (!res) {
      printf("ahh fail: %s\n", strerror(errno));
      exit(1);
    }
    entry->entry = *res;
    /*if (ret) return -1;
    if (ret) break;
    if (res == NULL) return 0;
    if (res->d_ino) {
      if ((res->d_name[0] == '.' && res->d_name[1] == '\0')
       || (res->d_name[0] == '.' && res->d_name[1] == '.'
       && res->d_name[2] == '\0')) continue;
       break;
    }*/
  }
  entry->name = entry->entry.d_name;
  ret = fstatat(dirfd(dir->dh), entry->entry.d_name, &entry->entry_stat, 0);
  if (ret == 0) {
    entry->entry_mode = entry->entry_stat.st_mode;
    entry->sz = entry->entry_stat.st_size;
    entry->lastmod = entry->entry_stat.st_mtim;
    return 1;
  }
  return ret;
}

int kr_dir_iter(kr_dir *dir) {
  if (!dir || !dir->dh) return -1;
  if (dir->pos) {
    dir->pos = 0;
    return 0;
  }
  printf("Dir wasnt paused?\n");
  return -1;
}

int kr_dir_close(kr_dir *dir) {
  int ret;
  ret = -1;
  if (dir && dir->dh) {
    ret = closedir(dir->dh);
    dir->pos = 0;
    dir->dh = NULL;
  }
  return ret;
}

int kr_dir_is_open(kr_dir *dir) {
  if (dir && dir->dh) return 1;
  return 0;
}

int kr_dir_open(kr_dir *dir, char *path, size_t len) {
  int err;
  if (!dir || !path || len < 1) {
    printf("wtf?\n");
    return -1;
  }
  err = stat(path, &dir->s);
  if (err == -1) {
    if (ENOENT == errno) {
      // does not exist
      return 0;
    } else {
      // another error
      return 0;
    }
  }
  if (S_ISDIR(dir->s.st_mode)) {
    // it's a directory
    //return 1;
  } else {
    // exists but is no dir
    return 0;
  }
  dir->dh = opendir(path);
  if (dir->dh) {
    dir->pos = telldir(dir->dh);
    return 0;
  }
  printf("What the fuck!\n");
  exit(1);
  return -1;
}
