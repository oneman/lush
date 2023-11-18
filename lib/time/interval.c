#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/time/interval.h>

struct kr_interval {
  int fd;
  struct timespec start;
  uint64_t num;
  uint64_t den;
  uint64_t tick_ns;
  uint64_t ticks;
};

static inline uint64_t ts_to_nsec(struct timespec ts);
static inline struct timespec nsec_to_ts(uint64_t nsecs);
static struct timespec add_ns(struct timespec ts, uint64_t ns);
/*static struct timespec add_ms(struct timespec ts, uint64_t ms);*/

static inline uint64_t ts_to_nsec(struct timespec ts) {
  return (ts.tv_sec * 1000000000LL) + ts.tv_nsec;
}

static inline struct timespec nsec_to_ts(uint64_t nsecs) {
  struct timespec ts;
  ts.tv_sec = nsecs / (1000000000LL);
  ts.tv_nsec = nsecs % (1000000000LL);
  return ts;
}

static struct timespec add_ns(struct timespec ts, uint64_t ns) {
  uint64_t nsecs = ts_to_nsec(ts);
  nsecs += ns;
  return nsec_to_ts(nsecs);
}

/*
static struct timespec add_ms(struct timespec ts, uint64_t ms) {
  return add_ns(ts, ms * 1000000);
}
*/

int kr_interval_update(kr_interval *t) {
  ssize_t s;
  uint64_t exp;
  if (!t) return -1;
  //if (t->ticks) {
    s = read(t->fd, &exp, sizeof(uint64_t));
    if (s != sizeof(uint64_t)) {
      printke("Interval Timerfd read fail");
      return -2;
    }
  //}
  t->ticks++;
  return 0;
}

int kr_interval_restart(kr_interval *t) {
  int ret;
  struct itimerspec wakeup;
  if (!t) return -1;
  t->ticks = 0;
  clock_gettime(CLOCK_MONOTONIC, &t->start);
  memset(&wakeup, 0, sizeof(wakeup));
  if (t->num && t->num) wakeup.it_interval = nsec_to_ts(t->tick_ns);
  wakeup.it_value = add_ns(t->start, t->tick_ns);
  /*printk("%d %ld %ld %ld %ld",t->fd, wakeup.it_interval.tv_sec,
   wakeup.it_interval.tv_nsec, wakeup.it_value.tv_sec,
    wakeup.it_value.tv_nsec);*/
  ret = timerfd_settime(t->fd, TFD_TIMER_ABSTIME, &wakeup, NULL);
  if (ret == -1) {
    printke("timerfd settime %s", strerror(errno));
    return -2;
  }
  return 0;
}

int kr_interval_set(kr_interval *t, uint64_t ms) {
  if (!t || !ms) return -1;
  t->tick_ns = 1000000LL * ms;
  return kr_interval_restart(t);
}

int kr_interval_get_fd(kr_interval *t) {
  if (!t) return -1;
  return t->fd;
}

int kr_interval_destroy(kr_interval *t) {
  if (!t) return -1;
  if (t->fd != -1) close(t->fd);
  t->fd = -1;
  return 0;
}

kr_interval *kr_interval_create(kr_interval_setup *setup) {
  kr_interval *t;
  if (!setup || !setup->mem) return NULL;
  if ( (setup->num == 0 || setup->den == 0) && setup->ms == 0) return NULL;
  t = (kr_interval *)setup->mem;
  memset(t, 0, sizeof(*t));
  if (setup->ms != 0) {
    t->tick_ns = 1000000LL * setup->ms;
  } else {
    t->num = setup->num;
    t->den = setup->den;
    t->tick_ns = (1000000000LL / t->num) * t->den;
  }
  printkd("Interval: %llu ns", t->tick_ns);
  t->fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (t->fd < 0) return NULL;
  kr_interval_restart(t);
  return t;
}

size_t kr_interval_sizeof() {
  kr_interval t;
  return sizeof(t);
}
