#if !defined(_FEAT_GEN_TYPES_TO_EBML_H)
#define _FEAT_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_feature_aspect_pack_ebml(kr_ebml *ebml, void *info);
int kr_feature_pack_ebml(kr_ebml *ebml, void *info);
#endif

