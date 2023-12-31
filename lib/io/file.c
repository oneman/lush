#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/statfs.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "file.h"

struct kr_file {
  int32_t fd;
  off_t position;
  int32_t readable;
  int32_t writable;
  int32_t seekable;
  int32_t std_stream;
  int32_t local;
  int32_t remote;
  int32_t mapmode;
  off_t size;
  char *path;
  struct stat st;
  struct statfs stfs;
};

static kr_file kr_file_stdin;
static kr_file kr_file_stdout;

kr_file *kr_file_open_stdin() {
  errno = 0;
  fcntl(STDIN_FILENO, F_GETFD);
  if (errno == EBADF) {
    return NULL;
  }
  kr_file_stdin.fd = STDIN_FILENO;
  kr_file_stdin.path = "STDIN";
  kr_file_stdin.std_stream = 1;
  kr_file_stdin.seekable = 0;
  kr_file_stdin.readable = 1;
  kr_file_stdin.writable = 0;
  return &kr_file_stdin;
}

kr_file *kr_file_open_stdout() {
  errno = 0;
  fcntl(STDOUT_FILENO, F_GETFD);
  if (errno == EBADF) {
    return NULL;
  }
  kr_file_stdout.fd = STDOUT_FILENO;
  kr_file_stdout.path = "STDOUT";
  kr_file_stdout.std_stream = 1;
  kr_file_stdout.seekable = 0;
  kr_file_stdout.readable = 0;
  kr_file_stdout.writable = 1;
  return &kr_file_stdout;
}

int file_exists(char *path) {
  int err;
  struct stat s;
  err = stat(path, &s);
  if (err == -1) {
    if(ENOENT == errno) {
      // does not exist
      return 0;
    } else {
      // another error
      return 0;
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

int64_t file_size(char *path) {
  int err;
  int64_t filesize;
  struct stat s;
  err = stat (path, &s);
  if (err == -1) {
    if(ENOENT == errno) {
      // does not exist
      return -2;
    } else {
      // another error
      return -1;
    }
  } else {
    if ((S_ISREG(s.st_mode)) || (S_ISLNK(s.st_mode))) {
      // it's a file
      filesize = s.st_size;
      return filesize;
    } else {
      // exists but is not file
      return -3;
    }
  }
  return -1;
}

kr_file *kr_file_create(char *path) {
  kr_file file;
  kr_file *ofile;
  int err;
  int flags;
  int mode;
  flags = O_WRONLY | O_CREAT;
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  if (path == NULL) {
    return NULL;
  }
  if ((strlen(path) == 1) && (path[0] == '-')) {
    return kr_file_open_stdout();
  }
  memset(&file, 0, sizeof (kr_file));
  err = stat(path, &file.st);
  if (err == -1) {
    if (errno == ENOENT) {
      // does not exist
    } else {
      // another error
      return NULL;
    }
  } else {
    // file already exists
    return NULL;
  }
  //FIXME check that dir under filename exist
  if (statfs (path, &file.stfs) != -1) {
    if (file.stfs.f_type == 0xFF534D42 || /* cifs */
        file.stfs.f_type == 0x6969 || /* nfs */
        file.stfs.f_type == 0x517B) { /* smb */
      //FIXME so whatcha going to bout it?
      file.remote = 1;
    } else {
      file.local = 1;
    }
  }
  file.fd = open(path, flags, mode);
  if (file.fd < 0) {
    return NULL;
  }
  file.position = 0;
  file.readable = 0;
  file.writable = 1;
  file.position = 0;
  file.std_stream = 0;
  //file.seekable;
  //file.mapmode;
  ofile = kr_alloc(sizeof(kr_file));
  memcpy(ofile, &file, sizeof(kr_file));
  ofile->path = strdup(path);
  return ofile;
}

kr_file *kr_file_open(char *path) {
  kr_file file;
  kr_file *ofile;
  int err;
  if (path == NULL) {
    return NULL;
  }
  if ((strlen(path) == 1) && (path[0] == '-')) {
    return kr_file_open_stdin ();
  }
  memset(&file, 0, sizeof (kr_file));
  err = stat(path, &file.st);
  if (err == -1) {
    if (errno == ENOENT) {
      // does not exist
      return NULL;
    } else {
      // another error
      return NULL;
    }
  } else {
    //FIXME handle symlink
    if (!(S_ISREG(file.st.st_mode))) {
      return NULL;
    }
  }
  file.size = file.st.st_size;
  if (statfs(path, &file.stfs) != -1) {
    if (file.stfs.f_type == 0xFF534D42 || /* cifs */
        file.stfs.f_type == 0x6969 || /* nfs */
        file.stfs.f_type == 0x517B) { /* smb */
      //FIXME so whatcha going to bout it?
      file.remote = 1;
    } else {
      file.local = 1;
    }
  }
  file.fd = open(path, O_RDONLY);
  if (file.fd < 0) {
    return NULL;
  }
  file.position = 0;
  file.readable = 1;
  file.writable = 0;
  file.std_stream = 0;
  //file.seekable;
  //file.mapmode;
  ofile = kr_alloc(sizeof(kr_file));
  memcpy (ofile, &file, sizeof(kr_file));
  ofile->path = strdup(path);
  return ofile;
}

char *kr_file_path(kr_file *file) {
  if (file == NULL) return NULL;
  return file->path;
}

int64_t kr_file_size(kr_file *file) {
  if (file == NULL) return -1;
  return file->size;
}

int kr_file_fd(kr_file *file) {
  if (file == NULL) return -1;
  return file->fd;
}

ssize_t kr_file_read(kr_file *file, void *buffer, size_t len) {
  return read(file->fd, buffer, len);
}

ssize_t kr_file_write(kr_file *file, void *buffer, size_t len) {
  return write(file->fd, buffer, len);
}

int kr_file_close(kr_file **file) {
  if ((file == NULL) || (*file == NULL)) {
    return -1;
  }
  // We would never close std streams here
  if ((*file == &kr_file_stdin) || (*file == &kr_file_stdout)) {
    return 0;
  }
  close((*file)->fd);
  free((*file)->path);
  free(*file);
  *file = NULL;
  return 0;
}
