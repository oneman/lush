#if !defined(_VIDEO_GEN_TYPES_TO_EBML_H)
#define _VIDEO_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_fps_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

