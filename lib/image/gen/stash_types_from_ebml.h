#if !defined(_IMAGE_GEN_STASH_TYPES_FROM_EBML_H)
#define _IMAGE_GEN_STASH_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../stash_types.h"
#include <krad/ebml/ebml.h>
int kr_image_stash_item_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

