#if !defined(_container_wave_pack_H)
# define _container_wave_pack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include "common.h"

ssize_t kr_wave_pack_header(uint8_t *hdr, size_t sz, kr_wave_info *info);

#endif
