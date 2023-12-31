#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <alloca.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "timer.h"

static inline uint64_t ts_to_ms(struct timespec ts) {
  return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

kr_timer *kr_timer_create() {
  return kr_timer_create_with_name("");
}

kr_timer *kr_timer_create_with_name(const char *name) {
  kr_timer *timer = kr_allocz(1, sizeof(kr_timer));
  timer->name = name;
  return timer;
}

int kr_timer_name_set(kr_timer *timer, const char *name) {
  if (timer == NULL) return -1;
  if (name == NULL) return -2;
  timer->name = name;
  return 0;
}

void kr_timer_status(kr_timer *timer) {
  printk("%s timer: %"PRIu64"ms", timer->name,
   kr_timer_sample_duration_ms(timer));
}

void kr_timer_start(kr_timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->start);
  timer->started = 1;
}

int32_t kr_timer_started(kr_timer *timer) {
  return timer->started;
}

void kr_timer_finish(kr_timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->finish);
}

uint64_t kr_timer_sample_duration_ms(kr_timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->sample );
  return ts_to_ms(timer->sample) - ts_to_ms(timer->start);
}

uint64_t kr_timer_current_ms(kr_timer *timer) {
  if (timer->started == 0) {
    return 0;
  }
  return kr_timer_sample_duration_ms(timer);
}

uint64_t kr_timer_duration_ms(kr_timer *timer) {
  return ts_to_ms(timer->finish) - ts_to_ms(timer->start);
}

void kr_timer_destroy(kr_timer *timer) {
  free(timer);
}
