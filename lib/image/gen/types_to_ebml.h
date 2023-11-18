#if !defined(_IMAGE_GEN_TYPES_TO_EBML_H)
#define _IMAGE_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_rect_pack_ebml(kr_ebml *ebml, void *info);
int kr_image_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_coded_image_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

