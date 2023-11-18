#if !defined(_DRAW_GEN_VECTOR_TYPES_TO_EBML_H)
#define _DRAW_GEN_VECTOR_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../vector_types.h"
#include <krad/ebml/ebml.h>
int kr_vector_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

