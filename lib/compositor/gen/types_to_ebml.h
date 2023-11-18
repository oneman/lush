#if !defined(_COMPOSITOR_GEN_TYPES_TO_EBML_H)
#define _COMPOSITOR_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_compositor_output_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_compositor_bus_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_compositor_input_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_compositor_source_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_compositor_overlay_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_compositor_path_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

