#if !defined(_VIDEO_GEN_TYPES_FROM_EBML_H)
#define _VIDEO_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_fps_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

