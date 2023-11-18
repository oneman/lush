#if !defined(_CONTAINER_MPEG_TS_GEN_TYPES_FROM_EBML_H)
#define _CONTAINER_MPEG_TS_GEN_TYPES_FROM_EBML_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/ebml/ebml.h>
int kr_mts_packet_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_mts_stream_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_mts_program_info_unpack_ebml(kr_ebml *ebml, void *info);
int kr_mts_info_unpack_ebml(kr_ebml *ebml, void *info);
#endif

