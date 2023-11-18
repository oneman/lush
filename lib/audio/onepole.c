#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include <krad/app/debug.h>

typedef struct {
  double a0;
  double b1;
  double z1;
} kr_onepole;

int kr_onepole_set_fc(kr_onepole *p, double fc) {
  if (!p || fc < 0) return -1;
  p->b1 = exp(-2.0 * M_PI * fc);
  p->a0 = 1.0 - p->b1;
  return 0;
}

size_t kr_onepole_sizeof() {
  return sizeof(kr_onepole);
}

float kr_onepole_get_val(kr_onepole *p) {
  if (!p) return 0;
  return p->z1;
}

float kr_onepole_process(kr_onepole *p, float in) {
  float s;
  if (!p) return 0;
  s = in * p->a0 + p->z1 * p->b1;
  p->z1 = s;
  return s;
}

int kr_onepole_init(uint8_t *mem, float rate) {
  kr_onepole *p;
  if (!mem || rate < 0.0f) return 1;
  p = (kr_onepole *)mem;
  p->z1 = 0;
  kr_onepole_set_fc(p, rate);
  return 0;
}
