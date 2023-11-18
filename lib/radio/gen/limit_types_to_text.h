#if !defined(_RADIO_GEN_LIMIT_TYPES_TO_TEXT_H)
#define _RADIO_GEN_LIMIT_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../limit_types.h"
int kr_limit_info_to_text(char *text, void *info, int max);
int kr_limits_info_to_text(char *text, void *info, int max);
#endif

