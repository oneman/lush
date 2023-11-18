#if !defined(_adapter_sw_jack_jack_H)
# define _adapter_sw_jack_jack_H (1)

#include <jack/jack.h>
#include <krad/adapter/adapter.h>
#include <krad/audio/audio.h>
#include <krad/app/debug.h>
#include "types.h"

int kr_jack_lctl(kr_adapter_path *, kr_patchset *);
int kr_jack_unlink(kr_adapter_path *);
int kr_jack_link(kr_adapter *, kr_adapter_path *);
int kr_jack_ctl(kr_adapter *, kr_patchset *);
int kr_jack_close(kr_adapter *);
int kr_jack_open(kr_adapter *);

#endif
