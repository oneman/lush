#include "eq.h"

#define KR_EQ_BW_MIN 0.5
#define KR_EQ_BW_MAX 4.0
#define KR_EQ_DB_MIN -50.0
#define KR_EQ_DB_MAX 20.0
#define KR_EQ_HZ_MIN 20.0
#define KR_EQ_HZ_MAX 20000.0

typedef struct {
  biquad filter;
  float db;
  float bw;
  float hz;
} kr_eq_band;

typedef struct {
  kr_eq_band band[KR_EQ_MAX_BANDS];
} band_group;

struct kr_eq {
  band_group *channel;
};

static void calc_band_group(band_group *b, kr_eq_info *info, int sample_rate);
static void apply_band_group(float *dst, float *src, band_group *bg, int ns);

static void calc_band_group(band_group *bg, kr_eq_info *info, int sample_rate) {
  int b;
  for (b = 0; b < KR_EQ_MAX_BANDS; b++) {
    if (bg->band[b].db != info->band[b].db
     || bg->band[b].bw != info->band[b].bw
     || bg->band[b].hz != info->band[b].hz) {
      bg->band[b].db = info->band[b].db;
      bg->band[b].bw = info->band[b].bw;
      bg->band[b].hz = info->band[b].hz;
      eq_set_params(&bg->band[b].filter, bg->band[b].hz, bg->band[b].db,
       bg->band[b].bw, sample_rate);
    }
  }
}

static void apply_band_group(float *dst, float *src, band_group *bg, int ns) {
  int s;
  int i;
  int run;
  run = 0;
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    if (bg->band[i].db == 0.0f) continue;
    for (s = 0; s < ns; s++) {
      dst[s] = biquad_run(&bg->band[i].filter, src[s]);
    }
    if (!run) {
      src = dst;
      run++;
    }  
  }
  if (!run && dst != src) memcpy(dst, src, sizeof(src[0]) * ns);
}

size_t kr_eq_sizeof(int nchannels) {
  size_t sz;
  kr_eq *e;
  sz = sizeof(*e);
  sz += sizeof(*e->channel) * nchannels;
  return sz;
}

kr_eq *kr_eq_init(void *mem, int nchannels, int sample_rate,
 kr_eq_info *info) {
  int b;
  int c;
  kr_eq *eq;
  eq = (kr_eq *)mem;
  eq->channel = mem + sizeof(*eq);
  for (c = 0; c < nchannels; c++) {
    for (b = 0; b < KR_EQ_MAX_BANDS; b++) {
      biquad_init(&eq->channel[c].band[b].filter);
    }
    calc_band_group(&eq->channel[c], &info[c], sample_rate);
  }
  return eq;
}

int kr_eq_process(kr_eq *eq, kr_audio *out, kr_audio *in, kr_eq_info *info) {
  int c;
  for (c = 0; c < in->channels; c++) {
    calc_band_group(&eq->channel[c], &info[c], in->rate);
    apply_band_group(out->samples[c], in->samples[c], &eq->channel[c],
     in->count);
  }
  return in->count;
}
