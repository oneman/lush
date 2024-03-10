#if !defined(_compositor_client_H)
# define _compositor_client_H (1)

#include <krad/radio/client.h>
#include <krad/app/debug.h>
#include "types.h"

int kr_compositor_make(kr_client *client, char *name, kr_compositor_path_info *info);

#endif
