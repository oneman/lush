#if !defined(_playlist_m3u_H)
# define _playlist_m3u_H (1)

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include "m3u_types.h"

int kr_m3u_parse(kr_m3u_info *info, char *buffer, size_t sz);

#endif
