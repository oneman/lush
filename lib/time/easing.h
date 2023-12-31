#ifndef KRAD_EASING_H
#define KRAD_EASING_H

#include <inttypes.h>
#include "types.h"

typedef struct {
  float target;
  int duration;
  kr_easing easing;
  void *ptr;
} kr_easer_update;

typedef struct {
  int active;
  volatile int updated;
  int updating;
  kr_easer_update update;
  float target;
  float start_value;
  float change_amount;
  int elapsed_time;
  int duration;
  kr_easing easing;
} kr_easer;

kr_easing kr_easing_rand();
float kr_ease(kr_easing easing, float now, float start, float amt, float dur);
void kr_easer_set(kr_easer *easer, float target, int dur, kr_easing easing, void *ptr);
float kr_easer_process(kr_easer *easer, float current, void **ptr);
int kr_easer_active(kr_easer *easer);
void kr_easer_destroy(kr_easer *easer);
kr_easer *kr_easer_create();

#endif
