#if !defined(_mixer_mixer_H)
# define _mixer_mixer_H (1)

#include <krad/audio/audio.h>
#include "types.h"

typedef struct kr_mixer kr_mixer;
typedef struct kr_mixer_path kr_mixer_path;
typedef struct kr_mixer_event kr_mixer_event;
typedef struct kr_mixer_setup kr_mixer_setup;
typedef struct kr_mixer_port_setup kr_mixer_port_setup;

#include "io.h"

typedef void (kr_mixer_event_cb)(kr_mixer_event *);

typedef enum {
  KR_MIXER_CREATE = 1,
  KR_MIXER_METER,
  KR_MIXER_PATCH,
  KR_MIXER_DESTROY
} kr_mixer_event_type;

struct kr_mixer_event {
  kr_mixer_path *path;
  void *user;
  void *user_path;
  kr_mixer_event_type type;
  kr_patchset *patchset;
  kr_mixer_path_info info;
};

struct kr_mixer_setup {
  uint32_t path_count;
  void *user;
  kr_mixer_event_cb *event_cb;
};

int kr_mixer_path_info_get(kr_mixer_path *path, kr_mixer_path_info *info);
int kr_mixer_ctl(kr_mixer_path *path, kr_patchset *patchset);
int kr_mixer_remove(kr_mixer_path *path);
int kr_mixer_bus(kr_mixer *mixer, kr_mixer_path_info *info, void *user);
int kr_mixer_link(kr_mixer_path *output, kr_mixer_path *from,
 kr_mixer_path_info *info, void *user);
int kr_mixer_destroy(kr_mixer *mixer);
kr_mixer *kr_mixer_create(kr_mixer_setup *setup);

#endif
