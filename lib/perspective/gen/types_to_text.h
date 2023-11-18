#if !defined(_PERSPECTIVE_GEN_TYPES_TO_TEXT_H)
#define _PERSPECTIVE_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_pos_to_text(char *text, void *info, int max);
int kr_perspective_view_to_text(char *text, void *info, int max);
#endif

