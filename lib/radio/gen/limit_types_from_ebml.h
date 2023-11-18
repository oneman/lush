#if !defined(_RADIO_GEN_LIMIT_TYPES_FROM_EBML_H)
#define _RADIO_GEN_LIMIT_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../limit_types.h"
#include <krad/ebml/ebml.h>
int kr_limit_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_limits_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

