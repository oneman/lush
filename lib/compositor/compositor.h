#if !defined(_compositor_compositor_H)
# define _compositor_compositor_H (1)

typedef struct kr_compositor kr_compositor;
typedef struct kr_compositor_setup kr_compositor_setup;
typedef struct kr_compositor_event kr_compositor_event;
typedef void (kr_compositor_event_cb)(kr_compositor_event *);

#include "types.h"
#include "path.h"
#include "io.h"

#define KR_COMP_PATHS_MAX 32
#define KR_COMP_TEXTS_MAX 32
#define KR_COMP_VECTORS_MAX 32

struct kr_compositor_setup {
  void *user;
  kr_compositor_event_cb *event_cb;
};

typedef enum {
  KR_COM_CREATE = 1,
  KR_COM_PATCH,
  KR_COM_DESTROY
} kr_compositor_event_type;

struct kr_compositor_event {
  kr_compositor_path *path;
  void *user;
  void *user_path;
  kr_compositor_event_type type;
  kr_patchset *patchset;
  kr_compositor_path_info info;
};

int kr_compositor_destroy(kr_compositor *compositor);
kr_compositor *kr_compositor_create(kr_compositor_setup *setup);

#endif
