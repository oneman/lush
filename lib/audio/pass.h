#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#if !defined(_audio_pass_H)
# define _audio_pass_H (1)

#include "audio.h"
#include "biquad.h"
#include "types.h"

typedef struct kr_pass kr_pass;
typedef struct kr_pass kr_lowpass;
typedef struct kr_pass kr_highpass;

size_t kr_pass_sizeof(int nchannels);
kr_pass *kr_pass_init(void *mem, int nchannels, int sample_rate,
 kr_pass_info *info);
int kr_pass_process(kr_pass *pass, kr_audio *out, kr_audio *in,
 kr_pass_info *info);

#endif
