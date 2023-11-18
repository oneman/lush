#if !defined(_audio_signal_H)
# define _audio_signal_H (1)

#include "audio.h"

typedef struct kr_signal_gen kr_signal_gen;

typedef enum {
  KR_SIGNAL_GEN_DTMF = 1,
  KR_SIGNAL_GEN_MORSE 
} kr_signal_type;

typedef struct {
  kr_signal_type type;
  char str[48];
  size_t len;
} kr_signal;

typedef struct {
  struct {
    int mark_duration;
    int space_duration;
  } dtmf;
  struct {
    int dit_duration;
  } morse;
} kr_signal_info;

typedef struct {
  uint8_t *mem;
  int sample_rate;
  kr_signal_info info;
} kr_signal_gen_setup;

size_t kr_signal_gen_sizeof();
int kr_signal_gen_setup_init(kr_signal_gen_setup *setup);
kr_signal_gen *kr_signal_gen_init(kr_signal_gen_setup *setup);
int kr_signal_gen_ctl(kr_signal_gen *g, kr_signal_info *info);
int kr_signal_gen_put(kr_signal_gen *g, kr_signal *s);
int kr_signal_emit(kr_signal_gen *g, kr_audio *audio);

#endif
