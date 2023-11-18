#if !defined(_track_studio_H)
# define _track_studio_H (1)

#include <krad/xfer/xfer.h>
#include <krad/mixer/mixer.h>
#include <krad/compositor/compositor.h>
#include <krad/track/tracker.h>
#include <krad/image/stash.h>

typedef struct kr_studio kr_studio;
typedef struct kr_studio_path kr_studio_path;
typedef struct kr_studio_event kr_studio_event;
typedef int (kr_studio_event_cb)(kr_studio_event *);

typedef enum {
  KR_STUDIO_ADD = 1,
  KR_STUDIO_REMOVE,
  KR_STUDIO_PATCH
} kr_studio_event_type;

struct kr_studio_event {
  void *user;
  void *user_path;
  kr_patchset *patchset;
  kr_studio_path *path;
  kr_studio_event_type type;
  kr_studio_path_info info;
};

typedef struct {
  uint32_t npaths;
  uint32_t nprocs;
  void *user;
  kr_studio_event_cb *event_cb;
  kr_xfer *xfer;
  kr_compositor *compositor;
  kr_mixer *mixer;
  kr_image_stash *stash;
  kr_tracker *tracker;
} kr_studio_setup;

int kr_studio_source_destroy(kr_studio_path *path);
int kr_studio_source_create(kr_studio *studio,
 kr_studio_path_info *info, void *user);
ssize_t kr_studio_source_read(kr_studio_path *path,
 uint8_t *buf, size_t len);
int kr_studio_source_decode_image(kr_studio_path *path);
int kr_studio_path_info_get(kr_studio_path *path, kr_studio_path_info *info);
int kr_studio_ctl(kr_studio_path *path, kr_patchset *patchset);
int kr_studio_remove(kr_studio_path *path);
int kr_studio_connect(kr_studio_path *p, kr_studio_path_info *i, void *user);
int kr_studio_add(kr_studio *studio, kr_studio_path_info *info, void *user);
int kr_studio_destroy(kr_studio *studio);
kr_studio *kr_studio_create(kr_studio_setup *setup);

#endif
