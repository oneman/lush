#include <math.h>
#include "convert.h"

int16_t kr_audio_float_to_16bit(float samp) {
  double scaled;
  int16_t res;
  scaled = samp * (8.0 * 0x10000000);
  res = (lrint(scaled) >> 16);
  return res;
}

int32_t kr_audio_float_to_24bit(float samp) {
  double scaled;
  int32_t res;
  scaled = samp * (8.0 * 0x10000000);
  res = (lrint(scaled) >> 8);
  return res;
}

float kr_audio_16bit_to_float(int samp) {
  float f;
  f = (float)((samp << 16) / (8.0 * 0x10000000));
  return f;
}

float kr_audio_24bit_to_float(int samp) {
  float f;
  f = (float) ((samp << 8) / (8.0 * 0x10000000));
  return f;
}

int kr_audio_interleave(float *out, kr_audio *a) {
  int i;
  int c;
  int s;
  i = 0;
  if (!out) return -1;
  if (!a) return -2;
  for (s = 0; s < a->count; s++) {
    for (c = 0; c < a->channels; c++) {
      out[i++] = a->samples[c][s];
    }
  }
  return 0;
}

int kr_audio_deinterleave(kr_audio *out, float *input) {
  int i;
  int c;
  int s;
  for (i = s = 0; i < (out->count * out->channels) ; i += out->channels) {
    for (c = 0; c < out->channels; c++) {
      out->samples[c][s] = input[i + c];
    }
    s++;
  }
  return 0;
}

int kr_audio_deinterleave_16bit(kr_audio *out, int16_t *input) {
  int i;
  int c;
  int s;
  for (i = s = 0; i < (out->count * out->channels) ; i += out->channels) {
    for (c = 0; c < out->channels; c++) {
      out->samples[c][s] = kr_audio_16bit_to_float(input[i + c]);
    }
    s++;
  }
  return 0;
}

int kr_audio_interleave_16bit(int16_t *out, kr_audio *a) {
  int i;
  int c;
  int s;
  i = 0;
  if (!out) return -1;
  if (!a) return -2;
  for (s = 0; s < a->count; s++) {
    for (c = 0; c < a->channels; c++) {
      out[i++] = kr_audio_float_to_16bit(a->samples[c][s]);
    }
  }
  return 0;
}

int kr_audio_interleave_16bit_in_32bit(int32_t *out, kr_audio *a) {
  int i;
  int c;
  int s;
  i = 0;
  if (!out) return -1;
  if (!a) return -2;
  for (s = 0; s < a->count; s++) {
    for (c = 0; c < a->channels; c++) {
      out[i++] = kr_audio_float_to_16bit(a->samples[c][s]);
    }
  }
  return 0;
}

int kr_audio_interleave_24bit(int32_t *out, kr_audio *a) {
  int i;
  int c;
  int s;
  i = 0;
  for (s = 0; s < a->count; s++) {
    for (c = 0; c < a->channels; c++) {
      out[i++] = kr_audio_float_to_24bit(a->samples[c][s]);
    }
  }
  return 0;
}

