#include <stdlib.h>
#include "biquad.h"
#include "analog.h"

#define ANALOG_DRIVE_MIN 0.1f
#define ANALOG_DRIVE_MAX 10.0f
#define ANALOG_BLEND_MIN -10.0f
#define ANALOG_BLEND_MAX 10.0f
#define EPS 0.000000001f

typedef struct {
  float drive;
  float blend;
  float prev_med;
  float prev_out;
  float rdrive;
  float rbdr;
  float kpa;
  float kpb;
  float kna;
  float knb;
  float ap;
  float an;
  float imr;
  float kc;
  float srct;
  float sq;
  float pwrq;
} analog;

struct kr_analog {
  analog *channel;
};

static inline float M(float x) {
  if (x > EPS || x < -EPS) return x;
  return 0.0f;
}

static inline float D(float x) {
  if (x > EPS) {
    return sqrt(x);
  } else if (x < -EPS) {
    return sqrt(-x);
  }
  return 0.0f;
}

static void calc_analog(analog *a, kr_analog_info *info, int sample_rate);
static void apply_analog(float *dst, float *src, analog *a, int ns);

static void calc_analog(analog *a, kr_analog_info *info, int sample_rate) {
  float drive;
  float blend;
  drive = LIMIT(info->drive, ANALOG_DRIVE_MIN, ANALOG_DRIVE_MAX);
  blend = LIMIT(info->blend, ANALOG_BLEND_MIN, ANALOG_BLEND_MAX);
  if (a->drive == drive && a->blend == blend) return;
  a->drive = drive;
  a->blend = blend;
  a->rdrive = 12.0f / a->drive;
  a->rbdr = a->rdrive / (10.5f - blend) * 780.0f / 33.0f;
  a->kpa = D(2.0f * (a->rdrive*a->rdrive) - 1.0f) + 1.0f;
  a->kpb = (2.0f - a->kpa) / 2.0f;
  a->ap = ((a->rdrive * a->rdrive) - a->kpa + 1.0f) / 2.0f;
  a->kc = a->kpa / D(2.0f * D(2.0f * (a->rdrive * a->rdrive) - 1.0f) - 2.0f * a->rdrive * a->rdrive);
  a->srct = (0.1f * sample_rate) / (0.1f * sample_rate + 1.0f);
  a->sq = a->kc * a->kc + 1.0f;
  a->knb = -1.0f * a->rbdr / D(a->sq);
  a->kna = 2.0f * a->kc * a->rbdr / D(a->sq);
  a->an = a->rbdr * a->rbdr / a->sq;
  a->imr = 2.0f * a->knb + D(2.0f * a->kna + 4.0f * a->an - 1.0f);
  a->pwrq = 2.0f / (a->imr + 1.0f);
}

static void apply_analog(float *dst, float *src, analog *a, int ns) {
  int s;
  float in;
  float out;
  float med;
  if (a->drive < ANALOG_DRIVE_MIN) {
    if (src != dst) {
      memcpy(dst, src, ns * sizeof(dst[0]));
    }
    return;
  }
  for (s = 0; s < ns; s++) {
    in = src[s];
    if (in >= 0.0f) {
      med = (D(a->ap + in * (a->kpa - in)) + a->kpb) * a->pwrq;
    } else {
      med = (D(a->an - in * (a->kna + in)) + a->knb) * a->pwrq * -1.0f;
    }
    out = a->srct * (med - a->prev_med + a->prev_out);
    if (out < -1.0f) out = -1.0f;
    dst[s] = out;
    a->prev_med = M(med);
    a->prev_out = M(out);
  }
}

size_t kr_analog_sizeof(int nchannels) {
  size_t sz;
  sz = sizeof(kr_analog);
  sz += sizeof(analog) * nchannels;
  return sz;
}

kr_analog *kr_analog_init(void *mem, int nchannels, int sample_rate,
 kr_analog_info *info) {
  int c;
  kr_analog *analog;
  analog = (kr_analog *)mem;
  analog->channel = mem + sizeof(*analog);
  for (c = 0; c < nchannels; c++) {
    calc_analog(&analog->channel[c], info, sample_rate);
  }
  return analog;
}

int kr_analog_process(kr_analog *a, kr_audio *out, kr_audio *in,
 kr_analog_info *info) {
  int c;
  if (in->channels != out->channels) return -1;
  if (in->count > out->count) return -2;
  for (c = 0; c < out->channels; c++) {
    calc_analog(&a->channel[c], &info[c], in->rate);
    apply_analog(out->samples[c], in->samples[c], &a->channel[c], in->count);
  }
  return in->count;
}
