#if !defined(_audio_pool_H)
# define _audio_pool_H (1)

#include <krad/mem/pool.h>
#include <krad/audio/audio.h>

typedef struct {
  int channels;
  int nframes;
  size_t count;
  int shared;
} kr_audio_pool_setup;

kr_pool *kr_audio_pool_create(kr_audio_pool_setup *setup);

#endif
