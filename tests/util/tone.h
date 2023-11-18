#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#if !defined(_audio_tone_H)
# define _audio_tone_H (1)

#define MAX_TONES 32
#define KR_TONE_DEFAULT_VOLUME 35

typedef struct kr_tone kr_tone;
typedef struct kr_tone krad_tone_t;

kr_tone *krad_tone_create(float sample_rate);
void krad_tone_set_sample_rate(kr_tone *krad_tone, float sample_rate);
void krad_tone_add(kr_tone *krad_tone, float frequency);
void krad_tone_add_preset(kr_tone *krad_tone, char *preset);
void krad_tone_clear(kr_tone *krad_tone);
void krad_tone_set_volume(kr_tone *krad_tone, int volume);
void krad_tone_remove(kr_tone *krad_tone, float frequency);
void krad_tone_run(kr_tone *krad_tone, float *buffer, int samples);
void krad_tone_destroy(kr_tone *krad_tone);

#endif
