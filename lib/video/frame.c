#include <krad/app/debug.h>
#include "frame.h"

void kr_frame_debug_time(kr_frame *frame, char *str, size_t sz) {
  printk("%.*s Frame TC: %zu:%zu", sz, str, frame->info.ts.tv_sec,
   frame->info.ts.tv_usec);
}

uint64_t kr_frame_addtime(kr_frame *frame, uint64_t tc) {
  if (frame == NULL) return 0;
  frame->info.tc += tc;
  return frame->info.tc;
}

int kr_frame_copytime(kr_frame *out, kr_frame *in) {
  if (in == NULL) return -1;
  if (out == NULL) return -1;
  out->info.ts = in->info.ts;
  return 0;
}
