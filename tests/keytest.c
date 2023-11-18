#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <linux/input.h>

/* gcc -Wall keytest.c -o keytest */

#define NEVENTS 1

int main(int argc, char *argv[]) {
  int pd;
  int kd;
  int ret;
  int err;
  char *keyboard;
  struct input_event inev;
  struct epoll_event ev;
  struct epoll_event events[NEVENTS];
  memset(&ev, 0, sizeof(ev));
  keyboard = "/dev/input/by-id/usb-CM_Storm_QuickFire_Rapid_keyboard-event-kbd";
  pd = epoll_create1(EPOLL_CLOEXEC);
  kd = open(keyboard, O_RDONLY);
  ev.events = EPOLLIN;
  ev.data.fd = kd;
  ret = epoll_ctl(pd, EPOLL_CTL_ADD, kd, &ev);
  if (ret == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
    return 1;
  }
  for (;;) {
    ret = epoll_wait(pd, events, NEVENTS, -1);
    if (ret == -1) return 1;
    read(kd, &inev, sizeof(inev));
    printf("ev.type %d ev.code %d\n", inev.type, inev.code);
  }
  close(kd);
  close(pd);
  return 0;
}
