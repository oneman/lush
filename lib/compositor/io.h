#if !defined(_compositor_io_H)
# define _compositor_io_H (1)

#include "compositor.h"

typedef struct {
  kr_compositor_path_info info;
  void *frame_user;
  void *control_user;
} kr_compositor_port_setup;

kr_compositor_path *kr_compositor_port(kr_compositor *comp,
 kr_compositor_port_setup *setup);

int kr_compositor_port_fd(kr_compositor_path *path);
int kr_compositor_read(kr_compositor_path *path, kr_frame *frame);
int kr_compositor_write(kr_compositor_path *path, kr_frame *frame);

#endif
