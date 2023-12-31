#include "text.h"

struct kr_text {
  kr_text_info info;
  int prerendered;
  cairo_font_face_t *cr_face;
  cairo_scaled_font_t* scaled_font;
  FT_Face ft_face;
  FT_Library *ft_library;
};

static void kr_text_set_font(kr_text *text, char *font);
static void kr_text_prerender_cancel(kr_text *text, cairo_t *cr);
static void kr_text_prerender(kr_text *text, cairo_t *cr);

size_t kr_text_sizeof() {
  return sizeof(kr_text);
}

void kr_text_clear(kr_text *text) {
  if (text->cr_face) {
    cairo_font_face_destroy(text->cr_face);
    text->cr_face = NULL;
  }
}

int kr_text_init(kr_text *text, char *str, char *font, FT_Library *ftlib) {
  if (!text || !str || !ftlib) return -1;
  memset(text, 0, sizeof(kr_text));
  text->ft_library = ftlib;
  strncpy(text->info.string, str, (sizeof(text->info.string) - 1));
  text->info.string[(sizeof(text->info.string) - 1)] = '\0';
  kr_text_set_font(text, font);
  text->info.red = 1.0f;
  text->info.green = 1.0f;
  text->info.blue = 1.0f;
  return 0;
}

int kr_text_info_get(kr_text *text, kr_text_info *info) {
  if (!text || !info) return -1;
  *info = text->info;
  return 0;
}

static void kr_text_set_font(kr_text *text, char *font) {
  static const cairo_user_data_key_t key;
  int status;
  if (!font || (strlen(font) <= 0)) {
    strcpy(text->info.font, KRAD_TEXT_DEFAULT_FONT);
    return;
  }
  if (FT_New_Face(*text->ft_library, font, 0, &text->ft_face) == 0) {
    FT_Set_Char_Size(text->ft_face, 0, 50.0, 100, 100 );
    text->cr_face = cairo_ft_font_face_create_for_ft_face(text->ft_face, 0);
    status = cairo_font_face_set_user_data(text->cr_face, &key, text->ft_face,
     (cairo_destroy_func_t)FT_Done_Face);
    if (status) {
       cairo_font_face_destroy(text->cr_face);
       text->cr_face = NULL;
       FT_Done_Face(text->ft_face);
    } else {
      strncpy(text->info.font, font, (sizeof(text->info.font) - 1));
      text->info.font[(sizeof(text->info.font) - 1)] = '\0';
    }
  }
}

void kr_text_prerender_cancel(kr_text *text, cairo_t *cr) {
  if (text->prerendered == 1) {
    cairo_restore(cr);
    text->prerendered = 0;
  }
}

static void kr_text_prerender(kr_text *krad_text, cairo_t *cr) {
  cairo_text_extents_t extents;
  if (krad_text->prerendered == 1) {
    kr_text_prerender_cancel(krad_text, cr);
  }
  cairo_save(cr);
  if (krad_text->cr_face != NULL) {
    cairo_set_font_face(cr, krad_text->cr_face);
  } else {
    cairo_select_font_face(cr, krad_text->info.font, CAIRO_FONT_SLANT_NORMAL,
     CAIRO_FONT_WEIGHT_NORMAL);
  }
/*
  cairo_set_font_size(cr, krad_text->info.input_info.pos.h);
  cairo_set_source_rgba (cr,
                         krad_text->info.red,
                         krad_text->info.green,
                         krad_text->info.blue,
                         krad_text->info.input_info.opacity);
*/
  cairo_text_extents(cr, krad_text->info.string, &extents);
/*
  krad_text->info.input_info.pos.w = extents.width;
  krad_text->info.input_info.pos.h = extents.height;
  cairo_translate (cr, krad_text->info.input_info.pos.x, krad_text->info.input_info.pos.y);
  if (krad_text->info.input_info.rotation != 0.0f) {
    cairo_translate (cr, krad_text->info.input_info.pos.w / 2,
                     krad_text->info.input_info.pos.h / -2);
    cairo_rotate (cr, krad_text->info.input_info.rotation * (M_PI/180.0));
    cairo_translate (cr, krad_text->info.input_info.pos.w / -2,
                     krad_text->info.input_info.pos.h / 2);
  }
  krad_text->prerendered = 1;
  */
}

void kr_text_render(kr_text *text, cairo_t *cr) {
  /*kr_compositor_input_info *input_info;*/
  kr_text_prerender(text, cr);
  cairo_show_text(cr, text->info.string);
  cairo_stroke(cr);
  cairo_restore(cr);
  text->prerendered = 0;
  /*
  input_info = &text->info.input_info;
  printk("rendered %s %s %f %f %f %f %f %d %d %d %d", text->info.string,
   text->info.font, text->info.red, text->info.green, text->info.blue,
   input_info->opacity, input_info->rotation, input_info->pos.x, input_info->pos.y,
   input_info->pos.w, input_info->pos.h);
  */
}
