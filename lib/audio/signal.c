#include <math.h>
#include <krad/app/debug.h>
#include "signal.h"

/*typedef struct {
  float tone[2];
} dtmf_tone;
*/

#define KR_DTMF_NKEYS 16

typedef struct {
  float low;
  float high;
} dtmf_tone_pair;

typedef enum {
  KR_DTMF_0,
  KR_DTMF_1,
  KR_DTMF_2,
  KR_DTMF_3,
  KR_DTMF_4,
  KR_DTMF_5,
  KR_DTMF_6,
  KR_DTMF_7,
  KR_DTMF_8,
  KR_DTMF_9,
  KR_DTMF_STAR,
  KR_DTMF_HASH,
  KR_DTMF_A,
  KR_DTMF_B,
  KR_DTMF_C,
  KR_DTMF_D
} kr_dtmf_name;

static const char dtmf_keys[] = {
  [KR_DTMF_0] = '0',
  [KR_DTMF_1] = '1',
  [KR_DTMF_2] = '2',
  [KR_DTMF_3] = '3',
  [KR_DTMF_4] = '4',
  [KR_DTMF_5] = '5',
  [KR_DTMF_6] = '6',
  [KR_DTMF_7] = '7',
  [KR_DTMF_8] = '8',
  [KR_DTMF_9] = '9',
  [KR_DTMF_STAR] = '*',
  [KR_DTMF_HASH] = '#',
  [KR_DTMF_A] = 'a',
  [KR_DTMF_B] = 'b',
  [KR_DTMF_C] = 'c',
  [KR_DTMF_D] = 'd'
};

/*static const float dtmf_tone_row[] = {
  697, 770, 852, 941
};

static const float dtmf_tone_col[] = {
  1209, 1336, 1477, 1633
};*/

static const float dtmf_tones[] = {
  697, 770, 852, 941, 1209, 1336, 1477, 1633
};

#define KR_DTMF_NTONES 8

static const dtmf_tone_pair dtmf_tone_pairs[] = {
  [KR_DTMF_0] = {941, 1336},
  [KR_DTMF_1] = {697, 1209},
  [KR_DTMF_2] = {697, 1336},
  [KR_DTMF_3] = {697, 1477},
  [KR_DTMF_4] = {770, 1209},
  [KR_DTMF_5] = {770, 1336},
  [KR_DTMF_6] = {770, 1477},
  [KR_DTMF_7] = {852, 1209},
  [KR_DTMF_8] = {852, 1336},
  [KR_DTMF_9] = {852, 1477},
  [KR_DTMF_STAR] = {941, 1209},
  [KR_DTMF_HASH] = {941, 1477},
  [KR_DTMF_A] = {697, 1633},
  [KR_DTMF_B] = {770, 1633},
  [KR_DTMF_C] = {852, 1633},
  [KR_DTMF_D] = {941, 1633}
};

typedef enum {
  KR_SIGNAL_GEN_OFF,
  KR_SIGNAL_GEN_ACTIVE
} kr_signal_gen_state;

typedef enum {
  KR_SIGNALING_SYMBOL,
  KR_SIGNALING_SPACE
} kr_signaling_state;

typedef struct {
  float hz;
  float delta;
  float angle;
  int16_t nsamples;
  int8_t active;
  int8_t last_sign;
} kr_sine;

struct kr_signal_gen {
  int sample_rate;
  kr_signal_info info;
  kr_signal signal;
  int pos;
  kr_signal_gen_state state;
  kr_signaling_state signaling_state;
  int nsamples_space;
  kr_sine sine[2];
};

static int samples_per_ms(kr_signal_gen *g) {
  return g->sample_rate / 1000;
}

static void sine_set(kr_signal_gen *g, kr_sine *s, float hz) {
  s->hz = hz;
  s->delta = (2.0f * M_PI * s->hz) / g->sample_rate;
  s->angle = 0.0f;
  s->nsamples = g->info.dtmf.mark_duration * samples_per_ms(g);
  s->active = 1;
  s->last_sign = 0;
}

static void dtmf_set(kr_signal_gen *g, char key) {
  int i;
  dtmf_tone_pair pair;
  for (i = 0; i < KR_DTMF_NKEYS; i++) {
    if (dtmf_keys[i] == key) {
      pair = dtmf_tone_pairs[i];
      sine_set(g, &g->sine[0], pair.low);
      sine_set(g, &g->sine[1], pair.high);
      printk("dtmf set to %f and %f", pair.low, pair.high);
      return;
    }
  }  
}

static int valid_key(char key) {
  int i;
  for (i = 0; i < KR_DTMF_NKEYS; i++) {
    if (dtmf_keys[i] == key) return 1;
  }
  printk("Signal gen DTMF: key %c is invalid", key);
  return 0;
}

static int valid_signal(kr_signal *s) {
  int i;
  char key;
  if (s->len < 1 || s->len > sizeof(s->str)) return 0;
  if (s->type != KR_SIGNAL_GEN_DTMF) return 0;
  for (i = 0; i < s->len; i++) {
    key = s->str[i];
    if (!valid_key(key)) return 0;
  }
  return 1; 
}

 /*void krad_tone_run(kr_tone *krad_tone, float *buffer, int numsamples) {
  int t;
  int s;
  float sample;
  int sign;
  sign = 0;
    if (krad_tone->tones[t].active == 2) {
      for (s = 0; s < numsamples; s++) {
        sample = krad_tone->volume_actual * sin(krad_tone->tones[t].angle);



        buffer[s] += sample;
        krad_tone->tones[t].angle += krad_tone->tones[t].delta;
        if (krad_tone->tones[t].angle > M_PI) {
          krad_tone->tones[t].angle -= 2.0f * M_PI;
        }
        krad_tone->tones[t].last_sign = sign;
      }
    }
  }
}*/

size_t kr_signal_gen_sizeof() {
  return sizeof(kr_signal_gen);
}

int kr_signal_gen_setup_init(kr_signal_gen_setup *setup) {
  if (!setup) return -1;
  memset(setup, 0, sizeof(*setup));
  setup->sample_rate = 48000;
  setup->info.dtmf.mark_duration = 70;
  setup->info.dtmf.space_duration = 60;
  setup->info.morse.dit_duration = 50;
  return 0;
}

kr_signal_gen *kr_signal_gen_init(kr_signal_gen_setup *setup) {
  kr_signal_gen *g;
  if (!setup) return NULL;
  g = (kr_signal_gen *)setup->mem;
  g->sample_rate = setup->sample_rate;
  g->info = setup->info;
  g->state = KR_SIGNAL_GEN_OFF;
  return g;
}

int kr_signal_gen_ctl(kr_signal_gen *g, kr_signal_info *info) {
  if (!g || !info) return -1;
  g->info = *info;
  return 0;
}

int kr_signal_gen_put(kr_signal_gen *g, kr_signal *s) {
  if (!g || !s) return -1;
  if (!valid_signal(s)) return -2;
  printk("Valid signal put. %d keys long", s->len);
  g->signal = *s; 
  g->state = KR_SIGNAL_GEN_ACTIVE;
  g->signaling_state = KR_SIGNALING_SYMBOL;
  g->pos = 0;
  dtmf_set(g, g->signal.str[g->pos]);
  return 0;
}

#define KR_DTMF_VOLUME 0.25 * 0.25

static int sine_run(kr_signal_gen *g, kr_audio *audio, kr_sine *s) {
  int i;
  int c;
  int ns;
  float samp;
  int finish;
  int sign;
  if (!s->active) return 0;
  finish = 0;
  ns = s->nsamples;
  if (audio->count < ns) ns = audio->count;
  s->nsamples -= ns; 
  if (s->nsamples == 0) {
    ns = audio->count;
    finish = 1;
  }
  for (i = 0; i < ns; i++) {
    samp = KR_DTMF_VOLUME * sin(s->angle);
    if (finish) {
      if (samp > 0.0f) {
        sign = 1;
      } else {
        sign = -1;
      }
      if (s->last_sign != 0 && s->last_sign != sign) {
        s->active = 0;
        return i;
      }
      s->last_sign = sign;
    }
    s->angle += s->delta;
    if (s->angle > M_PI) {
      s->angle -= 2.0f * M_PI;
    }
    for (c = 0; c < audio->channels; c++) {
      audio->samples[c][i] += samp;
    }
  }
  return ns;
}

static int dtmf_run(kr_signal_gen *g, kr_audio *audio) {
  int ret;
  int ret2;
  ret = 0;
  ret2 = 0;
  if (g->signaling_state == KR_SIGNALING_SPACE) {
      g->nsamples_space -= audio->count;
      if (g->nsamples_space <= 0) {
        if (g->pos == g->signal.len) {
          g->state = KR_SIGNAL_GEN_OFF;
          return 0;
        } else {
          g->signaling_state = KR_SIGNALING_SYMBOL;
          g->pos++;
          dtmf_set(g, g->signal.str[g->pos]);
        }
      }
  }
  if (g->signaling_state == KR_SIGNALING_SYMBOL) {
    ret = sine_run(g, audio, &g->sine[0]);
    ret2 = sine_run(g, audio, &g->sine[1]);
    if (ret2 > ret) ret = ret2;
    if (ret != audio->count) {
      g->signaling_state = KR_SIGNALING_SPACE;
      g->nsamples_space = g->info.dtmf.space_duration * samples_per_ms(g);
      g->nsamples_space -= (audio->count - ret);
    }
  }
  return ret;
}

float goertzel(float *x, int N, float frequency, int samplerate) {
  float Skn, Skn1, Skn2;
  Skn = Skn1 = Skn2 = 0;
  for (int i=0; i<N; i++) {
    Skn2 = Skn1;
    Skn1 = Skn;
    Skn = 2*cos(2*M_PI*frequency/samplerate)*Skn1 - Skn2 + x[i];
  }
  float WNk = exp(-2*M_PI*frequency/samplerate); // this one ignores complex stuff
  //float WNk = exp(-2*j*PI*k/N);
  return (Skn - WNk*Skn1);
}

float tone_power(float value) {
    return 20*log10f(fabs(value));
}

static void dtmf_detector(kr_audio *audio) {
  float hz;
  float db;
  int i;
  int c;
  c = 0;
  for (i = 0; i < KR_DTMF_NTONES; i++) {
    hz = dtmf_tones[i];
    db = tone_power(goertzel(audio->samples[c], audio->count, hz, audio->rate));
    if (db > 25.0) printk("detected freq: %gHz  ->  dB: %f", hz, db);
  }
}

int kr_signal_emit(kr_signal_gen *g, kr_audio *audio) {
  if (!g || !audio) return -1;
  if (g->state == KR_SIGNAL_GEN_OFF) return 0;
  if (g->signal.type == KR_SIGNAL_GEN_DTMF) {
    dtmf_run(g, audio);
    dtmf_detector(audio);
  }
  return 1;
}
