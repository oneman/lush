#include "pool.h"


kr_pool *kr_audio_pool_create(kr_audio_pool_setup *setup) {
  kr_pool_setup pool_setup;
  if (!setup) return NULL;
  memset(&pool_setup, 0, sizeof(pool_setup));
  if (setup->channels < 1 || setup->channels > 8) return NULL;
  if (setup->count < 1 || setup->count > 8192) return NULL;
  if (setup->nframes < 1 || setup->nframes > 48000) return NULL;
  pool_setup.size = setup->channels * sizeof(float) * setup->nframes;
  pool_setup.slices = setup->count;
  pool_setup.shared = setup->shared;
  return kr_pool_create(&pool_setup);
}
