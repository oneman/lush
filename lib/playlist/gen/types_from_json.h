#if !defined(_PLAYLIST_GEN_TYPES_FROM_JSON_H)
#define _PLAYLIST_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../m3u_types.h"
int kr_m3u_tag_prim_info_unpack_json(char *json, void *info);
int kr_m3u_tag_alist_info_unpack_json(char *json, void *info);
int m3u_tag_info_unpack_json(char *json, void *info);
int m3u_el_info_unpack_json(char *json, void *info);
int kr_m3u_info_unpack_json(char *json, void *info);
#endif

