#if !defined(_OPS_MEDIA_GEN_TYPES_TO_TEXT_H)
#define _OPS_MEDIA_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
int kr_media_file_info_to_text(char *text, void *info, int max);
int kr_media_metadata_info_to_text(char *text, void *info, int max);
int kr_media_url_info_to_text(char *text, void *info, int max);
int kr_media_item_info_to_text(char *text, void *info, int max);
int kr_media_info_to_text(char *text, void *info, int max);
#endif

