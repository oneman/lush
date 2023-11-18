#if !defined(_PLAYLIST_GEN_TYPES_TO_TEXT_H)
#define _PLAYLIST_GEN_TYPES_TO_TEXT_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../m3u_types.h"
int kr_m3u_tag_prim_info_to_text(char *text, void *info, int max);
int kr_m3u_tag_alist_info_to_text(char *text, void *info, int max);
int m3u_tag_info_to_text(char *text, void *info, int max);
int m3u_el_info_to_text(char *text, void *info, int max);
int kr_m3u_info_to_text(char *text, void *info, int max);
#endif

