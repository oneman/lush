#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <krad/app/debug.h>
#include <krad/image/coder.h>
#include <krad/image/crop.h>
#include <krad/compositor/compositor.h>
#include "../data.h"
#include "../util/image.h"

#define NEVENTS 1
#define PORTS_MAX 16

#define OUTPUT_W 1600
#define OUTPUT_H 900

static int current_name = 0;

typedef struct comp_test comp_test;

typedef struct {
  char name[64];
  kr_compositor_path *path;
  kr_compositor_path_info info;
  kr_frame frame;
  comp_test *ct;
} comp_port;

struct comp_test {
  kr_image *result;
  kr_compositor *comp;
  comp_port ports[PORTS_MAX];
  char name[64];
  int pd;
};

#if KR_USE_GTK
#include <cairo.h>
#include <gtk/gtk.h>

static void do_drawing(gpointer *user, cairo_t *cr) {
  comp_test *ct;
  cairo_surface_t *cst;
  ct = (comp_test *)user;
  cst = cairo_image_surface_create_for_data(ct->result->px[0],
   CAIRO_FORMAT_ARGB32, ct->result->info.w, ct->result->info.h,
   ct->result->ps[0]);
  if (cairo_surface_status(cst)) return;
  cairo_set_source_surface(cr, cst, 0, 0);
  cairo_paint(cr);
  cairo_surface_destroy(cst);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user) {
  do_drawing(user, cr);
  return FALSE;
}

int gtk_window(comp_test *ct) {
  GtkWidget *window;
  GtkWidget *darea;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), ct);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), ct->result->info.w,
   ct->result->info.h);
  gtk_window_set_title(GTK_WINDOW(window), "Krad Radio");
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}

#endif

static int current_image = 0;

static const char *images[] = {
  "images/png/Screenshot_2014-03-08-18-46-50.png",
  "images/png/Nokia-Monitor-Test_1.png",
  "images/png/rgb_2048_1.png",
  "images/jpg/ColorGradients.jpg",
  "images/jpg/03_color4test.jpg",
  "images/png/PNG-2048x1536-Test-Pattern.PNG"
};

static comp_port *get_port(comp_test *ct) {
  int i;
  for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].path == NULL) {
      ct->ports[i].ct = ct;
      return &ct->ports[i];
    }
  }
  return NULL;
}

static void compositor_event(kr_compositor_event *event) {
  int i;
  comp_test *ct;
  ct = (comp_test *)event->user;
  comp_port *port;
  if (ct == NULL) {
    printke("CompTest: event with null user pointer");
    return;
  }
  switch (event->type) {
    case KR_COM_CREATE:
      printk("%s: Compositor path create event", ct->name);
      if (event->info.type == KR_COM_INPUT) {
        port = get_port(ct);
        port->path = event->path;
        event->user_path = port;
        printk("attached link");
      }
      break;
    case KR_COM_PATCH:
      printk("%s: Compositor path patch event", ct->name);
      break;
    case KR_COM_DESTROY:
      printk("%s: Compositor path delete event", ct->name);
      port = (comp_port *)event->user_path;
      if (event->info.type != KR_COM_INPUT) {
        kr_image_pixels_free(&port->frame.image);
      }
      for (i = 0; i < PORTS_MAX; i++) {
        if (&ct->ports[i] == port) {
          printk("destroy found");
          ct->ports[i].path = NULL;
          break;
        }
      }
      break;
    default:
      printke("%s: Bad event from compositor", ct->name);
      break;
  }
}

/*
static comp_port *port_find_by_name(comp_test *ct, char *name) {
  int i = 0;
  if (name == NULL) return NULL;
  for (i = 0; i < PORTS_MAX; i++) {
    if (!strcmp(ct->ports[i].name, name)) {
      return &ct->ports[i];
    }
  }
  return NULL;
}
*/

static int port_wait_readable(comp_port *port) {
  int ready;
  int err;
  int ret;
  int fd;
  comp_test *ct;
  struct epoll_event ev;
  struct epoll_event events[NEVENTS];
  ct = port->ct;
  ready = 0;
  fd = kr_compositor_port_fd(port->path);
  if (fd == -1) return -1;
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = fd;
  ret = epoll_ctl(ct->pd, EPOLL_CTL_ADD, fd, &ev);
  if (ret == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
    return -1;
  }
  ret = epoll_wait(ct->pd, events, NEVENTS, 100);
  if (ret == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
  } else {
    if (ret == 1) {
      if (events[0].events & EPOLLIN) {
        ready = 1;
      }
    }
  }
  ret = epoll_ctl(ct->pd, EPOLL_CTL_DEL, fd, NULL);
  if (ret == -1) {
    err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
  }
  return ready;
}

static int img_release_cb(void *ptr) {
  comp_port *port;
  port = (comp_port *)ptr;
  kr_image_pixels_clear(&port->frame.image);
  printk("%s %s Release cb!", port->ct->name, port->name);
  return 0;
}

static int img_ready_cb(kr_av_event *ev) {
  int ret;
  char outfile[256];
  comp_port *port;
  kr_image *img;
  port = ev->frame->image.owner;
  img = &ev->frame->image;
  img->info.fmt = KR_PIXELS_ARGB;
  printk("%s %s Ready cb!", port->ct->name, port->name);
  memset(outfile, 0, sizeof(outfile));
  snprintf(outfile, sizeof(outfile), "%s/%s_%d.jpg", test_output_dir(),
   port->ct->name, test_time());
#if KR_USE_GTK
  port->ct->result = img;
  gtk_window(port->ct);
#endif
  ret = kr_image_save_file(outfile, img);
  if (ret != 0) {
    printke("Error saving img file!");
  } else {
    printk("Successfully saved %s",outfile);
  }
  return ret;
}

static int load_image(kr_frame *frame, char *filename) {
  int ret;
  ret = kr_image_load_file(&frame->image, filename);
  if (ret != 0) {
    printke("Error loading img file! %s", filename);
    exit(1);
  } else {
    printk("%s loaded.",filename);
  }
  return ret;
}

static int source_frame_setup(comp_port *port) {
  char srcfile[256];
  memset(srcfile, 0, sizeof(srcfile));
  snprintf(srcfile, sizeof(srcfile), "%s/%s", test_dir(), images[current_image++]);
  port->frame.type = KR_IMAGE_RAW;
  port->frame.image.release = img_release_cb;
  port->frame.image.owner = port;
  //kr_image_pixels_clear(&port->frame.image);
  load_image(&port->frame, srcfile);
  port->frame.image.info.fmt = KR_PIXELS_ARGB;
  return 0;
}

static int output_frame_setup(comp_port *port) {
  port->frame.type = KR_IMAGE_RAW;
  port->frame.image.owner = port;
  port->frame.image.ready = img_ready_cb;
  port->frame.image.info.w = OUTPUT_W;
  port->frame.image.info.h = OUTPUT_H;
  port->frame.image.ps[0] = port->frame.image.info.w * 4;
  port->frame.image.info.fmt = KR_PIXELS_ARGB;
  kr_image_pixels_alloc(&port->frame.image);
  kr_image_pixels_clear(&port->frame.image);
  return 0;
}

static int create_link(comp_test *ct) {
  int ret;
  kr_compositor_path_info info;

  memset(&info, 0, sizeof(info));
  info.type = KR_COM_INPUT;
  kr_compositor_input_info_init(&info.input);
  info.input.pos.x = 0;
  info.input.pos.y = 0;
  info.input.rotation = 0;
  info.input.opacity = 1;
  ret = kr_compositor_link(ct->ports[1].path, ct->ports[0].path, &info, ct);
  if (ret != 0) printke("Error creating link");
/*
  memset(&info, 0, sizeof(info));
  info.type = KR_COM_INPUT;
  kr_compositor_input_info_init(&info.input_info);
  info.input_info.pos.x = 200;
  info.input_info.pos.y = 100;
  info.input_info.opacity = 0.75;
  info.input_info.rotation = 90;
  ret = kr_compositor_link(ct->ports[3].path, ct->ports[link_from++].path, &info, ct);
  if (ret != 0) printke("Error creating link");

  memset(&info, 0, sizeof(info));
  info.type = KR_COM_INPUT;
  kr_compositor_input_info_init(&info.input_info);
  info.input_info.pos.x = 45;
  info.input_info.pos.y = 45;
  info.input_info.rotation = 0;
  info.input_info.opacity = 0.55;
  ret = kr_compositor_link(ct->ports[3].path, ct->ports[link_from++].path, &info, ct);
  if (ret != 0) printke("Error creating link");
*/
  return ret;
}

static int create_ports(comp_test *ct) {
  comp_port *port;
  port = NULL;
  kr_compositor_port_setup setup;

  memset(&setup, 0 , sizeof(setup));
  setup.info.type = KR_COM_SOURCE;
  kr_compositor_source_info_init(&setup.info.source);
  port = get_port(ct);
  if (port == NULL) return -1;
  strcpy(port->name, names[current_name++]);
  port->info = setup.info;
  setup.control_user = port;
  port->path = kr_compositor_port(ct->comp, &setup);
  if (port->path == NULL) return -1;
  source_frame_setup(port);
/*
  memset(&setup, 0 , sizeof(setup));
  setup.info.type = KR_COM_SOURCE;
  kr_compositor_source_info_init(&setup.info.source_info);
  port = get_port(ct);
  if (port == NULL) return -1;
  strcpy(port->name, names[current_name++]);
  port->info = setup.info;
  setup.control_user = port;
  port->path = kr_compositor_port(ct->comp, &setup);
  if (port->path == NULL) return -1;
  source_frame_setup(port);

  memset(&setup, 0 , sizeof(setup));
  setup.info.type = KR_COM_SOURCE;
  kr_compositor_source_info_init(&setup.info.source_info);
  port = get_port(ct);
  if (port == NULL) return -1;
  strcpy(port->name, names[current_name++]);
  port->info = setup.info;
  setup.control_user = port;
  port->path = kr_compositor_port(ct->comp, &setup);
  if (port->path == NULL) return -1;
  source_frame_setup(port);
*/
  memset(&setup, 0 , sizeof(setup));
  setup.info.type = KR_COM_OUTPUT;
  kr_compositor_output_info_init(&setup.info.output);
  port = get_port(ct);
  if (port == NULL) return -1;
  strcpy(port->name, names[current_name++]);
  port->info = setup.info;
  setup.control_user = port;
  port->path = kr_compositor_port(ct->comp, &setup);
  if (port->path == NULL) return -1;
  output_frame_setup(port);
  return 0;
}

static int comp_teardown(comp_test *ct) {
  int i;
  for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].path != NULL) {
      printk("Calling remove");
      kr_compositor_remove(ct->ports[i].path);
    }
  }
  kr_compositor_destroy(ct->comp);
  close(ct->pd);
  free(ct);
  ct = NULL;
  return 0;
}

static int comp_run(comp_test *ct) {
  int i;
  int ret;
  comp_port *port;
  comp_port *port2;
  for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].info.type == KR_COM_OUTPUT) {
      port = &ct->ports[i];
      kr_compositor_write(port->path, &port->frame);
      break;
    }
  }
  for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].info.type == KR_COM_SOURCE) {
      port2 = &ct->ports[i];
      kr_compositor_write(port2->path, &port2->frame);
      break;
    }
  }
  for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].info.type == KR_COM_OUTPUT) {
      port = &ct->ports[i];
      ret = port_wait_readable(port);
      if (ret == 1) {
        printk("%s %s Output Port is readable!", port->ct->name, port->name);
        kr_compositor_read(port->path, &port->frame);
      }
      break;
    }
  }
 /* for (i = 0; i < PORTS_MAX; i++) {
    if (ct->ports[i].info.type == KR_COM_SOURCE) {
      port = &ct->ports[i];
      ret = port_wait_readable(port);
      if (ret == 1) {
        printk("%s %s Source Port is readable!", port->ct->name, port->name);
        kr_compositor_read(port->path, &port->frame);
      }
    }
  }
  */
  printk("Comp run is done!");
  return 0;
}

static comp_test *comp_setup(char *name) {
  comp_test *ct;
  kr_compositor_setup setup;
  int err;
  if (name == NULL) return NULL;
  ct = calloc(1, sizeof(*ct));
  ct->pd = epoll_create1(EPOLL_CLOEXEC);
  if (ct->pd == -1) {
  err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
  }
  snprintf(ct->name, sizeof(ct->name), "%s", name);
  memset(&setup, 0 , sizeof(setup));
  setup.user = ct;
  setup.event_cb = compositor_event;
  ct->comp = kr_compositor_create(&setup);
  if (ct->comp == NULL) return NULL;
  create_ports(ct);
  create_link(ct);
  return ct;
}

static int test1() {
  int ret;
  comp_test *ct;
  ct = comp_setup("Comp-Test-NewWay");
  if (ct == NULL) return 1;
  ret = comp_run(ct);
  if (ret != 0) {
    printke("Comprun ret: %d\n", ret);
  }
  ret = comp_teardown(ct);
  if (ret != 0) {
    printke("Teardown ret: %d\n", ret);
  }
  return ret;
}

int main(int argc, char *argv[]) {
  int ret;
#if KR_USE_GTK
  gtk_init(&argc, &argv);
#endif
  ret = test1();
  printk("Test1 result: %d\n", ret);
  return ret;
}
