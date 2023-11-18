#if !defined(_system_types_H)
#define _system_types_H (1)

typedef enum {
  KR_PRIORITY_UNKNOWN,
  KR_PRIORITY_NORMAL,
  KR_PRIORITY_NONINTERACTIVE,
  KR_PRIORITY_TRANSCODER,
  KR_PRIORITY_REALTIME,
  KR_PRIORITY_REALTIME_IO
} kr_priority;

#define KR_PRIORITY_MIN KR_PRIORITY_NORMAL
#define KR_PRIORITY_MAX KR_PRIORITY_REALTIME_IO

#if !defined(_KR_LAUNCHER_C)
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#endif

#endif
