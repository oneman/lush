#if !defined(_mixer_client_H)
# define _mixer_client_H (1)

#include <krad/radio/client.h>
#include <krad/app/debug.h>
#include "types.h"

int kr_mixer_make(kr_client *client, char *name, kr_mixer_path_info *info);

#endif
