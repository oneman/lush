#if !defined(_audio_analog_H)
# define _audio_analog_H (1)

#include "audio.h"
#include "types.h"

typedef struct kr_analog kr_analog;

size_t kr_analog_sizeof(int nchannels);
kr_analog *kr_analog_init(void *mem, int nchannels, int sample_rate,
 kr_analog_info *info);
int kr_analog_process(kr_analog *analog, kr_audio *out, kr_audio *in,
 kr_analog_info *info);

#endif
