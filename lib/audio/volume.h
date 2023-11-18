#if !defined(_audio_volume_H)
# define _audio_volume_H (1)

#include "audio.h"
#include "types.h"

typedef struct kr_volume kr_volume;

size_t kr_volume_sizeof(int nchannels);
kr_volume *kr_volume_init(void *mem, int nchannels, int sample_rate,
 kr_volume_info *info);
int kr_volume_process(kr_volume *volume, kr_audio *out, kr_audio *in,
 kr_volume_info *info);

#endif
