#if !defined(_ADAPTER_SW_WAYLAND_GEN_TYPES_FROM_EBML_H)
#define _ADAPTER_SW_WAYLAND_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_wayland_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_wayland_path_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

