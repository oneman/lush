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
  uint8_t redirect;
  char path[256];
  uint8_t path_len;
} web_client_test;

static int web_event(kr_web_event *event);
static int add_web_clients(web_client_test *test);
static int signal_event(kr_event *event);
static int stop_web_client_test(void *user);
static int start_web_client_test(kr_app_server *app, void *user);

static int web_event(kr_web_event *event) {
  ssize_t ret;
  web_client_test *test;
  uint8_t buffer[8192];
  kr_web_client *client;
  kr_web_client_info *info;
  kr_http_response *res;
  kr_http_request req;
  kr_http_header hdr;
  kr_http_url url;
  //static char *path = "/rack/uploads/";
  static char *path = "/";
  static char *ws_path = "/";
  memset(&url, 0, sizeof(url));
  client = kr_web_path_get_client(event->path);
  info = kr_web_client_get_info(client);
  switch (event->type) {
    case KR_WEB_OPEN:
      printk("WebClientTest: Open event from InterWeb");
      test = kr_web_path_user_get(event->path);
      if (1 || info->protocol == KR_PROTOCOL_WEBSOCKET) {
        kr_http_request_init(&req);
        
        printk("yay");
        kr_http_request_set_method(&req, KR_HTTP_GET);
        kr_http_request_set_path(&req, ws_path, strlen(ws_path));

        hdr.key = "Upgrade";
        hdr.key_len = strlen(hdr.key);
        hdr.value = "websocket";
        hdr.value_len = strlen(hdr.value);
        kr_http_request_add_header(&req, &hdr);
        
        hdr.key = "Connection";
        hdr.key_len = strlen(hdr.key);
        hdr.value = "Upgrade";
        hdr.value_len = strlen(hdr.value);
        kr_http_request_add_header(&req, &hdr);
        
        hdr.key = "Sec-WebSocket-Version";
        hdr.key_len = strlen(hdr.key);
        hdr.value = "13";
        hdr.value_len = strlen(hdr.value);
        kr_http_request_add_header(&req, &hdr);
        
        hdr.key = "Sec-WebSocket-Protocol";
        hdr.key_len = strlen(hdr.key);
        hdr.value = "krad-ws-api";
        hdr.value_len = strlen(hdr.value);
        kr_http_request_add_header(&req, &hdr);
        
        hdr.key = "Sec-WebSocket-Key";
        hdr.key_len = strlen(hdr.key);
        hdr.value = "AQIDBAUGBwgJCgsMDQ4PEC==";
        hdr.value_len = strlen(hdr.value);
        kr_http_request_add_header(&req, &hdr);
        
        ret = kr_http_request_string((char *)buffer, &req, sizeof(buffer));
        printk("---%.*s ---", ret, buffer);

        kr_web_client_pack_request(client, &req);
      } else {
        if (!test->redirect) {
          kr_http_request_init(&req);
          kr_http_request_set_method(&req, KR_HTTP_GET);
          kr_http_request_set_path(&req, path, strlen(path));
          kr_web_client_pack_request(client, &req);
        } else {
          kr_http_request_init(&req);
          kr_http_request_set_method(&req, KR_HTTP_GET);
          kr_http_request_set_path(&req, test->path, test->path_len);
          kr_web_client_pack_request(client, &req);
          test->redirect = 0;
        }
      }
      break;
    case KR_WEB_CLOSE:
      printk("WebClientTest: Close event from InterWeb");
      break;
    case KR_WEB_HEADERS:
      printk("WebClientTest: Headers event from InterWeb");
      res = kr_web_client_get_response(client);
      ret = kr_http_response_string((char *)buffer, res, sizeof(buffer));
      printk("%.*s", ret, buffer);
      if (res->type == KR_HTTP_FOUND || res->type == KR_HTTP_MOVED) {
        memset(&req, 0, sizeof(req));
        kr_http_response_get_header(&hdr, "Location", 8, res);
        printk("REDIRECT to %.*s detected!", hdr.value_len, hdr.value);
        kr_http_url_parse(&url, hdr.value, hdr.value_len);
        //printk("HOST: %.*s, PORT: %u, PATH: %.*s", url.host_len, url.host, url.port, url.path_len, url.path);
        test = kr_web_path_user_get(event->path);
        kr_web_path_setup_info new;
        memset(&new, 0, sizeof(new));
        new.type = KR_WEB_CLIENT;
        new.client.protocol = KR_PROTOCOL_HTTP;
        snprintf(new.client.remote.host,
         sizeof(new.client.remote.host), "%.*s", url.host_len, url.host);
        new.client.remote.port = url.port;
        test->redirect = 1;
        test->path_len = snprintf(test->path, sizeof(test->path), "%.*s", (int)url.path_len, url.path);
        ret = kr_web_path_create(test->web, &new, test, NULL);
        if (ret) {
          printke("WebClientTest: web path create returned %d", ret);
        }
      }
      break;
    case KR_WEB_DATA:
      printk("WebClientTest: Data event from InterWeb");
      for (;;) {
        ret = kr_web_client_unpack(client, buffer, sizeof(buffer));
        if (ret < 1) break;
        printf("%.*s", (int)ret, buffer);
      }
      break;
  }
  return 0;
}

static int add_web_clients(web_client_test *test) {
  int ret;
  kr_web_path_setup_info new;
  memset(&new, 0, sizeof(new));
  new.type = KR_WEB_CLIENT;
  /*new.client.protocol = KR_PROTOCOL_HTTP;*/
  new.client.protocol = KR_PROTOCOL_WEBSOCKET;
  snprintf(new.client.remote.host,
   sizeof(new.client.remote.host), "kr4d.com");
  new.client.remote.port = 80;
  ret = kr_web_path_create(test->web, &new, test, NULL);
  if (ret) {
    printke("WebClientTest: web path create returned %d", ret);
  }
  return ret;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  web_client_test *test;
  struct signalfd_siginfo fdsi;
  test = (web_client_test *)event->user;
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

static int stop_web_client_test(void *user) {
  web_client_test *test;
  kr_timer *timer;
  test = (web_client_test *)user;
  if (test == NULL) return -1;
  printk("WebClientTest: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
  kr_timer_start(timer);
  if (test->web != NULL) {
    kr_web_destroy(test->web);
    test->web = NULL;
  }
  kr_timer_status(timer);
  kr_timer_finish(timer);
  printk("WebClientTest: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  return 0;
}

static int start_web_client_test(kr_app_server *app, void *user) {
  int fail;
  fail = 0;
  web_client_test *test;
  kr_web_setup web_setup;
  test = (web_client_test *)user;
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
    printk("WebClientTest: Created");
    fail = add_web_clients(test);
  }
  if (!fail) return 0;
  stop_web_client_test(test);
  return -1;
}

void krad_web_client_test(char *callsign) {
  static web_client_test test;
  static kr_app_server_setup setup;
  printk("WebClientTest: Creating %s", callsign);
  strncpy(test.callsign, callsign, sizeof(test.callsign));
  strncpy(setup.appname, "web_client_test", sizeof(setup.appname));
  strncpy(setup.sysname, callsign, sizeof(setup.sysname));
  setup.user = &test;
  setup.startup_handler = start_web_client_test;
  setup.signal_handler = signal_event;
  setup.shutdown_handler = stop_web_client_test;
  krad_app_server(&setup);
}

int main(int argc, char *argv[]) {
  char *callsign;
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  if (argc != 2) {
    callsign = "webclienttest";
    printf("Using default %s callsign\n", callsign);
  } else {
    callsign = argv[1];
  }
  if (!kr_sysname_valid(callsign)) exit(1);
  krad_web_client_test(callsign);
  return -1;
}
