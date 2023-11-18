static int blend_in(kr_mixer_path *mix, kr_mixer_path *in);
static int blend_out(kr_mixer_path *mix, kr_mixer_path *in);

static int blend_in(kr_mixer_path *mix, kr_mixer_path *in) {
  int c;
  int nc;
  int ns;
  int zc;
  int blended;
  ns = in->nframes;
  nc = in->channels;
  blended = 0;
  for (c = 0; c < nc; c++) {
    if (in->fused[c]) {
      sum_samples(mix->samples[c], in->samples[c], ns);
      blended++;
    } else {
      zc = find_zc_sample(in->samples[c], ns);
      if (zc == -1) continue;
      sum_samples(mix->samples[c] + zc, in->samples[c] + zc, ns - zc);
      in->fused[c] = 1;
      blended++;
    }
  }
  return blended;
}

static int blend_out(kr_mixer_path *mix, kr_mixer_path *in) {
  int c;
  int nc;
  int ns;
  int zc;
  int blended;
  ns = in->nframes;
  nc = in->channels;
  blended = nc;
  for (c = 0; c < nc; c++) {
    if (!in->fused[c]) {
      blended--;
    } else {
      zc = find_zc_sample(in->samples[c], ns);
      if (zc == -1) continue;
      sum_samples(mix->samples[c], in->samples[c], zc);
      in->fused[c] = 0;
      blended--;
    }
  }
  return blended;
}
