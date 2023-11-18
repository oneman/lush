#if !defined(_CONTAINER_MPEG_TS_GEN_TYPES_TO_TEXT_H)
#define _CONTAINER_MPEG_TS_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_mts_packet_info_to_text(char *text, void *info, int max);
int kr_mts_stream_info_to_text(char *text, void *info, int max);
int kr_mts_program_info_to_text(char *text, void *info, int max);
int kr_mts_info_to_text(char *text, void *info, int max);
#endif

