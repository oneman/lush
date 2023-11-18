#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <xcb/xcb.h>

int main (int argc, char **argv) {
  xcb_connection_t *c;
  xcb_screen_t *s;
  xcb_window_t w;
  xcb_gcontext_t g;
  xcb_generic_event_t *e;
  uint32_t mask;
  uint32_t values[2];
  int done;
  int i;
  xcb_rectangle_t r = { 60, 60, 320, 180 };
  c = xcb_connect(NULL, NULL);
  if (!c) {
    printf("Cannot open display\n");
    exit(1);
  }
  s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;
  /* create window */
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = s->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
  w = xcb_generate_id(c);
  xcb_create_window(c, XCB_COPY_FROM_PARENT, w, s->root, 10, 10, 640, 360, 1,
   XCB_WINDOW_CLASS_INPUT_OUTPUT, s->root_visual, mask, values);
  /* create black graphics context */
  mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  values[0] = s->black_pixel;
  values[1] = 0;
  g = xcb_generate_id(c);
  xcb_create_gc(c, g, w, mask, values);
  xcb_map_window(c, w);
  xcb_flush(c);
  done = 0;
  i = 0;
  while (!done && (e = xcb_wait_for_event(c))) {
    switch (e->response_type) {
      case XCB_EXPOSE:
        printf("EXPOSE %d \r", i++);
        fflush(stdout);
        xcb_poly_fill_rectangle(c, w, g, 1, &r);
        xcb_flush(c);
        break;
      case XCB_KEY_PRESS:
        done = 1;
        break;
    }
    free(e);
  }
  xcb_disconnect(c);
  return 0;
}
