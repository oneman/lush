#if !defined(_ADAPTER_HW_ALSA_GEN_TYPES_TO_TEXT_H)
#define _ADAPTER_HW_ALSA_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_alsa_info_to_text(char *text, void *info, int max);
int kr_alsa_path_info_to_text(char *text, void *info, int max);
#endif

