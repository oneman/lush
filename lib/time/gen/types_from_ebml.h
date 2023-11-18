#if !defined(_TIME_GEN_TYPES_FROM_EBML_H)
#define _TIME_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_timestamp_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

