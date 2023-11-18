#if !defined(_ADAPTER_HW_DECKLINK_GEN_TYPES_TO_EBML_H)
#define _ADAPTER_HW_DECKLINK_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_decklink_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_decklink_path_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

