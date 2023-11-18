#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <krad/app/debug.h>
#include <krad/time/timer.h>
#include <krad/mem/mem.h>
#include <krad/loop/loop.h>
#include <krad/io/file.h>

static int winning(kr_event *event) {
  int ret;
  uint64_t num;
  int efd;
  efd = *(int *)event->user;
  num = 666;
  ret = write(efd, &num, sizeof(num));
  if (ret < 0) return -1;
  printk("Roar!");
  return 0;
}

static int loop_test() {
  uint64_t num;
  int efd;
  int ret;
  kr_loop *loop;
  kr_event event;
  kr_loop_setup loop_setup;
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "Someloop");
  loop = kr_loop_create(&loop_setup);
  if (loop) {
    efd = eventfd(0, EFD_CLOEXEC);
    event.fd = 0; /* stdin */
    event.user = &efd;
    event.events = EPOLLIN | EPOLLET;
    event.handler = winning;
    kr_loop_add(loop, &event);
    printk("Press enter to win.");
    ret = read(efd, &num, sizeof(num));
    if (ret < 0) return -2;
    kr_loop_destroy(loop);
    printk("Winner! Number %"PRIu64"", num);
    return 0;
  }
  return -1;
}

/***** Mainloop Test *******/

typedef struct {
  char name[32];
  kr_loop *mainloop;
} my_struct;

int my_signal(kr_event *event) {
  ssize_t s;
  my_struct *me;
  struct signalfd_siginfo fdsi;
  me = (my_struct *)event->user;
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", me->name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", me->name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", me->name);
        printk("%s: Shutting down", me->name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", me->name);
        printk("%s: Shutting down", me->name);
        return -1;
      default:
        printk("%s: Got Signal %u", me->name, fdsi.ssi_signo);
    }
  }
  return 0;
}

int my_startup(kr_loop *loop, void *user) {
  my_struct *me;
  me = (my_struct *)user;
  printk("%s startup", me->name);
  me->mainloop = loop;
  return 0;
}

int my_shutdown(void *user) {
  my_struct *me;
  me = (my_struct *)user;
  printk("%s shutdown", me->name);
  return 0;
}

static int mainloop_test() {
  my_struct me;
  kr_mainloop_setup setup;
  snprintf(me.name, sizeof(me.name), "Meee");
  setup.user = &me;
  setup.signal_handler = my_signal;
  setup.startup_handler = my_startup;
  setup.shutdown_handler = my_shutdown;
  kr_mainloop(&setup);
  return 0;
}

int main (int argc, char *argv[]) {
  int ret;
  if ((argc == 2) && (atoi(argv[1]) == 666)) {
    ret = mainloop_test();
  } else {
    ret = loop_test();
  }
  return ret;
}
