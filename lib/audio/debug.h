#if !defined(_audio_debug_H)
# define _audio_debug_H (1)

#include <krad/app/debug.h>
#include "types.h"
#include "audio.h"

void kr_audio_debug(kr_audio *a);
void kr_coded_audio_debug(kr_coded_audio *a);

#endif
