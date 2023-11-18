#if !defined(_mixer_io_H)
# define _mixer_io_H (1)

#include "mixer.h"

typedef int (kr_mixer_io_cb)(int nframes, void *user);

struct kr_mixer_port_setup {
  kr_mixer_path_info info;
  void *control_user;
  void *audio_user;
  int driver;
  kr_mixer_io_cb *process;
};

int kr_mixer_port_setup_init(kr_mixer_port_setup *setup);
kr_mixer_path *kr_mixer_port(kr_mixer *mixer, kr_mixer_port_setup *setup);
int kr_mixer_port_fd(kr_mixer_path *path);
int kr_mixer_read(kr_mixer_path *path, kr_audio *audio);
int kr_mixer_write(kr_mixer_path *path, kr_audio *audio);

#endif
