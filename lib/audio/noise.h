#if !defined(_audio_noise_H)
# define _audio_noise_H (1)

#include <krad/app/debug.h>
#include "audio.h"

typedef struct kr_noise kr_noise;

size_t kr_noise_sizeof(void);
int kr_noise_init(kr_noise *n);
float kr_noise_gen(kr_noise *n);
int kr_noise_fill(kr_noise *n, kr_audio *a);
int kr_noise_add(kr_noise *n, kr_audio *a);

#endif
