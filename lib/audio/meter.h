#if !defined(_audio_meter_H)
# define _audio_meter_H (1)

#include <krad/app/debug.h>
#include "audio.h"

typedef struct kr_meter kr_meter;

size_t kr_meter_sizeof(int nc);
kr_meter *kr_meter_init(void *mem, int nchannels);
int kr_meter_process(kr_meter *m, kr_audio *a, kr_meter_info *info);

#endif
