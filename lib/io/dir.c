#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
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
  mode = S_IRWXU | S_IRWXG | S_IROTH;
  return mkdir(dirname, mode);
}

char *kr_dir_current_dir_name(kr_dir *dir) {
  if (!dir || !dir->dh) return "WHAT THE FUCK";
  return dir->name;
}

//entry->sz = entry->entry_stat.st_size;
//entry->lastmod = entry->entry_stat.st_mtim;

int kr_dir_entry_is_dir(kr_dir_entry *e) {
  if (!e) return 0;
  if (S_ISDIR(e->entry_mode)) return 1;
  return 0;
}

int kr_dir_entry_is_file(kr_dir_entry *e) {
  if (!e) return 0;
  if (S_ISREG(e->entry_mode)) return 1;
  return 0;
}

int kr_dir_get_entry(kr_dir *dir, kr_dir_entry *entry) {
  int ret;
  if (!dir || !dir->dh || !entry) return -1;
  if (dir->pos) {
    seekdir(dir->dh, dir->pos);
  }
  for (;;) {
    entry->entry = readdir(dir->dh);
    if (!entry->entry) {
      //printf("no entry: %s\n", strerror(errno));
      //exit(1);
      return 0;
    }
    dir->pos = telldir(dir->dh);
    if (dir->pos == -1) {
      printf("telldir: %s\n", strerror(errno));
      dir->pos = 0;
    }
    if ((entry->entry->d_name[0] == '.' && entry->entry->d_name[1] == '\0')
       || (entry->entry->d_name[0] == '.' && entry->entry->d_name[1] == '.'
       && entry->entry->d_name[2] == '\0')) continue;
    break;
  }
  entry->name = entry->entry->d_name;
  ret = fstatat(dirfd(dir->dh), entry->name, &entry->entry_stat, 0);
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
  if (!dir || !path || len < 1 || (len > (PATH_MAX -1))) {
    printf("wtf?\n");
    return -1;
  }
  err = stat(path, &dir->s);
  if (err == -1) {
    if (ENOENT == errno) {
      printf("Prolly trying to open a linkhole\n");
      return -1;
    } else {
      printf("another error\n");
      return -1;
    }
  }
  if (S_ISDIR(dir->s.st_mode)) {
    // it's a directory
    //return 1;
  } else {
    // exists but is no dir
    return 0;
  }
  if (S_ISLNK(dir->s.st_mode)) {
    printf("Oh noe: its a link, dont follow links u dont chek rly exist\n");
    return -1;
  }
  dir->dh = opendir(path);
  if (dir->dh) {
    memcpy(dir->name, path, len);
    dir->name[len] = '\0';
    dir->pos = telldir(dir->dh);
    return 0;
  }
  printf("What the fuck!\n");
  exit(1);
  return -1;
}
