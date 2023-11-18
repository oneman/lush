#if !defined(_adapter_sw_aux_aux_H)
# define _adapter_sw_aux_aux_H (1)

#include <krad/app/debug.h>
#include <krad/adapter/adapter.h>
#include "types.h"

int kr_aux_lctl(kr_adapter_path *, kr_patchset *);
int kr_aux_unlink(kr_adapter_path *);
int kr_aux_link(kr_adapter *, kr_adapter_path *);
int kr_aux_ctl(kr_adapter *, kr_patchset *);
int kr_aux_adapter_close(kr_adapter *);
int kr_aux_open(kr_adapter *);

#endif
