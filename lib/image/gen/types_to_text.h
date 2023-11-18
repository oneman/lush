#if !defined(_IMAGE_GEN_TYPES_TO_TEXT_H)
#define _IMAGE_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_rect_to_text(char *text, void *info, int max);
int kr_image_info_to_text(char *text, void *info, int max);
int kr_coded_image_info_to_text(char *text, void *info, int max);
#endif

