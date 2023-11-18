#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/eventfd.h>
#include <krad/app/debug.h>
#include "notice.h"

uint64_t kr_take_notice(int fd) {
  int ret;
  uint64_t n;
  n = 0;
  ret = read(fd, &n, sizeof(uint64_t));
  if (ret != sizeof(n)) {
    printke("Take notice: read ret %d %s", ret, strerror(errno));
    return -1;
  }
  return n;
}

int kr_give_notice(int fd) {
  int ret;
  uint64_t n;
  n = 1;
  ret = write(fd, &n, sizeof(uint64_t));
  if (ret != sizeof(n)) {
    printke("Give notice: write ret %d %s", ret, strerror(errno));
    return -1;
  }
  return 0;
}

int kr_notifier_destroy(int fd) {
  return close(fd);
}

int kr_notifier_create() {
  int fd;
  fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  return fd;
}
