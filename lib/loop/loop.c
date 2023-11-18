#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sched.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <pthread.h>
#include <krad/app/debug.h>
#include "loop.h"

enum kr_loop_state {
  KR_LOOP_START = 0,
  KR_LOOP_LOOPING,
  KR_LOOP_DONE
};

#define KR_LOOP_NEVENTS 64
#define KR_LOOP_NHARNESSES 65536

struct kr_loop {
  pthread_t thread;
  kr_priority priority;
  kr_loop_type type;
  int ed;
  int pd;
  int state;
  char name[16];
  kr_event watch[KR_LOOP_NHARNESSES];
  kr_loop *master;
};

static int loop_destruct(kr_event *event) {
  kr_loop *loop;
  loop = (kr_loop *)event->user;
  loop->state = KR_LOOP_DONE;
  return 0;
}

static void loop_wait(kr_loop *loop, int ms) {
  int n;
  int i;
  int ret;
  int err;
  kr_event *event;
  struct epoll_event events[KR_LOOP_NEVENTS];
  n = epoll_wait(loop->pd, events, KR_LOOP_NEVENTS, ms);
  if (((ms == -1) && (n < 1)) || (n < 0)) {
    err = errno;
    printke("Loop: error on epoll wait %s", strerror(err));
    /*
    if (err == EINTR) continue;
    break;
    */
    return;
  }
  for (i = 0; i < n; i++) {
    event = (kr_event *)events[i].data.ptr;
    event->events = events[i].events;
    ret = event->handler(event);
    if (ret == -1) {
      loop->state = KR_LOOP_DONE;
      break;
    }
  }
}

static int subloop_event(kr_event *event) {
  kr_loop *loop;
  loop = (kr_loop *)event->user;
  loop_wait(loop, 0);
  return 0;
}

static void loop_cycle(kr_loop *loop) {
  while (loop->state == KR_LOOP_LOOPING) {
    loop_wait(loop, -1);
  }
}

static void *loop_thread(void *arg) {
  int ret;
  int err;
  kr_loop *loop;
  loop = (kr_loop *)arg;
  loop->state = KR_LOOP_LOOPING;
  ret = prctl(PR_SET_NAME, loop->name, NULL, NULL, NULL);
  if (ret == -1) {
    err = errno;
    printke("Loop: prctl %s", strerror(err));
  }
  if (loop->priority > KR_PRIORITY_MIN && loop->priority <= KR_PRIORITY_MAX) {
    ret = kr_priority_set(loop->priority);
    if (ret) {
      printke("Loop: setting priority");
    }
  }
  printk("Loop: %s starting", loop->name);
  loop_cycle(loop);
  printk("Loop: %s exiting", loop->name);
  loop->state = KR_LOOP_DONE;
  return NULL;
}

int kr_loop_del(kr_loop *loop, int fd) {
  int ret;
  int err;
  kr_event *watch;
  if (loop == NULL) return -1;
  if (fd < 0 || fd >= KR_LOOP_NHARNESSES) return -2;
  watch = &loop->watch[fd];
  if (watch == NULL) return -3;
  ret = epoll_ctl(loop->pd, EPOLL_CTL_DEL, fd, NULL);
  watch->fd = 0;
  if (ret != 0) {
    err = errno;
    printke("Loop: epoll ctl del %s", strerror(err));
    return -5;
  }
  return 0;
}

int kr_loop_close(kr_loop *loop, int fd) {
  int ret;
  ret = kr_loop_del(loop, fd);
  if (ret != 0) return ret;
  return close(fd);
}

int kr_loop_mod(kr_loop *loop, kr_event *event) {
  int ret;
  int err;
  kr_event *watch;
  struct epoll_event ev;
  if (loop == NULL) return -1;
  if (event == NULL) return -2;
  if (event->fd < 0 || event->fd >= KR_LOOP_NHARNESSES) return -3;
  watch = &loop->watch[event->fd];
  if (watch == NULL) return -4;
  *watch = *event;
  memset(&ev, 0, sizeof(ev));
  ev.events = watch->events;
  ev.data.ptr = watch;
  ret = epoll_ctl(loop->pd, EPOLL_CTL_MOD, watch->fd, &ev);
  if (ret != 0) {
    err = errno;
    printke("Loop: epoll ctl mod %s", strerror(err));
    return -5;
  }
  return 0;
}

int kr_loop_add(kr_loop *loop, kr_event *event) {
  int ret;
  struct epoll_event ev;
  if (loop == NULL) return -1;
  if (event == NULL) return -2;
  if (event->fd < 0 || event->fd >= KR_LOOP_NHARNESSES) return -3;
  if (event->handler == NULL) return -4;
  if (loop->watch[event->fd].fd == 0) {
    loop->watch[event->fd] = *event;
    memset(&ev, 0, sizeof(ev));
    ev.events = event->events;
    ev.data.ptr = &loop->watch[event->fd];
    ret = epoll_ctl(loop->pd, EPOLL_CTL_ADD, event->fd, &ev);
    if (ret != 0) {
      printke("Loop: epoll ctl add event fd to loop pd fail");
      return -5;
    }
    return 0;
  }
  return -6;
}

int kr_loop_add_timeout(kr_loop *loop, int ms, kr_handler *handler,
 void *user) {
  int tfd;
  int ret;
  kr_event event;
  struct itimerspec new_value;
  time_t sec;
  long nsec;
  if (loop == NULL) return -1;
  if (handler == NULL) return -2;
  if (ms <= 0) return -3;
  tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (tfd < 0) {
    printke("Loop: timerfd_create fail");
    return -5;
  }
  memset(&new_value, 0, sizeof(new_value));
  sec = ms / 1000;
  nsec = (ms % 1000) * 1000000;
  new_value.it_value.tv_sec = sec;
  new_value.it_value.tv_nsec = nsec;
  new_value.it_interval.tv_sec = sec;
  new_value.it_interval.tv_nsec = nsec;
  ret = timerfd_settime(tfd, 0, &new_value, NULL);
  if (ret != 0) {
    printke("Loop: timerfd settime fail");
    close(tfd);
    return -5;
  }
  memset(&event, 0, sizeof(event));
  event.fd = tfd;
  event.handler = handler;
  event.user = user;
  event.events = EPOLLIN;
  ret = kr_loop_add(loop, &event);
  if (ret != 0) {
    close(tfd);
    return ret;
  }
  return 0;
}

int kr_loop_destroy(kr_loop *loop) {
  if (loop == NULL) return -1;
  printk("Loop: destroy");
  uint64_t u;
  int s;
  u = 666;
  if (loop->type == KR_SUPERLOOP) {
    s = write(loop->ed, &u, sizeof(uint64_t));
    if (s != sizeof(uint64_t)) {
      printk("Loop: error writing to loop ed");
    }
    pthread_join(loop->thread, NULL);
  }
  if (loop->type == KR_SUBLOOP) {
    kr_loop_del(loop->master, loop->pd);
  }
  close(loop->ed);
  close(loop->pd);
  free(loop);
  printk("Loop: destroyed");
  return 0;
}

kr_loop *kr_loop_create(kr_loop_setup *setup) {
  int ret;
  kr_loop *loop;
  kr_event harness;
  if (setup == NULL) return NULL;
  loop = calloc(1, sizeof(*loop));
  memcpy(loop->name, setup->name, sizeof(loop->name));
  loop->priority = setup->priority;
  if (!setup->master) {
    loop->type = KR_SUPERLOOP;
  } else {
    loop->type = KR_SUBLOOP;
  }
  loop->ed = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (loop->ed == -1) {
    printke("Loop: efd evenfd created failed");
    free(loop);
    return NULL;
  }
  loop->pd = epoll_create1(EPOLL_CLOEXEC);
  if (loop->pd == -1) {
    printke("Loop: pd epoll created failed");
    close(loop->ed);
    free(loop);
    return NULL;
  }
  harness.fd = loop->ed;
  harness.user = loop;
  harness.events = EPOLLIN;
  harness.handler = loop_destruct;
  ret = kr_loop_add(loop, &harness);
  if ((ret == 0) && (loop->type == KR_SUPERLOOP)) {
    pthread_create(&loop->thread, NULL, loop_thread, (void *)loop);
  }
  if ((ret != 0) || ((!loop->thread) && (loop->type == KR_SUPERLOOP))) {
    printf("Thread launch Error: %s", strerror(errno));
    close(loop->ed);
    close(loop->pd);
    free(loop);
    return NULL;
  }
  if (loop->type == KR_SUBLOOP) {
    loop->master = setup->master;
    harness.fd = loop->pd;
    harness.user = loop;
    harness.events = EPOLLIN;
    harness.handler = subloop_event;
    ret = kr_loop_add(setup->master, &harness);
    if (ret != 0) {
      printke("Subloop: could not add to my master %d", ret);
    }
  }
  return loop;
}

int kr_loop_setup_init(kr_loop_setup *setup) {
  if (!setup) return -1;
  memset(setup->name, 0, sizeof(setup->name));
  setup->master = NULL;
  setup->priority = KR_PRIORITY_NORMAL;
  return 0;
}

static int mainloop_signal_setup() {
  int sfd;
  sigset_t mask;
  sigemptyset(&mask);
  sigfillset(&mask);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
    fprintf(stderr, "Mainloop: Could not set signal mask!");
    exit(1);
  }
  sfd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
  if (sfd == -1) {
    fprintf(stderr, "Mainloop: could not setup signalfd");
    exit(1);
  }
  return sfd;
}

void kr_mainloop(kr_mainloop_setup *setup) {
  int ret;
  int sfd;
  int fail;
  kr_loop *loop;
  kr_event signal_event;
  if (setup == NULL) return;
  fail = 0;
  sfd = -1;
  ret = 1;
  loop = calloc(1, sizeof(*loop));
  loop->pd = epoll_create1(EPOLL_CLOEXEC);
  if (loop->pd == -1) {
    printke("Loop: pd epoll created failed");
    fail = 1;
  }
  if (!fail) {
    sfd = mainloop_signal_setup();
    ret = setup->startup_handler(loop, setup->user);
    if (ret != 0) fail = 1;
  }
  if (!fail) {
    signal_event.fd = sfd;
    signal_event.events = EPOLLIN;
    signal_event.user = setup->user;
    signal_event.handler = setup->signal_handler;
    kr_loop_add(loop, &signal_event);
    loop->state = KR_LOOP_LOOPING;
    loop_cycle(loop);
    ret = setup->shutdown_handler(setup->user);
  }
  close(loop->pd);
  close(sfd);
  free(loop);
  exit(ret);
}
