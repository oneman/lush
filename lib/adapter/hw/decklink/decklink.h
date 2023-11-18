#if !defined(_adapter_hw_decklink_decklink_H)
# define _adapter_hw_decklink_decklink_H (1)

#include <krad/adapter/adapter.h>
#include "types.h"

#define DEFAULT_DECKLINK_DEVICE "0"

int kr_decklink_lctl(kr_adapter_path *, kr_patchset *);
int kr_decklink_unlink(kr_adapter_path *);
int kr_decklink_link(kr_adapter *, kr_adapter_path *);
int kr_decklink_ctl(kr_adapter *, kr_patchset *);
int kr_decklink_close(kr_adapter *);
int kr_decklink_open(kr_adapter *);

#endif
