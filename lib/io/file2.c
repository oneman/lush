#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/statfs.h>
#include <krad/mem/pool.h>
#include <krad/app/debug.h>
#include "file2.h"
#include "error.h"

#define KRPGSZ 4096

struct kr_file2 {
  kr_file_set *fs;
  uint8_t *data;
  int fd;
  int ref;
  size_t pages;
  struct stat st;
  struct statfs stfs;
  kr_file2_info info;
};

struct kr_file_set {
  kr_pool *pool;
  void *user;
  kr_fs_event_cb *event_cb;
};

static kr_file2 *fs_slot(kr_file_set *fs);
static kr_file2 *fs_find(kr_file_set *fs, char *path, size_t len);

static kr_file2 *fs_slot(kr_file_set *fs) {
  kr_file2 *file;
  file = kr_pool_slice(fs->pool);
  if (!file) return NULL;
  memset(file, 0, sizeof(*file));
  file->fs = fs;
  return file;
}

static kr_file2 *fs_find(kr_file_set *fs, char *path, size_t len) {
  int i;
  kr_file2 *cur;
  i = 0;
  while ((cur = kr_pool_iterate_active(fs->pool, &i))) {
    if (cur->info.len != len) continue;
    if (memcmp(cur->info.path, path, len) == 0) {
      return cur;
    }
  }
  return NULL;
}

static void fs_info(kr_file2 *file) {
  int ret;
  ret = fstatfs(file->fd, &file->stfs);
  if (ret != -1) {
    if (file->stfs.f_type == 0xFF534D42 /* cifs */
     || file->stfs.f_type == 0x6969 /* nfs */
     || file->stfs.f_type == 0x517B) { /* smb */
      file->info.remote_fs = 1;
    }
  }
}

static void run_event(kr_file2 *file, kr_file_event_type type) {
  kr_file_event event;
  event.user = file->fs->user;
  event.file = file;
  event.type = type;
  file->fs->event_cb(&event);
}

int kr_file2_exists(char *path, size_t len) {
  char fpath[4096];
  int err;
  struct stat s;
  if (len >= sizeof(fpath)) return -1;
  memset(fpath, 0, sizeof(fpath));
  memcpy(fpath, path, len);
  memset(&s, 0, sizeof(s));
  err = stat(fpath, &s);
  if (err == -1) {
    if(ENOENT == errno) {
      // does not exist
      return 0;
    } else {
      // another error
      return -1;
    }
  } else {
    if ((S_ISREG(s.st_mode)) || (S_ISLNK(s.st_mode))) {
      // it's a file
      return 1;
    } else {
      // exists but is no dir
      return 0;
    }
  }
  return 0;
}

uint8_t *kr_file2_get_data(kr_file2 *file) {
  if (!file) return NULL;
  run_event(file, KR_FILE_DATA);
  return file->data;
}

int kr_file2_get_info(kr_file2 *file, kr_file2_info *info) {
  if (!file || !info) return -1;
  *info = file->info;
  return 0;
}

int kr_file2_unlink(kr_file2 *file) {
  int ret;
  if (!file) return -1;
  if (file->info.access_mode == KR_FILE_ACCESS_ONLY) return -2;
  ret = unlink(file->info.path);
  if (ret != 0) return ret;
  return kr_file2_close(file);
}

int kr_file2_close(kr_file2 *file) {
  int ret;
  size_t sz;
  if (!file) return -1;
  file->ref--;
  if (file->ref) return 0;
  //printk("File: closing %s", file->info.path);
  if (file->fd > -1) {
    ret = close(file->fd);
    file->fd = -1;
  }
  sz = file->info.sz;
  ret = munmap(file->data, sz);
  ret = kr_pool_release(file->fs->pool, file);
  run_event(file, KR_FILE_CLOSE);
  return ret;
}

int kr_file2_truncate(kr_file2 *file, size_t sz) {
  if (!file || !sz) return -1;
  if (file->fd < 0) return -1;
  if (file->info.access_mode == KR_FILE_ACCESS_ONLY) return -2;
  return ftruncate(file->fd, sz);
}

int kr_file2_wrote(kr_file2 *file, size_t sz) {
  if (!file || !sz) return -1;
  if (file->info.access_mode == KR_FILE_ACCESS_ONLY) return -2;
  file->info.wrote += sz;
  if (file->info.wrote > file->info.sz) return -3;
  run_event(file, KR_FILE_WROTE);
  return 0;
}

ssize_t kr_file2_append(kr_file2 *file, uint8_t *data, size_t sz) {
  size_t len;
  uint8_t *buf;
  if (!file || !data || !sz) return -1;
  if (file->fd < 0) return -1;
  if (file->info.access_mode == KR_FILE_ACCESS_ONLY) return -2;
  buf = kr_file2_get_data(file);
  len = sz;
  if (file->info.wrote + len > file->info.sz) {
    len = file->info.sz - file->info.wrote;
  }
  memcpy(buf + file->info.wrote, data, len);
  kr_file2_wrote(file, len);
  return len;
}

ssize_t kr_file2_read(kr_file2 *file, uint8_t *data, size_t sz) {
  return read(file->fd, data, sz);
}

static int file_stat_to_text(kr_file2 *file) {
  int ret;
  ret = 0;
  if (S_ISSOCK(file->st.st_mode)) {
    memcpy(file->info.type, "socket", 7);
  }
  if (S_ISLNK(file->st.st_mode)) {
    memcpy(file->info.type, "symbolic link", 14);
  }
  if (S_ISREG(file->st.st_mode)) {
    memcpy(file->info.type, "regular file", 13);
  }
  if (S_ISBLK(file->st.st_mode)) {
    memcpy(file->info.type, "block device", 13);
  }
  if (S_ISDIR(file->st.st_mode)) {
    memcpy(file->info.type, "directory", 10);
  }
  if (S_ISCHR(file->st.st_mode)) {
    memcpy(file->info.type, "character device", 17);
  }
  if (S_ISFIFO(file->st.st_mode)) {
    memcpy(file->info.type, "FIFO", 5);
  }
  if (strnlen(file->info.type, 26) < 4) {
    printf("There is seven types of file, wtf %d even is.\n", file->st.st_mode);
    ret = 1;
    return ret;
  }
  printf("\nThis type of file is %s\n", file->info.type);
  file->info.lastmod = file->st.st_mtim.tv_sec;
  file->info.sz = file->st.st_size;
  printf("path: %s\n", file->info.path);
  printf("lastmod: %ld\n", file->info.lastmod);
  printf("sz: %lu\n", file->info.sz);
  printf("type: %s\n", file->info.type);
  /*if (!(file->st.st_mode && S_IFREG)) {
    printf("Can we even MMAP %s files?\n", file->info.type);
  }*/
  return ret;
}

kr_file2 *kr_file2_open(kr_file_set *fs, char *path, size_t len) {
  int ret;
  int err;
  kr_file2 *file;
  static const int flags = O_RDONLY;
  static const int mflags = MAP_PRIVATE | MAP_NONBLOCK;
  static const int prot = PROT_READ;
  if (!fs || !path) return NULL;
  if (len > (sizeof(file->info.path) - 1)) return NULL;
  file = fs_find(fs, path, len);
  if (file) {
    file->ref++;
    return file;
  } else {
    file = fs_slot(fs);
  }
  if (!file) return NULL;
  file->ref = 1; /* FIXME .. this look suspicious */
  file->info.access_mode = KR_FILE_ACCESS_ONLY;
  file->info.len = len;
  file->info.path[len] = '\0';
  memcpy(file->info.path, path, len);
  ret = stat(file->info.path, &file->st);
  if (ret == 0) {
    file_stat_to_text(file);
  }
  if (ret == -1) {
    err = errno;
    if (err == ENOENT) {
      printke("File: open - %s does not exist: %s", strerror(err),
       file->info.path);
      kr_pool_release(fs->pool, file);
      return NULL;
    } else {
      printke("File: open - err: %s - %s", strerror(err), file->info.path);
      kr_pool_release(fs->pool, file);
      return NULL;
    }
  } else {
    //FIXME handle symlink
    /* NOFIXME: we can open any kind of file its file but lets know the mode
    if (!(S_ISREG(file->st.st_mode))) {
      printke("File: open - %s not a normal file", file->info.path);
      kr_pool_release(fs->pool, file);
      return NULL;
    }*/
  }
  if (file->info.sz == 0) {
    //printke("File: %s is zero bytes ??", file->info.path);
    //kr_pool_release(fs->pool, file);
    //return NULL;
  }
  file->fd = open(file->info.path, flags);
  if (file->fd < 0) {
    err = errno;
    printke("File: error opening file %s - %s", strerror(err),
     file->info.path);
    kr_pool_release(fs->pool, file);
    return NULL;
  }
  /*printk("pages: %d bytes: %zu", file->pages, file->st.st_size);*/
  file->info.remote_fs = 0;
  fs_info(file);
  if ((file->info.sz > 0) && ((file->st.st_mode & S_IFREG) == S_IFREG)) {
    printf("We have a regular type file with size, we attempt to mmap\n");
    file->pages = file->info.sz / KRPGSZ;
    if (!file->pages || file->pages % KRPGSZ) file->pages++;
    file->data = mmap(NULL, file->pages * KRPGSZ, prot, mflags, file->fd, 0);
    if (file->data == MAP_FAILED) {
      err = errno;
      printke("File: mmap err %s", strerror(err));
      close(file->fd);
      kr_pool_release(fs->pool, file);
      return NULL;
    }
    printf("MMAP'ed %s\n", file->info.path);
  }
  run_event(file, KR_FILE_OPEN);
  return file;
}

kr_file2 *kr_file2_create(kr_file_set *fs, char *path, size_t len, size_t sz) {
  static const int flags = O_CREAT | O_EXCL | O_RDWR;
  static const int mflags = MAP_SHARED | MAP_NONBLOCK;
  static const int prot = PROT_READ | PROT_WRITE;
  kr_file2 *file;
  if (!fs || !path || !sz) return NULL;
  file = fs_find(fs, path, len);
  if (file) return NULL;
  file = fs_slot(fs);
  if (!file) return NULL;
  file->ref = 1;
  file->info.access_mode = KR_FILE_ACCESS_APPEND;
  file->info.len = len;
  file->info.path[len] = '\0';
  memcpy(file->info.path, path, len);
  file->info.sz = sz;
  if (file->info.sz == 0) {
    printke("File: create - %s is zero bytes, screw it", file->info.path);
    kr_pool_release(fs->pool, file);
    return NULL;
  }
  file->fd = open(file->info.path, flags, 0640);
  if (file->fd < 0) {
    printke("File: error opening file %s - %s for creation", strerror(errno),
     file->info.path);
    kr_pool_release(fs->pool, file);
    return NULL;
  }
  if (fallocate(file->fd, 0, 0, file->info.sz) < 0) {
    printke("fallocate failed: %m\n");
    printk("size was %"PRIu64, file->info.sz);
    unlink(path);
    close(file->fd);
    kr_pool_release(fs->pool, file);
    return NULL;
  }
  file->info.remote_fs = 0;
  fs_info(file);
  file->pages = file->info.sz / KRPGSZ;
  if (!file->pages || file->pages % KRPGSZ) file->pages++;
  file->data = mmap(NULL, file->pages * KRPGSZ, prot, mflags, file->fd, 0);
  if (file->data == MAP_FAILED) {
    printke("File: mmap %s", strerror(errno));
    unlink(path);
    close(file->fd);
    kr_pool_release(fs->pool, file);
    return NULL;
  }
  run_event(file, KR_FILE_CREATE);
  return file;
}

int kr_file_set_destroy(kr_file_set *fs) {
  int i;
  kr_file2 *file;
  if (!fs) return -1;
  //printk("File set: Destroying");
  i = 0;
  while ((file = kr_pool_iterate_active(fs->pool, &i))) {
    kr_file2_close(file);
  }
  kr_pool_destroy(fs->pool);
  //printk("File set: Destroyed");
  return 0;
}

kr_file_set *kr_file_set_create(kr_fs_setup *setup) {
  kr_file_set *fs;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (!setup) return NULL;
  //printk("File set: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*fs);
  pool_setup.size = sizeof(kr_file2);
  pool_setup.slices = setup->nfiles;
  pool = kr_pool_create(&pool_setup);
  if (!pool) nullfail(KR_ERR_IO_FILE_RAWR);
  fs = kr_pool_overlay_get(pool);
  memset(fs, 0, sizeof(*fs));
  fs->pool = pool;
  fs->user = setup->user;
  fs->event_cb = setup->event_cb;
  //printk("File set: Created");
  return fs;
}
