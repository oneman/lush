#if !defined(_xpdr_client_H)
# define _xpdr_client_H (1)

#include "types.h"
#include <krad/adapter/sw/aux/client.h>

int kr_xpdr_make(kr_client *client, char *name, kr_xpdr_path_info *info);

#endif
