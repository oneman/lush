#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

/*
*  gcc cairo_sandbox.c -o cairo_sandbox `pkg-config --libs --cflags cairo`
*/

#define letterpaper_quarter_inch_ptx 18
#define letterpaper_half_inch_ptx 36
#define letterpaper_inch_ptx 72
#define letterpaper_height_ptx 792
#define letterpaper_width_ptx 612
#define letterpaper_gov_margin 72
#define letterpaper_nonmarginal_width 468 /* perm sym */
#define letterpaper_nonmarginal_height 648 /* perm sym */
/* 648 / 468 = 1.384615 */
#define letterpaper_nonmarginal_quad_ruled_columns 26
#define letterpaper_nonmarginal_quad_ruled_rows 36
/* 36 / 26 = 1.384615 */

/* content area: 6.5 * 9 = 58.5 in 484704 */
/* letterpaper area: 58.5 + 35 = 93.5 in */
/* letterpaper total quanta = 93.5 * 5184 */
/* margin area: (11 * 2) + (13 * 1) = 35in */
/* margin quanta: 5184 * 35 = 181440 */
/* margin overlapping quanta: 5184 * 4 = 20736 */
/* margin nonoverlapping quanta: 160704 */
/* margin nonoverlapping wide quanta: 67392 */
/* margin nonoverlapping tall quanta: 93312 */
/* margin nonoverlapping ratio: 93312/67392 = 1.384615 */

/* dollar size: 6.14 in 2.61 in (6.2 * 2.6) */



int main(int argc, char *argv[]) {
  cairo_surface_t *surface;
  cairo_t *cr;
  surface = NULL;
  cr = NULL;
  char filename[128];
  if (argc > 1) {
    snprintf(filename, sizeof(filename),
    "%s/rad_letter_paper.pdf",
    getenv("HOME"));
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 612, 792);
  } else {
    snprintf(filename, sizeof(filename),
    "%s/rad_letter_paper.png",
    getenv("HOME"));
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 612, 792);
  }
  cr = cairo_create(surface);

  /* white paper */
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  cairo_set_line_width(cr, 0.5);
  cairo_set_source_rgba(cr, 0.26, 0.26, 0.99, 1);

  // quad rule

  int c = 1;
  int r = 1;
  for (c = 1; c < 34; c++) {
    cairo_move_to(cr, c * 18, 0);
    cairo_line_to(cr, c * 18, 792);
    cairo_stroke(cr);
  }
  for (r = 1; r < 44; r++) {
    cairo_move_to(cr, 0, r * 18);
    cairo_line_to(cr, 612, r * 18);
    cairo_stroke(cr);
  }

  /* golden margin */
  cairo_set_line_width(cr, 2);
  cairo_set_source_rgba(cr, 1.0, 0.83, 0, 1);
  cairo_move_to(cr, 72, 72);
  cairo_line_to(cr, 540, 72);
  cairo_line_to(cr, 540, 720);
  cairo_line_to(cr, 72, 720);
  cairo_close_path(cr);
  cairo_stroke(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1.0, 0.84, 0.13, 0.71);
  cairo_rectangle(cr, 72, 72, 18 * 26, 18 * 26);
  cairo_fill(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1.0, 0.13, 0.66, 0.2);
  cairo_rectangle(cr, 72, 18 * 30, 18 * 16, 18 * 10);
  cairo_fill(cr);

  cairo_surface_write_to_png(surface, filename);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  return 0;
}
