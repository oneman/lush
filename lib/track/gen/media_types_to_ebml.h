#if !defined(_TRACK_GEN_MEDIA_TYPES_TO_EBML_H)
#define _TRACK_GEN_MEDIA_TYPES_TO_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../media_types.h"
#include <krad/ebml/ebml.h>
int kr_media_source_info_pack_ebml(kr_ebml *ebml, void *info);
int kr_media_packet_pack_ebml(kr_ebml *ebml, void *info);
#endif

