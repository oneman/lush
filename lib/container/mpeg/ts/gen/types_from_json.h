#if !defined(_CONTAINER_MPEG_TS_GEN_TYPES_FROM_JSON_H)
#define _CONTAINER_MPEG_TS_GEN_TYPES_FROM_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_mts_packet_info_unpack_json(char *json, void *info);
int kr_mts_stream_info_unpack_json(char *json, void *info);
int kr_mts_program_info_unpack_json(char *json, void *info);
int kr_mts_info_unpack_json(char *json, void *info);
#endif

