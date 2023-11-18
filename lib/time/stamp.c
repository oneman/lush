#include <time.h>
#include <sys/time.h>
#include "stamp.h"

int kr_timestamp_now(kr_timestamp_info *info, struct timespec *ts) {
  clock_gettime(CLOCK_MONOTONIC, ts);
  info->seconds = ts->tv_sec;
  info->nanoseconds = ts->tv_nsec;
  return 0;
}

uint64_t kr_unixtime() {
  uint64_t seconds;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  seconds = tv.tv_sec;
  return seconds;
}
