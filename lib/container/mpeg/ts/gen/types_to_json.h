#if !defined(_CONTAINER_MPEG_TS_GEN_TYPES_TO_JSON_H)
#define _CONTAINER_MPEG_TS_GEN_TYPES_TO_JSON_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_mts_packet_info_pack_json(char *json, void *info, int max);
int kr_mts_stream_info_pack_json(char *json, void *info, int max);
int kr_mts_program_info_pack_json(char *json, void *info, int max);
int kr_mts_info_pack_json(char *json, void *info, int max);
#endif

