static void limit_samples(float **samples, int nc, int ns);
/*static void clear_samples(float **dst, int nc, int ns);*/
static void copy_samples(float **dst, float **src, int nc, int ns);
static void sum_samples(float *out, float *src, int ns);
static void sum_channels(float **dst, float **src, int nc, int ns);
static int find_zc_sample(float *s, int ns);
static void find_zc(kr_audio *audio);

static void limit_samples(float **samples, int nc, int ns) {
  int c;
  for (c = 0; c < nc; c++) {
    krad_hardlimit(samples[c], ns);
  }
}

/*static void clear_samples(float **dst, int nc, int ns) {
  int s;
  int c;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] = 0.0f;
    }
  }
}*/

static void clone_channel(float **dst, float **src, int outc, int inc, int ns) {
  int s;
  for (s = 0; s < ns; s++) {
    dst[outc][s] = src[inc][s];
  }
}

static void copy_samples(float **dst, float **src, int nc, int ns) {
  int s;
  int c;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] = src[c][s];
    }
  }
}

static void sum_samples(float *out, float *src, int ns) {
  int s;
  for (s = 0; s < ns; s++) {
    out[s] += src[s];
  }
}

static void sum_channels(float **dst, float **src, int nc, int ns) {
  int c;
  int s;
  for (c = 0; c < nc; c++) {
    for (s = 0; s < ns; s++) {
      dst[c][s] += src[c][s];
    }
  }
}

static int find_zc_sample(float *s, int ns) {
  int i;
  i = 0;
  if (s[i] == 0.0f) return 0;
  if (s[i] < 0.0f) {
    while (++i < ns) if (s[i] >= 0.0f) return i;
  } else {
    while (++i < ns) if (s[i] <= 0.0f) return i;
  }
  return -1;
}

static void find_zc(kr_audio *audio) {
  int c;
  for (c = 0; c < audio->channels; c++) {
    audio->zc[c] = find_zc_sample(audio->samples[c], audio->count);
  }
}
