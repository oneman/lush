#if !defined(_compositor_path_H)
# define _compositor_path_H (1)

#include <krad/xpdr/event.h>
#include <krad/image/convert.h>
#include <krad/image/crop.h>
#include <krad/image/insert.h>
#include <krad/perspective/perspective.h>

typedef struct kr_compositor_path kr_compositor_path;

int kr_compositor_ctl(kr_compositor_path *path, kr_patchset *patchset);
int kr_compositor_remove(kr_compositor_path *path);
int kr_compositor_bus(kr_compositor *c, kr_compositor_path_info *i, void *user);
int kr_compositor_link(kr_compositor_path *output, kr_compositor_path *from,
  kr_compositor_path_info *info, void *user);
int kr_compositor_path_info_get(kr_compositor_path *path,
 kr_compositor_path_info *info);
size_t kr_compositor_path_size();

#endif
