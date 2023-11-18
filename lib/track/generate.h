#include "types.h"

#if !defined(_track_generate_H)
# define _track_generate_H (1)

#include <krad/audio/noise.h>

#define KR_VIDEO_GEN_FRAMES 2

typedef struct {
  union {
    struct {
      kr_compositor_path *source_port;
      kr_frame frame[KR_VIDEO_GEN_FRAMES];
      uint8_t *mem[KR_VIDEO_GEN_FRAMES];
    } video;
    struct {
      kr_mixer_path *source_port;
      kr_noise *noise;
      kr_amem(mem, 32)
    } audio;
  };
} studio_generator;

int studio_generator_destroy(kr_studio_path *path);
int studio_generator_create(kr_studio_path *path);

#endif
