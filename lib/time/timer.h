#include <inttypes.h>
#include <time.h>

#ifndef KRAD_TIMER_H
#define KRAD_TIMER_H

typedef struct kr_timer kr_timer;

struct kr_timer {
  struct timespec start;
  struct timespec sample;
  struct timespec finish;
  const char *name;
  int32_t started;
};

int kr_timer_name_set(kr_timer *timer, const char *name);
#define kr_timer_alloca() alloca(sizeof(kr_timer))
kr_timer *kr_timer_create();
kr_timer *kr_timer_create_with_name(const char *name);
void kr_timer_status(kr_timer *timer);
void kr_timer_start(kr_timer *timer);
int32_t kr_timer_started(kr_timer *timer);
uint64_t kr_timer_sample_duration_ms(kr_timer *timer);
uint64_t kr_timer_current_ms(kr_timer *timer);
void kr_timer_finish(kr_timer *timer);
uint64_t kr_timer_duration_ms(kr_timer *timer);
void kr_timer_destroy(kr_timer *timer);

#endif
