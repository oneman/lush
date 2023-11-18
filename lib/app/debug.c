#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include <krad/app/spinlock.h>

static kr_spinlock print_lock;

static uint64_t krad_unixtime();

static uint64_t krad_unixtime() {
  uint64_t seconds;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  seconds = tv.tv_sec;
  return seconds;
}

void kr_print_noop(char* format, ...) {
  return;
}

void kr_print(char* format, ...) {
  va_list args;
  kr_spin_lock(&print_lock);
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  fprintf(stdout, "\n");
  kr_spin_unlock(&print_lock);
}

void kr_print_err(char* format, ...) {
  va_list args;
  kr_spin_lock(&print_lock);
  fprintf(stderr, "Error: %"PRIu64" :: ", krad_unixtime());
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  kr_spin_unlock(&print_lock);
}

void failfast(char* format, ...) {
  va_list args;
  kr_spin_lock(&print_lock);
  fprintf(stderr, "***FAIL!: %"PRIu64" :: ", krad_unixtime());
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}
