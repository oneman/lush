#include <krad/adapter/adapter.h>
#include "types.h"

#ifndef KRAD_WAYLAND_H
#define KRAD_WAYLAND_H

int kr_wl_lctl(kr_adapter_path *, kr_patchset *);
int kr_wl_unlink(kr_adapter_path *);
int kr_wl_link(kr_adapter *, kr_adapter_path *);
int kr_wl_ctl(kr_adapter *, kr_patchset *);
int kr_wl_close(kr_adapter *);
int kr_wl_open(kr_adapter *);

#endif
