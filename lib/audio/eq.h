#if !defined(_audio_eq_H)
# define _audio_eq_H (1)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#include <krad/app/debug.h>
#include "types.h"
#include "audio.h"
#include "biquad.h"

typedef struct kr_eq kr_eq;

size_t kr_eq_sizeof(int nchannels);
kr_eq *kr_eq_init(void *mem, int nchannels, int sample_rate, kr_eq_info *info);
int kr_eq_process(kr_eq *eq, kr_audio *out, kr_audio *in, kr_eq_info *info);

#endif
