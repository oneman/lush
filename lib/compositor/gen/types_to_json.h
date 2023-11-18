#if !defined(_COMPOSITOR_GEN_TYPES_TO_JSON_H)
#define _COMPOSITOR_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_compositor_output_info_pack_json(char *json, void *info, int max);
int kr_compositor_bus_info_pack_json(char *json, void *info, int max);
int kr_compositor_input_info_pack_json(char *json, void *info, int max);
int kr_compositor_source_info_pack_json(char *json, void *info, int max);
int kr_compositor_overlay_info_pack_json(char *json, void *info, int max);
int kr_compositor_path_info_pack_json(char *json, void *info, int max);
#endif

