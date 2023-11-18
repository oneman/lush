#include <inttypes.h>
#include <krad/audio/audio.h>

#if !defined(_audio_resample_H)
# define _audio_resample_H (1)

typedef struct kr_resampler kr_resampler;
typedef struct kr_resampler_setup kr_resampler_setup;

struct kr_resampler_setup {
  int64_t speed;
  int channels;
  void *mem;
};

size_t kr_resampler_sizeof();
int kr_resampler_reset(kr_resampler *resampler);
/*int kr_resampler_set_speed(kr_resampler *resampler, int64_t speed);*/
int kr_resampler_process(kr_resampler *resampler, kr_audio *out, kr_audio *in);
int kr_resampler_destroy(kr_resampler *resampler);
kr_resampler *kr_resampler_init(kr_resampler_setup *setup);

#endif
