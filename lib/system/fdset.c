#include <krad/app/debug.h>
#include "fdset.h"

struct kr_fdset {
  int fd[KR_NFDS];
  int len;
};

size_t kr_fdset_sizeof() {
  return sizeof(kr_fdset);
}

int kr_fdset_init(kr_fdset *fdset) {
  int i;
  if (!fdset) return -1;
  for (i = 0; i < KR_NFDS; i++) {
    fdset->fd[i] = -1;
  }
  fdset->len = 0;
  return 0;
}

int kr_fdset_count(kr_fdset *fdset) {
  if (!fdset) return -1;
  return fdset->len;
}

int kr_fdset_put(kr_fdset *fdset, int *fds, int n) {
  int i;
  int pos;
  if (!fdset) return -1;
  if (n + fdset->len > KR_NFDS) return -1;
  for (i = 0; i < n; i++) {
    pos = i + fdset->len;
    fdset->fd[pos] = fds[i];    
  }
  fdset->len += n;
  return 0;
}

int kr_fdset_get(kr_fdset *fdset, int *fds, int n) {
  int i;
  if (!fdset) return -1;
  printk("fdset get is %d len is %d", n, fdset->len);
  if (n > fdset->len) return -2;
  for (i = 0; i < n; i++) {
    fds[i] = fdset->fd[i];
    fdset->fd[i] = -1;
  }
  fdset->len -= n;
  for (i = 0; i < fdset->len; i++) {
    fdset->fd[i] = fdset->fd[i + n];
    fdset->fd[i + n] = -1;
  }
  return 0;
}

int kr_fdset_close(kr_fdset *fdset) {
  int i;
  int ret;
  if (!fdset) return -1;
  for (i = 0; i < KR_NFDS; i++) {
    if (fdset->fd[i] != -1) {
      ret = close(fdset->fd[i]);
      if (ret) printke("fd set close fd");
      fdset->fd[i] = -1;
    }
  }
  fdset->len = 0;
  return 0; 
}
