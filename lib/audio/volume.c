#include <stdlib.h>
#include "volume.h"

typedef struct {
  float level;
} volume;

struct kr_volume {
  volume *channel;
};

static int apply_volume_pre_zc(float *dst, float *src, float level, int ns);
static void apply_volume(float *dst, float *src, float level, int ns);

static int apply_volume_pre_zc(float *dst, float *src, float level, int ns) {
  int s;
  int cross;
  cross = (!(src[0] > 0.0f));
  for (s = 0; s < ns; s++) {
    if (src[s] == 0.0f) break;
    if ((src[s] > 0.0f) == cross) break;
    dst[s] = src[s] * level;
  }
  return s;
}

static void apply_volume(float *dst, float *src, float level, int ns) {
  int s;
  if (level == 0.0f) {
    memset(dst, 0, ns * sizeof(dst[0]));
    return;
  } else if (level == 1.0f) {
    if (src != dst) {
      memcpy(dst, src, ns * sizeof(dst[0]));
    }
    return;
  }
  for (s = 0; s < ns; s++) {
    dst[s] = src[s] * level;
  }
}

size_t kr_volume_sizeof(int nchannels) {
  size_t sz;
  kr_volume *v;
  sz = sizeof(*v);
  //sz = 0;
  sz += sizeof(*v->channel) * nchannels;
  return sz;
}

kr_volume *kr_volume_init(void *mem, int nchannels, int sample_rate,
 kr_volume_info *info) {
  kr_volume *v;
  int c;
  v = (kr_volume *)mem;
  v->channel = mem + sizeof(*v);
  for (c = 0; c < nchannels; c++) {
    v->channel[c].level = kr_dbtolin(info[c].gain + info[c].fader);
  }
  return v;
}

int kr_volume_process(kr_volume *volume, kr_audio *out, kr_audio *in,
 kr_volume_info *info) {
  int c;
  int s;
  int ns;
  float level;
  float new_level;
  float *src;
  float *dst;
  if (in->channels != out->channels) return -1;
  if (in->count > out->count) return -2;
  ns = in->count;
  for (c = 0; c < out->channels; c++) {
    src = in->samples[c];
    dst = out->samples[c];
    level = volume->channel[c].level;
    new_level = kr_dbtolin(info[c].gain + info[c].fader);
    if (level == new_level) {
      apply_volume(dst, src, level, ns);
    } else {
      s = apply_volume_pre_zc(dst, src, level, ns);
      if (s != ns) {
        src = in->samples[c] + s;
        dst = out->samples[c] + s;
        apply_volume(dst, src, new_level, ns - s);
        volume->channel[c].level = new_level;
      }
    }
  }
  return in->count;
}
