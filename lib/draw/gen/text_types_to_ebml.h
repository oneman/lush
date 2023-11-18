#if !defined(_DRAW_GEN_TEXT_TYPES_TO_EBML_H)
#define _DRAW_GEN_TEXT_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../text_types.h"
#include <krad/ebml/ebml.h>
int kr_text_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

