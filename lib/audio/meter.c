#include <math.h>
#include <krad/audio/onepole.h>
#include "meter.h"

typedef struct {
  uint16_t count;
  kr_meter_info info;
  kr_amem(onepole_mem, 4 * sizeof(double));
} meter_channel;

struct kr_meter {
  uint16_t hold;
  meter_channel *channel;
};

size_t kr_meter_sizeof(int nchannels) {
  size_t sz;
  kr_meter *m;
  sz = sizeof(*m);
  sz += sizeof(*m->channel) * nchannels;
  return sz;
}

kr_meter *kr_meter_init(void *mem, int nchannels) {
  int c;
  kr_meter *m;
  m = (kr_meter *)mem;
  m->channel = mem + sizeof(*m);
  for (c = 0; c < nchannels; c++) {
    m->channel[c].count = 0;
    kr_meter_info_init(&m->channel[c]);
    kr_onepole_init(m->channel[c].onepole_mem, 250 / 48000.0 / 500);
  }
  m->hold = 0;
  return m;
}

int kr_meter_process(kr_meter *m, kr_audio *a, kr_meter_info *info) {
  if (!m || !a || !info) return -1;
  kr_onepole *p;
  int c;
  int s;
  float samp;
  float peak_samp;
  float peak_db;
  float rate_factor;
  rate_factor = a->count / 256.0;
  m->hold = 48000.0 /  (8 * a->count);
  for (c = 0; c < a->channels; c++) {
    p = (kr_onepole *)m->channel[c].onepole_mem;
    peak_samp = 0.0f;
    for (s = 0; s < a->count; s++) {
      samp = fabs(a->samples[c][s]);
      kr_onepole_process(p, samp);
      if (samp > peak_samp) peak_samp = samp;
    }
    peak_db = kr_lintodb(peak_samp);
    if (peak_db >= m->channel[c].info.peak) {
      m->channel[c].info.peak = peak_db;
      m->channel[c].count = m->hold;
    } else if (m->channel[c].count) {
       m->channel[c].count--;
    }  else {
      if (m->channel[c].info.peak > -120.0f) {
        m->channel[c].info.peak -= 0.1f * rate_factor;
      } else {
        m->channel[c].info.peak = -120.0f;
      }
    }
    samp = kr_onepole_get_val(p);
    m->channel[c].info.avg = kr_lintodb(samp);
  }
  for (c = 0; c < a->channels; c++) {
    info[c] = m->channel[c].info;
  }
  return a->count;
}
