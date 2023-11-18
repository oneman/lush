#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>
#include <krad/mem/mem.h>
#include <krad/image/image.h>
#include <krad/draw/blur.h>
#include "util/image.c"

#define KR_PI 3.14159265358979323846

typedef struct {
  float r;
  float g;
  float b;
  float a;
} kr_color;

typedef struct {
  int on;
  int width;
  kr_color color; 
} kr_stroke;

typedef struct {
  int on;
  kr_color color; 
} kr_fill;

typedef struct {
  int w;
  int h;
  kr_stroke stroke;
  kr_fill fill;
} kr_shape;

typedef struct {
  int x;
  int y;
} kr_pos;

typedef struct {
  kr_shape shape;
  kr_pos pos;
  float radius;
} kr_rectangle;

typedef struct {
  int handle_pos;
} kr_fader;

static void print_color(kr_color *c, char *label) {
  if (!c) return;
  if (!label) label = "";
  printf("%s: %f,%f,%f,%f\n", label, c->r, c->g, c->b, c->a);
}

static void arc_rectangle(cairo_t *cr, kr_rectangle *r) {
  int x;
  int y;
  int w;
  int h;
  double aspect;
  double corner_radius;
  double radius;
  double degrees;
  if (!cr || !r) return;
  x = r->pos.x;
  y = r->pos.y;
  w = r->shape.w;
  h = r->shape.h;
  if (w > h) {
    aspect = w/h;
  } else {
    aspect = h/w;
  }
  //corner_radius = r->shape.h / r->radius;
  corner_radius = r->radius;
  radius = corner_radius / aspect;
  degrees = KR_PI / 180.0;
  printf("Rectangle: %d,%d %dx%d\n", x, y, w, h);
  //cairo_new_sub_path(cr);
  cairo_new_path(cr);
  cairo_arc(cr, x + w - radius, y + radius, radius, -90 * degrees, 0);
  cairo_arc(cr, x + w - radius, y + h - radius, radius, 0, 90 * degrees);
  cairo_arc(cr, x + radius, y + h - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);
}

static void fill(cairo_t *cr, kr_fill *f) {
  cairo_set_source_rgba(cr, f->color.r, f->color.g, f->color.b, f->color.a);
  cairo_fill_preserve(cr);
  print_color(&f->color, "Fill");
}

static void stroke(cairo_t *cr, kr_stroke *s) {
  cairo_set_source_rgba(cr, s->color.r, s->color.g, s->color.b, s->color.a);
  cairo_set_line_width(cr, s->width);
  cairo_stroke(cr);
  print_color(&s->color, "Stroke");
}

static void render_rectangle(cairo_t *cr, kr_rectangle *r) {
  arc_rectangle(cr, r);
  if (r->shape.fill.on) {
    fill(cr, &r->shape.fill);
  }
  if (r->shape.stroke.on) {
    stroke(cr, &r->shape.stroke);
  }
}

static void render_track(cairo_t *cr, kr_fader *f) {
 kr_rectangle r;
 kr_shape *s;
 s = &r.shape;
 s->w = 180;
 s->h = 600;
 s->stroke.on = 1;
 s->stroke.color.r = 0.5;
 s->stroke.color.g = 0.5;
 s->stroke.color.b = 0.5;
 s->stroke.color.a = 0.2;
 s->stroke.width = 2;
 s->fill.on = 1;
 s->fill.color.r = 0.7;
 s->fill.color.g = 0.68;
 s->fill.color.b = 0.68;
 s->fill.color.a = 1;
 r.pos.x = 220;
 r.pos.y = 40;
 r.radius = 30;
 cairo_save(cr);
 render_rectangle(cr, &r);
 cairo_restore(cr);
 cairo_save(cr);
 cairo_pattern_t *pat;
 //pat = cairo_pattern_create_radial(r.pos.x + s->w/2, r.pos.y + s->h/2, 222.4, r.pos.x + s->w/2, r.pos.y + s->h/2, 3.0);
 int s_len;
 s_len = 10;
 pat = cairo_pattern_create_linear(r.pos.x, r.pos.y, r.pos.x + s_len, r.pos.y);
 cairo_pattern_add_color_stop_rgba(pat, 0, 0, 0, 0, 1);
 cairo_pattern_add_color_stop_rgba(pat, 0.2, 0, 0, 0, 1);
 cairo_pattern_add_color_stop_rgba(pat, 0.6, 0, 0, 0, 0);
 cairo_pattern_add_color_stop_rgba(pat, 1, 0, 0, 0, 0);
 cairo_new_path(cr);
 cairo_set_source(cr, pat);
 arc_rectangle(cr, &r);
 cairo_fill(cr);
 cairo_pattern_destroy(pat);

 pat = cairo_pattern_create_linear(r.pos.x, r.pos.y, r.pos.x, r.pos.y + s_len);
 cairo_pattern_add_color_stop_rgba(pat, 0, 0, 0, 0, 1);
 cairo_pattern_add_color_stop_rgba(pat, 0.2, 0, 0, 0, 1);
 cairo_pattern_add_color_stop_rgba(pat, 0.6, 0, 0, 0, 0);
 cairo_pattern_add_color_stop_rgba(pat, 1, 0, 0, 0, 0);
 cairo_new_path(cr);
 cairo_set_source(cr, pat);
 arc_rectangle(cr, &r);
 cairo_fill(cr);
 cairo_pattern_destroy(pat);

 cairo_restore(cr);
}

static void render_handle(cairo_t *cr, kr_fader *f) {
 kr_rectangle r;
 kr_shape *s;
 s = &r.shape;
 s->w = 160;
 s->h = 70;
 s->stroke.on = 1;
 s->stroke.color.r = 0;
 s->stroke.color.g = 0;
 s->stroke.color.b = 0;
 s->stroke.color.a = 0.46;
 s->stroke.width = 1;
 s->fill.on = 1;
 s->fill.color.r = 0.8;
 s->fill.color.g = 0.9;
 s->fill.color.b = 0.8;
 s->fill.color.a = 1;
 //r.pos.x = 220 + (40/2) - (s->w/2);
 r.pos.x = 214 + 16;
 r.pos.y = 80 + f->handle_pos;
 r.radius = 5;
 cairo_save(cr);
 render_rectangle(cr, &r);
 cairo_restore(cr);
 s->stroke.on = 0;
 r.pos.y += s->h/2;
 s->h = 8;
 r.pos.y -= s->h/2;
 s->fill.color.r = 0.99;
 s->fill.color.g = 0.1;
 s->fill.color.b = 0.1;
 s->fill.color.a = 0.74;
 cairo_save(cr);
 render_rectangle(cr, &r);
 cairo_restore(cr);
}

static void render_fader(cairo_t *cr) {
  kr_fader f;
  f.handle_pos = 50;
  render_track(cr, &f);
  render_handle(cr, &f);
}

static int fader_test(char *filename) {
  int ret;
  cairo_surface_t *cs;
  cairo_t *cr;
  kr_image img;
  kr_image_setup setup;
  img.info.w = 1280;
  img.info.h = 720;
  img.info.fmt = KR_PIXELS_ARGB;
  setup.mem = kr_alloc(kr_image_sizeof(&img.info));
  setup.info = img.info;
  setup.img = &img;
  ret = kr_image_init2(&setup);
  if (ret) return ret;
  cs = cairo_image_surface_create_for_data(img.px[0],
   CAIRO_FORMAT_ARGB32, img.info.w, img.info.h, img.ps[0]);
  cr = cairo_create(cs);
  cairo_set_source_rgb(cr, 0.8, 0.8, 0.7);
  cairo_paint(cr);
  render_fader(cr);
  //ret = kr_image_blur(&img);
  ret = kr_image_to_png_filename(filename, &img);
  cairo_destroy(cr);
  cairo_surface_destroy(cs);
  free(setup.mem);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  char filename[128];
  snprintf(filename, sizeof(filename), "%s/images/fader_test.png",
   getenv("HOME"));
  ret = fader_test(filename);
  if (ret == 0) {
    printf("It worked!\n");
  } else {
    printf("It failed. :|\n");
  }
  return ret;
}
