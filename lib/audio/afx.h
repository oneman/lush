#if !defined(_audio_afx_H)
# define _audio_afx_H (1)

#include <krad/app/debug.h>
#include "types.h"
#include "volume.h"
#include "eq.h"
#include "pass.h"
#include "analog.h"
#include "meter.h"

typedef struct kr_afx kr_afx;

size_t kr_afx_sizeof(int channels);
kr_afx *kr_afx_init(void *mem, int nchannels, int sample_rate,
 kr_afx_info *info);
int kr_afx_process(kr_afx *afx, kr_audio *out, kr_audio *in,
 kr_afx_info *info);

#endif
