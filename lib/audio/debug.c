#include "debug.h"

void kr_audio_debug(kr_audio *a) {
  printk("Audio: %zu - %d", a->stc, a->count);
}

void kr_coded_audio_debug(kr_coded_audio *a) {
  printk("Coded Audio: %zu - %d - %zu bytes", a->tc, a->frames, a->len);
}
