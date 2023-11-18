#include "hardlimiter.h"
#include <math.h>
#include <krad/app/debug.h>

void kr_rufflimit(float *samples, int count) {
  static int dboog = 0;
  static int sclips = 0;
  static int hclips = 0;
  int run;
  float pre;
  int s;
  float thresh;
  float alpha;
  float top;
  float bottom;
  float samp;
  float upper;

  upper = 2.0f;
  run = 0;
  pre = 0.0f;
  samp = 0.0f;
  top = 0.0f;
  bottom = 0.0f;

  //thresh = 0.65f;
  //alpha = 8.80573;
  //thresh = 0.75f;
  //alpha = 13.30200;
  //thresh = 0.5;
  //alpha = 5.71144;
  thresh = 0.90f;
  alpha = 41.15485;
  if (!count) return;
  for (s = 0; s < count; s++ ) {
    samp = fabs(samples[s]);
    if (samp <= thresh) continue;
    sclips++;
    if (samp > upper) {
      samp = upper;
      hclips++;
    }
    if (!run) pre = samp;
    top = log(1 + alpha * ((samp - thresh) / (upper - thresh)));
    bottom = log(1 + alpha);
    samp = thresh + (1 - thresh) * (top / bottom);
    samples[s] = copysign(samp, samples[s]);
    if (samp > 1.0f) {
      s--;
      continue;
    }
    if (!run) {
      run = 1;
      if (dboog++ % 400) continue;
      samp = samples[s];
      if (0) printk("%d Pre: %f Lim: %f :: top %f bottom %f", dboog, pre, samp, top, bottom);
      if (0) printk("%d soft clips %d hard clips", sclips, hclips);
      sclips = 0;
      hclips = 0;
    }
  }
}

void kr_distort(float *samples, int count) {
  int s;
  float a;
  float b;
  float g;
  float samp;
  float thresh;
  float over;
  thresh = 0.5f;
  g = 1.0f;
  a = 0;
  b = 0;
  for (s = 0; s < count; s++ ) {
    samp = samples[s];
    if (samp > thresh) {
      over = samp - thresh;
      samp -= thresh;
      samp += (expf(over * (a + g)) - expf(over * (b - g))) / (expf(over * g) + expf(over * -g));
    }
    samples[s] = samp;
  }
}

void krad_hardlimit(float *samples, int count) {
  int s;
  kr_rufflimit(samples, count);
  //kr_distort(samples, count);
  for (s = 0; s < count; s++ ) {
    if (samples[s] < -1.0f) {
      samples[s] = -1.0f;
    } else if (samples[s] > 1.0f) {
      samples[s] = 1.0f;
    }
  }
}
