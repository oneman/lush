#if !defined(_PERSPECTIVE_GEN_TYPES_FROM_EBML_H)
#define _PERSPECTIVE_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_pos_unpack_ebml(kr_ebml *ebml, void *info);
int kr_perspective_view_unpack_ebml(kr_ebml *ebml, void *info);
#endif

