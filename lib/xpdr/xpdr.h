#if !defined(_xpdr_xpdr_H)
# define _xpdr_xpdr_H (1)

typedef struct kr_xpdr kr_xpdr;
typedef struct kr_xpdr_setup kr_xpdr_setup;
typedef struct kr_xpdr_path kr_xpdr_path;
typedef struct kr_xpdr_event kr_xpdr_event;
typedef void (kr_xpdr_event_cb)(kr_xpdr_event *);

#include <krad/loop/loop.h>
#include <krad/audio/audio.h>
#include <krad/image/image.h>
#include <krad/mixer/mixer.h>
#include <krad/compositor/compositor.h>
#include <krad/xpdr/types.h>

typedef enum {
  KR_XPDR_CREATE = 1,
  KR_XPDR_PATCH,
  KR_XPDR_DESTROY
} kr_xpdr_event_type;

struct kr_xpdr_event {
  kr_xpdr_path *path;
  void *user;
  void *user_path;
  kr_xpdr_event_type type;
  kr_patchset *patchset;
  kr_xpdr_path_info info;
};

typedef int (kr_fds_get_cb)(void *user, int *fds, int n);

struct kr_xpdr_setup {
  kr_mixer *mixer;
  kr_compositor *compositor;
  kr_xpdr_event_cb *event_cb;
  kr_fds_get_cb *fds_get_cb;
  void *user;
  int path_count;
  kr_loop *master_loop;
};

int kr_xpdr_path_info_get(kr_xpdr_path *path, kr_xpdr_path_info *info);
int kr_xpdr_ctl(kr_xpdr_path *path, kr_patchset *patchset);
int kr_xpdr_remove(kr_xpdr_path *path);
int kr_xpdr_link(kr_xpdr_path *path, kr_xpdr_path_info *info, void *user,
 void *peer_user);
int kr_xpdr_open(kr_xpdr *xpdr, kr_xpdr_path_info *info, void *user);
int kr_xpdr_monitor(kr_xpdr *xpdr, int on);
int kr_xpdr_destroy(kr_xpdr *xpdr);
kr_xpdr *kr_xpdr_create(kr_xpdr_setup *setup);

#endif
