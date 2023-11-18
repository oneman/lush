#include <cairo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <krad/radio/client.h>
#include <krad/time/stamp.h>
#include <krad/mem/mem.h>
#include <krad/loop/loop.h>
#include <krad/container/y4m/unpack.h>

#define GREY  0.197 / 0.255 * 1.0, 0.203 / 0.255 * 1.0, 0.203 / 0.255 * 1.0
#define BLUE 0.0, 0.152 / 0.255 * 1.0, 0.212 / 0.255 * 1.0
#define BGCOLOR_CLR  0.0 / 0.255 * 1.0, 0.0 / 0.255 * 1.0, 0.0 / 0.255 * 1.0, 0.255 / 0.255   * 1.0
#define ORANGE  0.255 / 0.255 * 1.0, 0.080 / 0.255 * 1.0, 0.0

typedef struct aux_cli aux_cli;

struct aux_cli {
  kr_radio_client *client;
  kr_aux_params params;
  kr_aux *aux;
  int64_t started_on;
  kr_loop *loop;
  struct {
    struct {
      int got_header;
      int got_frame;
      int got_frame_header;
      size_t frame_pos;
      int frames;
      kr_y4m_demuxer *d;
      kr_amem(mem, 256)
      int ed;
      int enabled;
      int eof;
    } in;
    struct {
      int enabled;
    } out;
  } video;
  struct {
    struct {
      int enabled;
    } in;
    struct {
      int enabled;
    } out;
  } audio;
};

typedef struct {
  char *callsign;
  int video_in;
  int video_out;
  int audio_in;
  int audio_out;
} aux_cli_setup;

int color_render(kr_frame *frame) {
  kr_image *dst;
  cairo_surface_t *cst;
  cairo_t *cr;
  dst = &frame->image;
  cst = cairo_image_surface_create_for_data(dst->px[0], CAIRO_FORMAT_ARGB32,
   dst->info.w, dst->info.h, dst->ps[0]);
  cr = cairo_create(cst);
  cairo_save(cr);
  cairo_set_source_rgb(cr, BLUE);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
  cairo_restore(cr);
  cairo_surface_flush(cst);
  cairo_destroy(cr);
  cairo_surface_destroy(cst);
  return 0;
}

int save_frame(kr_frame *frame) {
  char filename[256];
  kr_image *dst;
  cairo_surface_t *cst;
  cairo_t *cr;
  dst = &frame->image;
  snprintf(filename, sizeof(filename), "%s/images/kr_aux_%zu.png",
   getenv("HOME"), kr_unixtime());
  cst = cairo_image_surface_create_for_data(dst->px[0], CAIRO_FORMAT_ARGB32,
   dst->info.w, dst->info.h, dst->ps[0]);
  cr = cairo_create(cst);
  cairo_surface_flush(cst);
  cairo_surface_write_to_png(cst, filename);
  cairo_destroy(cr);
  cairo_surface_destroy(cst);
  return 0;
}

#define NVEC 32

static int read_frame(aux_cli *c, kr_frame *frame) {
  int n;
  int err;
  size_t pos;
  ssize_t len;
  ssize_t ret;
  struct iovec iov[NVEC];
  pos = c->video.in.frame_pos;
  len = kr_image_sizeof_pixels(&frame->image.info);
  if (len < 1) return -1;
  //printk("len of %dx%d is %zu", frame->image.info.w, frame->image.info.h, len);
  char str[8];
  if (!c->video.in.got_frame_header) {
    ret = read(STDIN_FILENO, &str, 1);
    if (ret == -1) {
      err = errno;
      printk("Frame Header fraked! %s", strerror(err));
      exit(1);
    }
    if (ret == 0) {
      if (ret == 0) {
        printk("EOF Reading frame header");
        c->video.in.eof = 1;
      }
      return 0;
    }
    if (ret == 1 && str[0] == '\n') {
      c->video.in.got_frame_header = 1;
      return 0;
    }
  }
  if (pos != len) {
    memset(iov, 0, sizeof(iov));
    n = kr_image_get_iovec(iov, NVEC, &frame->image, pos);
    ret = readv(STDIN_FILENO, iov, n);
    if (ret == -1) {
      err = errno;
      printk("Frame fraKE! %s", strerror(err));
      return -1;
    }
    if (ret == 0) {
      c->video.in.eof = 1;
      printk("EOF Reading frame .. pos %d", pos);
    }
    pos += ret;
  }
  if (pos == len) {
    printk("Got full frame %d", c->video.in.frames++);
    c->video.in.got_frame_header = 0;
    return 1;
  }
  return 0;
}

static int read_header(aux_cli *c) {
  kr_y4m_demuxer_setup setup;
  kr_image_info info;
  kr_fps_info fps;
  int ret;
  int pos;
  uint8_t input[KR_Y4M_HEADER_SIZE_MAX];
  pos = 0;
  memset(&fps, 0, sizeof(fps));
  memset(&info, 0, sizeof(info));
  memset(input, 0, sizeof(input));
  if (sizeof(c->video.in.mem) < kr_y4m_demuxer_sizeof()) exit(1);
  setup.mem = c->video.in.mem;
  setup.header = input;
  for (;;) {
    if (pos > KR_Y4M_HEADER_SIZE_MAX) return -1;
    ret = read(STDIN_FILENO, input + pos, 1);
    if (ret < 1) return -1;
    if (ret == 0) return 0;
    pos += ret;
    setup.header_sz = pos;
    c->video.in.d = kr_y4m_demuxer_init(&setup);
    if (c->video.in.d) {
      printk("AUX Client: Yes! Got Y4M header!");
      kr_y4m_demuxer_get_image_info(c->video.in.d, &info);
      kr_y4m_demuxer_get_fps(c->video.in.d, &fps);
      printk("AUX Client Y4M: %dx%d %u/%u fmt %s", info.w, info.h, fps.num,
       fps.den, kr_pixel_fmt_to_str(info.fmt));
      return 1;
    }
  }
  return -1;
}

/*
static int frame_input(aux_cli *c, kr_frame *frame) {
  static int nframes = 0;
  printf("Frame Input: %d\n", nframes++);
  read_frame(frame);
  color_render(frame);
  return 0;
}

static int frame_output(aux_cli *c, kr_frame *frame) {
  static int nframes = 0;
  printf("Frame Output: %d\n", nframes++);
  save_frame(frame);
  return 0;
}
*/

/*
    ret = read_header(c);
    if (ret) {
      printke("AUX Client: Err reading Y4M Header :/");
      return -1;
    }
*/

/*
  //int ret;
  //kr_frame frame;
  ret = kr_aux_frame_acquire(c->aux, &frame);
  ret = frame_input(c, &frame);
  if (ret) printke("AUX: Video Port ret %d", ret);
  //kr_aux_frame_release(demo->aux, frame);
  //usleep(1000 * 10);
*/

static int input_event(kr_event *e) {
  kr_frame frame;
  int ret;
  aux_cli *c;
  c = (aux_cli *)e->user;
  if (!c->video.in.got_header) {
    ret = read_header(c);
    if (ret < 0) return -1;
    if (ret == 0) return 0;
    if (ret == 1) c->video.in.got_header = 1;
    return 0;
  }
  if (c->video.in.got_header) {
    if (!c->video.in.got_frame) {
      ret = kr_aux_frame_acquire(c->aux, &frame);
      c->video.in.got_frame = 1;
      c->video.in.frame_pos = 0;
      kr_y4m_demuxer_get_image_info(c->video.in.d, &frame.image.info);
    }
    ret = read_frame(c, &frame);
    if (ret == 1) {
      kr_aux_frame_write(c->aux, &frame);
      c->video.in.got_frame = 0;
      usleep(200 * 1000);
    }
    if (ret < 0)  {
      printk("Read frame ret %d", ret);
      return -1;
    }
    if (c->video.in.eof == 1) {
      printk("Input EOF");
      exit(0);
    }
  }
  return 0;
}

static int aux_cli_startup(kr_loop *loop, void *user) {
  aux_cli *c;
  kr_event e;
  c = (aux_cli *)user;
  c->loop = loop;
  if (c->audio.in.enabled == 1 || c->audio.out.enabled == 1) {
    printf("No support for audio i/o just this moment...\n");
    return -1;
  }
  if (c->video.in.enabled == 1) {
    e.events = EPOLLIN;
    e.user = c;
    e.fd = STDIN_FILENO;
    e.handler = input_event;
    kr_loop_add(c->loop, &e);
  } else if (c->video.out.enabled == 1) {
    for (;;) {
      /*ret = kr_aux_frame_acquire(c->aux, &frame);
      kr_aux_frame_write(c->aux, &frame);
      usleep(1000 * 1100);
      kr_aux_frame_read(c->aux, &frame);
      ret = frame_output(c, &frame);
      if (ret) printke("AUX: Video Port ret %d", ret);
      ret = kr_aux_frame_release(c->aux, &frame);*/
    }
  }
  return 0;
}

static int aux_cli_destroy(void *user) {
  aux_cli *c;
  int ret;
  int fail;
  fail = 0;
  c = (aux_cli *)user;
  if (!c) return -1;
  if (!c->client) return -2;
  if (c->video.in.ed > 0) close(c->video.in.ed);
  ret = kr_aux_close(c->client, c->aux);
  if (ret) {
    fail++;
    fprintf(stderr, "Demo: Could not disconnect aux adapter\n");
  }
  ret = kr_aux_destroy(c->client, c->params.adapter);
  if (ret) {
    fail++;
    fprintf(stderr, "Could not create aux adapter.\n");
  }
  kr_client_destroy(&c->client);
  free(c);
  if (fail) return 1;
  return 0;
}

static aux_cli *aux_cli_create(aux_cli_setup *setup) {
  int fail;
  int ret;
  aux_cli *c;
  fail = 0;
  if (setup->audio_in || setup->audio_out) {
    printf("No support for audio i/o just this moment...\n");
    exit(1);
  }
  c = kr_allocz(1, sizeof(*c));
  c->started_on = time(NULL);
  snprintf(c->params.adapter, sizeof(c->params.adapter), "AuxDemo");
  snprintf(c->params.port, sizeof(c->params.port), "Hex_%4lu", c->started_on);
  c->client = kr_client_create("krad aux stdio");
  if (!c) {
    fprintf(stderr, "Could not create KR client.\n");
    fail = 1;
  }
  if (!fail) {
    kr_connect(c->client, setup->callsign);
    if (!kr_connected(c->client)) {
      fprintf(stderr, "Could not connect to %s krad radio daemon.\n",
       setup->callsign);
      fail = 1;
    }
  }
  if (!fail) {
    /* check if this exists */
    ret = kr_aux_create(c->client, c->params.adapter);
    if (ret) {
      fprintf(stderr, "Could not create aux adapter.\n");
      fail = 1;
    }
  }
  if (!fail) {
    /* check if this exists */
    if (setup->video_in) {
      c->video.in.enabled = 1;
      ret = kr_video_in(c->client, c->params.adapter, c->params.port);
      printf("rar in\n");
    } else if (setup->video_out) {
      c->video.out.enabled = 1;
      ret = kr_video_out(c->client, c->params.adapter, c->params.port);
      printf("rar out\n");
    }
    if (ret) {
      fprintf(stderr, "Could not create aux video input.\n");
      kr_client_destroy(&c->client);
      fail = 1;
    }
  }
  /* check if this is connected */
  c->aux = kr_aux_connect(c->client, &c->params);
  if (!c->aux) {
    fprintf(stderr, "Could not connect to aux adapter.\n");
    fail = 1;
  }
  /*
  kr_videoport_set_callback(videoport, videoport_process, demo);
  kr_videoport_activate(videoport);
  printf("Connected!\n");
  int i = 1000000;
  kr_frame *frame;
  */
  return c;
}

static void handle_args(aux_cli_setup *setup, char **args, int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (i > 0) {
      printf("One thing at a time for now...\n");
      exit(1);
    }
    if ((strlen(args[i]) == 1) && (memcmp(args[i], "v", 1) == 0)) {
      setup->video_in = 1;
      printf("Video In enabled.\n");
      continue;
    }
    if ((strlen(args[i]) == 1) && (memcmp(args[i], "a", 1) == 0)) {
      setup->audio_in = 1;
      printf("Audio In enabled.\n");
      continue;
    }
    if ((strlen(args[i]) == 2) && (memcmp(args[i], "vo", 2) == 0)) {
      setup->video_out = 1;
      printf("Video Out enabled.\n");
      continue;
    }
    if ((strlen(args[i]) == 2) && (memcmp(args[i], "ao", 2) == 0)) {
      setup->audio_out = 1;
      printf("Audio Out enabled.\n");
      continue;
    }
    printk("Invalid arg %s\n", args[i]);
  }
  if (!setup->video_in && !setup->audio_in && !setup->video_out
   && !setup->audio_out) {
    printf("No Audio/Video Input/Output Selected!\n");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  aux_cli *c;
  aux_cli_setup setup;
  kr_mainloop_setup main_setup;
  memset(&setup, 0, sizeof(setup));
  if (argc < 2) {
    fprintf(stderr, "No station specified.\n");
    return 1;
  }
  if (argc < 3) {
    fprintf(stderr, "No A/V Args spec'ed\n");
    return 1;
  }
  setup.callsign = argv[1];
  handle_args(&setup, argv + 2, argc - 2);
  c = aux_cli_create(&setup);
  if (!c) return 1;
  main_setup.user = c;
  main_setup.startup_handler = aux_cli_startup;
  main_setup.shutdown_handler = aux_cli_destroy;
  main_setup.signal_handler = NULL;
  kr_mainloop(&main_setup);
  return -1;
}
