#if !defined(_COMPOSITOR_GEN_TYPES_FROM_JSON_H)
#define _COMPOSITOR_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_compositor_output_info_unpack_json(char *json, void *info);
int kr_compositor_bus_info_unpack_json(char *json, void *info);
int kr_compositor_input_info_unpack_json(char *json, void *info);
int kr_compositor_source_info_unpack_json(char *json, void *info);
int kr_compositor_overlay_info_unpack_json(char *json, void *info);
int kr_compositor_path_info_unpack_json(char *json, void *info);
#endif

