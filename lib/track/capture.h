#include "types.h"

#if !defined(_track_capture_H)
# define _track_capture_H (1)

typedef struct {
  kr_track *track;
  kr_track_head *head;
  union {
    kr_compositor_path *comp_output;
    kr_mixer_path *mixer_output;
  };
  kr_pool *raw_pool;
} studio_capture;

int studio_capture_destroy(kr_studio_path *path);
int studio_capture_create(kr_studio_path *path);

#endif
