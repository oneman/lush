#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <krad/web/web.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/app/common.h>
#include <krad/xfer/xfer.h>

#include "xfer_uplink_test.c"
#include "xfer_downlink_test.c"
#include "xfer_upload_test.c"
#include "xfer_download_test.c"
#include "xfer_transmit_test.c"

typedef enum {
  XF_TEST_UPLOAD = 1,
  XF_TEST_DOWNLOAD,
  XF_TEST_UPLINK,
  XF_TEST_DOWNLINK,
  XF_TEST_TRANSMIT
} xf_test_type;

typedef struct {
  kr_app_server *app;
  kr_web *web;
  kr_xfer *xfer;
  char callsign[16];
  char **argv;
  int argc;
  xf_test_type type;
  union {
    xf_upload_test upload;
    xf_download_test download;
    xf_uplink_test uplink;
    xf_downlink_test downlink;
    xf_transmit_test transmit;
  };
} xf_test;

static void web_setup(xf_test *test);
static void xfer_setup(xf_test *test);

static void print_usage() {
  printk("Usage:\n  ./xfer_test type [args]");
  printk("Types are: upload download uplink downlink transmit");
  printk("Examples:\n  ./xfer_test upload filepath");
  printk("  ./xfer_test download filepath");
  printk("  cat audio.opus | ./xfer_test uplink");
  printk("  ./xfer_test downlink 1> out.opus");
  printk("  cat audio.opus | ./xfer_test transmit");
}

static int str_to_xf_test_type(char *str) {
  if (!strcasecmp(str, "upload")) return XF_TEST_UPLOAD;
  if (!strcasecmp(str, "download")) return XF_TEST_DOWNLOAD;
  if (!strcasecmp(str, "uplink")) return XF_TEST_UPLINK;
  if (!strcasecmp(str, "downlink")) return XF_TEST_DOWNLINK;
  if (!strcasecmp(str, "transmit")) return XF_TEST_TRANSMIT;
  return 0;
}

static int test_404(kr_web_client *client) {
  int ret;
  uint64_t sz;
  kr_http_header hdr;
  kr_http_response res;
  uint8_t hdrbuf[512];
  static const char *not_found;
  not_found = "NO CARRIER\n";
  sz = strlen(not_found);
  kr_http_response_init(&res, KR_HTTP_NOT_FOUND);
  kr_http_length_header_init(&hdr, sz, hdrbuf, sizeof(hdrbuf));
  kr_http_response_add_header(&res, &hdr);
  ret = kr_web_client_pack_response(client, &res);
  if (ret > 0) kr_web_client_pack(client, (uint8_t *)not_found, sz);
  return 0;
}

static int web_event(kr_web_event *event) {
  int ret;
  xf_test *test;
  kr_web_path_info *info;
  kr_web_client *client;
  //kr_http_request *req;
  //kr_path *addr;
  kr_xfer_path *xf;
  client = NULL;
  xf = NULL;
  test = (xf_test *)event->user;
  if (test == NULL) return -1;
  info = kr_web_path_info_get(event->path);
  if (info->type == KR_WEB_CLIENT) {
    client = kr_web_path_get_client(event->path);
  }
  ret = -1;
  switch (event->type) {
    case KR_WEB_OPEN:
      printk("Radio: Open event from InterWeb");
      ret = 0;
      break;
    case KR_WEB_CLOSE:
      printk("Radio: Close event from InterWeb");
      if (info->type != KR_WEB_CLIENT) break;
      xf = kr_web_path_user_get(event->path);
      kr_xfer_remove(xf);
      ret = 0;
      break;
    case KR_WEB_HEADERS:
      printk("Radio: Headers event from InterWeb");
      if (info->client.protocol == KR_PROTOCOL_HTTP) {
        xf = kr_web_path_user_get(event->path);
        if (!xf) {
          ret = kr_xfer_accept(test->xfer, client, event->path);
          if (ret == 0) {
            xf = kr_web_path_user_get(event->path);
            ret = kr_xfer_headers(xf);
            if (ret == 0) {
              if (kr_xfer_done(xf)) {
                kr_xfer_remove(xf);
              }
              break;
            }
          }
        } else {
          ret = kr_xfer_headers(xf);
          if (ret == 0) {
            if (kr_xfer_done(xf)) {
              kr_xfer_remove(xf);
            }
            break;
          }
        }
      }
      test_404(client);
      ret = 0;
      break;
    case KR_WEB_DATA:
      if (info->type != KR_WEB_CLIENT) break;
      printk("Radio: Data event from InterWeb");
      xf = kr_web_path_user_get(event->path);
      ret = kr_xfer_data(xf);
      if (ret < 0) test_404(client);
      if (kr_xfer_done(xf)) {
        kr_xfer_remove(xf);
      }
      break;
    default:
      printke("Radio: Unknown event from InterWeb");
      ret = -1;
      break;
  }
  return ret;
}

static int xfer_event(kr_xfer_event *event) {
  int ret;
  xf_test *test;
  ret = 0;
  test = (xf_test *)event->user;
  if (!test) return -1;
  switch (event->type) {
    case KR_XFER_CREATE:
      printk("Radio: XFER Create event");
      break;
    case KR_XFER_PATCH:
      printk("Radio: XFER Patch event");
      break;
    case KR_XFER_DESTROY:
      printk("Radio: XFER Destroy event");
      break;
    default:
      printk("Radio: Some XFER event");
  }
  return ret;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  xf_test *test;
  struct signalfd_siginfo fdsi;
  test = (xf_test *)event->user;
  char *name = test->callsign;
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      default:
        printk("%s: Got Signal %u", name, fdsi.ssi_signo);
    }
  }
  return 0;
}

static int xf_test_start(kr_app_server *app, void *user) {
  int ret;
  xf_test *test;
  test = (xf_test *)user;
  web_setup(test);
  xfer_setup(test);
  ret = -1;
  switch (test->type) {
    case XF_TEST_UPLOAD:
      if (test->argc != 2) return -1;
      test->upload.path = test->argv[1];
      ret = xfer_upload_test(test->xfer, &test->upload);
      break;
    case XF_TEST_DOWNLOAD:
      if (test->argc != 2) return -1;
      test->download.path = test->argv[1];
      ret = xfer_download_test(test->xfer, &test->download);
      break;
    case XF_TEST_UPLINK:
      ret = xfer_uplink_test(test->xfer, &test->uplink);
      break;
    case XF_TEST_DOWNLINK:
      ret = xfer_downlink_test(test->xfer, &test->downlink);
      break;
    case XF_TEST_TRANSMIT:
      ret = xfer_transmit_test(test->web, test->xfer, &test->transmit);
      break;
  }
  return ret;
}

static int xf_test_stop(void *user) {
  xf_test *test;
  kr_timer *timer;
  test = (xf_test *)user;
  if (test == NULL) return -1;
  printk("XferTest: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
  kr_timer_start(timer);
  if (test->web != NULL) {
    kr_web_destroy(test->web);
    test->web = NULL;
  }
  if (test->xfer != NULL) {
    kr_xfer_destroy(test->xfer);
    test->xfer = NULL;
  }
  kr_timer_status(timer);
  kr_timer_finish(timer);
  printk("XferTest: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  return 0;
}

static void app_setup(xf_test *test) {
  kr_app_server_setup setup;
  memset(&setup, 0, sizeof(setup));
  strncpy(test->callsign, "xf_test", 7);
  strncpy(setup.appname, "xf_test", sizeof(setup.appname));
  strncpy(setup.sysname, "xf_test", 7);
  setup.user = test;
  setup.startup_handler = xf_test_start;
  setup.signal_handler = signal_event;
  setup.shutdown_handler = xf_test_stop;
  krad_app_server(&setup);
}

static void web_setup(xf_test *test) {
  kr_web_setup web_setup;
  memset(&web_setup, 0, sizeof(kr_web_setup));
  web_setup.event_cb = web_event;
  web_setup.user = test;
  web_setup.npaths = 16;
  web_setup.master_loop = kr_app_server_mainloop_get(test->app);
  test->web = kr_web_create(&web_setup);
}

static void xfer_setup(xf_test *test) {
  kr_xfer_setup xfer_setup;
  memset(&xfer_setup, 0, sizeof(kr_xfer_setup));
  xfer_setup.event_cb = xfer_event;
  xfer_setup.user = test;
  xfer_setup.npaths = 16;
  xfer_setup.web = test->web;
  test->xfer = kr_xfer_create(&xfer_setup);
}

static int xfer_test(int argc, char *argv[]) {
  xf_test test;
  memset(&test, 0, sizeof(test));
  test.type = str_to_xf_test_type(argv[0]);
  if (test.type <= 0) return -1;
  test.argc = argc;
  test.argv = argv;
  app_setup(&test);
  return 0;
}

int main(int argc, char *argv[]) {
  int res;
  if ( argc <= 1 || !strcmp(argv[1], "help") ) {
    print_usage();
    return 0;
  }
  res = xfer_test(argc - 1, argv + 1);
  if (res != 0) fprintf(stderr, "Failure!\n");
  return res;
}
