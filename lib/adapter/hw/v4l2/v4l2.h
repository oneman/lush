#if !defined(_adapter_hw_v4l2_v4l2_H)
# define _adapter_hw_v4l2_v4l2_H (1)

#include <krad/adapter/adapter.h>
#include "types.h"

int kr_v4l2_lctl(kr_adapter_path *, kr_patchset *);
int kr_v4l2_unlink(kr_adapter_path *);
int kr_v4l2_link(kr_adapter *, kr_adapter_path *);
int kr_v4l2_ctl(kr_adapter *, kr_patchset *);
int kr_v4l2_close(kr_adapter *);
int kr_v4l2_open(kr_adapter *);

#endif
