#if !defined(_draw_text_H)
# define _draw_text_H (1)

#include <ft2build.h>
#include FT_FREETYPE_H
#include <cairo-ft.h>

#define KRAD_TEXT_DEFAULT_FONT "sans"

#include "text_types.h"

typedef struct kr_text kr_text;

size_t kr_text_sizeof();
void kr_text_clear(kr_text *text);
int kr_text_init(kr_text *text, char *str, char *font, FT_Library *ftlib);
int kr_text_info_get(kr_text *text, kr_text_info *info);
void kr_text_render(kr_text *text, cairo_t *cr);

#endif
