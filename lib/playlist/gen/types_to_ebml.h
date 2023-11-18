#if !defined(_PLAYLIST_GEN_TYPES_TO_EBML_H)
#define _PLAYLIST_GEN_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../m3u_types.h"
#include <krad/ebml/ebml.h>
int kr_m3u_tag_prim_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_m3u_tag_alist_info_pack_ebml(kr_ebml *ebml, void *info);
int m3u_tag_info_pack_ebml(kr_ebml *ebml, void *info);
int m3u_el_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_m3u_info_pack_ebml(kr_ebml *ebml, void *info);
#endif

