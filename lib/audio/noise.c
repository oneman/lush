#include "string.h"
#include "inttypes.h"
#include "noise.h"

#define NOISE_LEVEL 0.075f

struct kr_noise {
  uint32_t seed;
  float brown;
};

static float gen_val(kr_noise *n) {
  static const float scale = NOISE_LEVEL;
  float w;
  uint32_t val;
  w = 0.0;
  n->seed = (n->seed * 196314165) + 907633515;
  val = n->seed >> 9;
  val |= 0x40000000;
  memcpy(&w, &val, sizeof(val));
  w -= 3.0f;
  w *= scale;
  return w;
}

size_t kr_noise_sizeof() {
  return sizeof(kr_noise);
}

int kr_noise_init(kr_noise *n) {
  if (!n) return -1;
  n->seed = 66666666;
  n->brown = 0.0f;
  return 0;
}

float kr_noise_gen(kr_noise *n) {
  float r;
  for (;;) {
    r = gen_val(n);
    n->brown += r;
    if (n->brown < -8.0f || n->brown > 8.0f) {
      n->brown -= r;
      continue;
    }
    break;
  }
  return n->brown * 0.0625f;
}

/*
  if (level > 1.0f || level < 0.0f) return -4;
*/

int kr_noise_fill(kr_noise *n, kr_audio *a) {
  int i;
  int c;
  float val;
  if (!n || !a) return -1;
  if (a->channels < 1) return -2;
  if (a->count < 1) return -3;
  for (i = 0; i < a->count; i++) {
    val = kr_noise_gen(n);
    for (c = 0; c < a->channels; c++) {
      a->samples[c][i] = val;
    }
  }
  return 0; 
}

int kr_noise_add(kr_noise *n, kr_audio *a) {
  int i;
  int c;
  float val;
  if (!n || !a) return -1;
  if (a->channels < 1) return -2;
  if (a->count < 1) return -3;
  for (i = 0; i < a->count; i++) {
    val = kr_noise_gen(n);
    for (c = 0; c < a->channels; c++) {
      a->samples[c][i] += val;
    }
  }
  return 0;
}
