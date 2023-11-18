#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#if !defined(_audio_onepole_H)
# define _audio_onepole_H (1)

#include "audio.h"
#include "biquad.h"
#include "types.h"

typedef struct kr_onepole kr_onepole;

int kr_onepole_set_fc(kr_onepole *p, double fc);
size_t kr_onepole_sizeof();
float kr_onepole_get_val(kr_onepole *p);
float kr_onepole_process(kr_onepole *p, float in);
int kr_onepole_init(uint8_t *mem, float rate);

#endif
