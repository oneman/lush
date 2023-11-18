#if !defined(_IMAGE_GEN_TYPES_FROM_EBML_H)
#define _IMAGE_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_rect_unpack_ebml(kr_ebml *ebml, void *info);
int kr_image_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_coded_image_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

