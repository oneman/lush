#include <inttypes.h>
#include <sys/types.h>

#if !defined(_audio_convert_H)
# define _audio_convert_H (1)

#include "audio.h"

int16_t kr_audio_float_to_16bit(float samp);
int32_t kr_audio_float_to_24bit(float samp);
float kr_audio_16bit_to_float(int samp);
float kr_audio_24bit_to_float(int samp);
int kr_audio_interleave(float *out, kr_audio *a);
int kr_audio_deinterleave(kr_audio *out, float *input);
int kr_audio_deinterleave_16bit(kr_audio *out, int16_t *input);
int kr_audio_interleave_16bit(int16_t *out, kr_audio *a);
int kr_audio_interleave_16bit_in_32bit(int32_t *out, kr_audio *a);
int kr_audio_interleave_24bit(int32_t *out, kr_audio *a);

#endif
