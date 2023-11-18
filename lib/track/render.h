#include "types.h"

#if !defined(_track_render_H)
# define _track_render_H (1)

#define KR_IMAGE_RENDER_FRAMES 2

typedef struct {
  union {
    struct {
      kr_compositor_path *com_port;
    } image;
    struct {
      kr_compositor_path *com_port;
    } video;
    struct {
      kr_mixer_path *mix_port;
    } audio;
  };
} studio_render;

int studio_render_destroy(kr_studio_path *path);
int studio_render_create(kr_studio_path *path);

#endif
