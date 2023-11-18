#include <stdlib.h>
#include <krad/web/web.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/app/common.h>

#define DEFAULT_WEB_SERVER_TEST_PORT 4444

typedef struct {
  kr_web *web;
  kr_app_server *app;
  char callsign[16];
} web_server_test;

static int web_event(kr_web_event *event);
static int setup_web_server_test(web_server_test *test);
static int signal_event(kr_event *event);
static int stop_web_server_test(void *user);
static int start_web_server_test(kr_app_server *app, void *user);

static ssize_t say_hello(kr_web_path *path) {
  kr_web_client *client;
  kr_http_header hdr;
  kr_http_response res;
  uint8_t hdrbuf[512];
  char *hello;
  size_t sz;
  size_t hdrsz;
  hdrsz = sizeof(hdrbuf);
  hello = "Hello, world!";
  sz = strlen(hello);
  client = kr_web_path_get_client(path);
  kr_http_response_init(&res, KR_HTTP_OK);
  kr_http_length_header_init(&hdr, sz, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_web_client_pack_response(client, &res);
  return kr_web_client_pack(client, (uint8_t *)hello, sz);
}

static int web_event(kr_web_event *event) {
  int ret;
  web_server_test *test;
  kr_web_client *client;
  kr_web_path_info *info;
  kr_http_request *req;
  kr_http_response res;
  kr_http_header header;
  uint8_t data[1024];
  test = (web_server_test *)event->user;
  ret = 0;
  if (!test) return -1;
  info = kr_web_path_info_get(event->path);
  if (info->type == KR_WEB_CLIENT) {
    client = kr_web_path_get_client(event->path);
  } else {
    client = NULL;
  }
  switch (event->type) {
    case KR_WEB_OPEN:
      printk("WebServerTest: Open event from InterWeb");
      ret = 0;
      break;
    case KR_WEB_CLOSE:
      printk("WebServerTest: Close event from InterWeb");
      ret = 0;
      break;
    case KR_WEB_HEADERS:
      printk("WebServerTest: Headers event from InterWeb");
      req = kr_web_client_get_request(client);
      if (kr_http_req_indata(req)) {
        printk("WebServerTest: Incoming data..");
        ret = kr_http_get_header(&header, "Expect", 6, req);
        if (ret == 0) {
          printk("WebServerTest: Sending continue response");
          kr_http_response_init(&res, KR_HTTP_CONTINUE);
          kr_web_client_pack_response(client, &res);
          ret = 0;
          break;
        }
      } else {
        if (info->client.protocol == KR_PROTOCOL_HTTP) {
          say_hello(event->path);
          ret = 0;
          break;
        }
      }
      kr_http_response_init(&res, KR_HTTP_NOT_FOUND);
      kr_http_response_zero_content(&res);
      kr_http_response_connection_close(&res);
      kr_web_client_pack_response(client, &res);
      ret = 0;
      break;
    case KR_WEB_DATA:
      printk("WebServerTest: Data event from InterWeb");
      for (;;) {
        ret = kr_web_client_unpack(client, data, sizeof(data));
        if (ret < 1) break;
        printk("WebServerTest: Yay we read %d bytes of data!", ret);
      }
      ret = 0;
      break;
    default:
      printke("WebServerTest: Unknown event from InterWeb");
      ret = -1;
      break;
  }
  return ret;
}

static int setup_web_server_test(web_server_test *test) {
  kr_web_path_setup_info new;
  memset(&new, 0, sizeof(new));
  if (getenv("KRAD_WEB_TEST_PORT")) {
    new.server.port = atoi(getenv("KRAD_WEB_PORT"));
  } else {
    new.server.port = DEFAULT_WEB_SERVER_TEST_PORT;
  }
  new.type = KR_WEB_SERVER;
  new.server.type = KR_WEB_SERVER_HTTP;
  if (kr_web_path_create(test->web, &new, "coconutptr", NULL)) return -1;
  return 0;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  web_server_test *test;
  struct signalfd_siginfo fdsi;
  test = (web_server_test *)event->user;
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

static int stop_web_server_test(void *user) {
  web_server_test *test;
  kr_timer *timer;
  test = (web_server_test *)user;
  if (test == NULL) return -1;
  printk("WebServerTest: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
  kr_timer_start(timer);
  if (test->web != NULL) {
    kr_web_destroy(test->web);
    test->web = NULL;
  }
  kr_timer_status(timer);
  kr_timer_finish(timer);
  printk("WebServerTest: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  return 0;
}

static int start_web_server_test(kr_app_server *app, void *user) {
  int fail;
  fail = 0;
  web_server_test *test;
  kr_web_setup web_setup;
  test = (web_server_test *)user;
  test->app = app;
  memset(&web_setup, 0, sizeof(kr_web_setup));
  web_setup.event_cb = web_event;
  web_setup.user = test;
  web_setup.npaths = 64;
  if (1) {
    web_setup.master_loop = kr_app_server_mainloop_get(app);
  }
  test->web = kr_web_create(&web_setup);
  if (!test->web) fail = 1;
  if (!fail) {
    fail = setup_web_server_test(test);
  }
  if (!fail) {
    printk("WebServerTest: Created");
    return 0;
  }
  stop_web_server_test(test);
  return -1;
}

void krad_web_server_test(char *callsign) {
  static web_server_test test;
  static kr_app_server_setup setup;
  printk("WebServerTest: Creating %s", callsign);
  strncpy(test.callsign, callsign, sizeof(test.callsign));
  strncpy(setup.appname, "web_server_test", sizeof(setup.appname));
  strncpy(setup.sysname, callsign, sizeof(setup.sysname));
  setup.user = &test;
  setup.startup_handler = start_web_server_test;
  setup.signal_handler = signal_event;
  setup.shutdown_handler = stop_web_server_test;
  krad_app_server(&setup);
}

int main(int argc, char *argv[]) {
  char *callsign;
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  if (argc != 2) {
    callsign = "webservertest";
    printf("Using default %s callsign\n", callsign);
  } else {
    callsign = argv[1];
  }
  if (!kr_sysname_valid(callsign)) exit(1);
  krad_web_server_test(callsign);
  return -1;
}
