#include "pass.h"
#include <krad/app/debug.h>

#define BW_MIN 0.666
#define BW_MAX 4
#define HZ_MIN 20
#define HZ_MAX 20000

typedef struct {
  biquad filter;
  float hz;
  float bw;
} pass;

typedef struct {
  pass high;
  pass low;
} pass_group;

struct kr_pass {
  pass_group *channel;
};

static void calc_pass_group(pass_group *p, kr_pass_info *info, int sample_rate);
static void apply_pass_group(float *dst, float *src, pass_group *p, int ns);

static void calc_pass_group(pass_group *p, kr_pass_info *info, int sample_rate) {
  if (p->high.hz != info->high.hz || p->high.bw != info->high.bw) {
    p->high.hz = info->high.hz;
    p->high.bw = info->high.bw;
    hp_set_params(&p->high.filter, p->high.hz, p->high.bw, sample_rate);
  }
  if (p->low.hz != info->low.hz || p->low.bw != info->low.bw) {
    p->low.hz = info->low.hz;
    p->low.bw = info->low.bw;
    lp_set_params(&p->low.filter, p->low.hz, p->low.bw, sample_rate);
  }
}

static void apply_pass_group(float *dst, float *src, pass_group *p, int ns) {
  int s;
  int run;
  run = 0;
  if (p->low.hz < HZ_MAX && p->low.bw > BW_MIN) {
    for (s = 0; s < ns; s++) {
      dst[s] = biquad_run(&p->low.filter, src[s]);
    }
    src = dst;
    run++;
  }
  if (p->high.hz > HZ_MIN && p->high.bw > BW_MIN) {
    for (s = 0; s < ns; s++) {
      dst[s] = biquad_run(&p->high.filter, src[s]);
    }
    run++;
  }
  if (!run && dst != src) memcpy(dst, src, sizeof(src[0]) * ns);
}

size_t kr_pass_sizeof(int nchannels) {
  size_t sz;
  kr_pass *p;
  sz = sizeof(*p);
  sz += sizeof(*p->channel) * nchannels;
  //printk("sz: %d", sz);
  return sz;
}

kr_pass *kr_pass_init(void *mem, int nchannels, int sample_rate,
 kr_pass_info *info) {
  kr_pass *p;
  int c;
  p = (kr_pass *)mem;
  p->channel = mem + sizeof(*p);
  for (c = 0; c < nchannels; c++) {
    biquad_init(&p->channel[c].low.filter);
    biquad_init(&p->channel[c].high.filter);
    calc_pass_group(&p->channel[c], &info[c], sample_rate);
  }
  return p;
}

int kr_pass_process(kr_pass *pass, kr_audio *out, kr_audio *in,
 kr_pass_info *info) {
  int c;
  for (c = 0; c < in->channels; c++) {
    calc_pass_group(&pass->channel[c], &info[c], in->rate);
    apply_pass_group(out->samples[c], in->samples[c], &pass->channel[c],
     in->count);
  }
  return in->count;
}
