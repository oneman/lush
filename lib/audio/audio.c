#include "math.h"
#include "audio.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

float kr_lintodb(float val) {
  float db;
  val = MAX(0.000001f, fabs(val));
  db = 20.0f * log10(val);
  return db;
}

float kr_dbtolin(float db) {
 return pow(10, db/20.0f);
}

int kr_audio_zero(kr_audio *a) {
  int c;
  if (!a) return -1;
  if (a->channels < 1) return -2;
  if (a->count < 1) return -3;
  for (c = 0; c < a->channels; c++) {
    memset(a->samples[c], 0, a->count * sizeof(float));
  }
  return 0; 
}

int kr_audio_init(kr_audio_setup *setup) {
  int c;
  size_t offset;
  size_t cs;
  kr_audio *a;
  uint8_t *mem;
  if (!setup) return -1;
  if (!setup->audio) return -1;
  if (!setup->mem) return -1;
  a = setup->audio;
  memset(a, 0, sizeof(*a));
  a->channels = setup->channels;
  a->rate = setup->rate;
  a->count = setup->count;
  offset = 0;
  cs = a->count * sizeof(float);
  for (c = 0; c < a->channels; c++) {
    mem = setup->mem + offset;
    a->samples[c] = (float *)mem;
    offset += cs;
  }
  return 0;
}
