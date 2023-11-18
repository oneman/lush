#if !defined(_time_stamp_H)
#define _time_stamp_H (1)

#include "types.h"

int kr_timestamp_now(kr_timestamp_info *info, struct timespec *ts);
uint64_t kr_unixtime();

#endif
