#include <stdlib.h>
#include <krad/web/web.h>
#include <krad/proxy/proxy.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/app/common.h>

typedef struct {
  kr_web *web;
  kr_proxy *proxy;
  uint8_t *mem;
  kr_app_server *app;
  char callsign[16];
  int listen;
  int upstream;
} proxy_test;

static int signal_event(kr_event *event);

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

static ssize_t reply_not_found(kr_web_path *path) {
  kr_web_client *client;
  kr_http_header hdr;
  kr_http_response res;
  uint8_t hdrbuf[512];
  char *msg;
  size_t sz;
  size_t hdrsz;
  hdrsz = sizeof(hdrbuf);
  msg = "I can't find that.";
  sz = strlen(msg);
  client = kr_web_path_get_client(path);
  kr_http_response_init(&res, KR_HTTP_NOT_FOUND);
  kr_http_length_header_init(&hdr, sz, hdrbuf, hdrsz);
  kr_http_response_add_header(&res, &hdr);
  kr_http_response_connection_close(&res);
  kr_web_client_pack_response(client, &res);
  return kr_web_client_pack(client, (uint8_t *)msg, sz);
}

static int web_event(kr_web_event *event) {
  int ret;
  proxy_test *test;
  kr_web_client *client;
  kr_web_path_info *info;
  kr_http_request *req;
  kr_path *addr;
  kr_http_method meth;
  size_t path_len;
  uint8_t data[1024];
  test = (proxy_test *)event->user;
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
      if (info->client.protocol == KR_PROTOCOL_WEBSOCKET) {
        if (kr_proxy_accept(test->proxy, event->path, NULL)) {
          printk("we matched the websocket proxy!");
          break;
        }
      } else if (info->client.protocol == KR_PROTOCOL_HTTP) {
        req = kr_web_client_get_request(client);
        meth = req->req_line.method;
        kr_path_alloca(addr);
        path_len = req->req_line.path_len;
        if ((req->req_line.path_len > 0)
         && (req->req_line.path[req->req_line.path_len - 1] == '/')) {
          path_len--;
        }
        kr_path_parse(addr, req->req_line.path, path_len);
        if (meth == KR_HTTP_GET && !kr_path_steps(addr)) {
          say_hello(event->path);
          ret = 0;
          break;
        }
        if (kr_proxy_accept(test->proxy, event->path, addr)) {
          printk("we matched the proxy!");
          break;
        }
        reply_not_found(event->path);
        ret = 0;
      }
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

static int setup_proxy_test(proxy_test *test) {
  kr_web_path_setup_info new;
  kr_proxy_setup ps;
  memset(&new, 0, sizeof(new));
  memset(&ps, 0, sizeof(ps));
  new.type = KR_WEB_SERVER;
  new.server.type = KR_WEB_SERVER_HTTP;
  snprintf(new.server.iface, sizeof(new.server.iface), "0.0.0.0");
  new.server.port = test->listen;
  if (kr_web_path_create(test->web, &new, "coconutptr", NULL)) return -1;
  test->mem = kr_allocz(1, kr_proxy_sizeof());
  ps.mem = test->mem;
  ps.web = test->web;
  ps.info.type = KR_PROXY_WEBSOCKET_TO_TCP;
  memcpy(ps.info.dst.host, "127.0.0.1", 9);
  ps.info.dst.host_len = 9;
  ps.info.dst.port = test->upstream;
  /*snprintf(ps.info.path, sizeof(ps.info.path), "rack");*/
  snprintf(ps.info.path, sizeof(ps.info.path), "%.*s", 0, "");
  test->proxy = kr_proxy_create(&ps);
  if (!test->proxy) {
    printke("Failed to create proxy");
    return -1;
  }
  return 0;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  proxy_test *test;
  struct signalfd_siginfo fdsi;
  test = (proxy_test *)event->user;
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

static int stop_proxy_test(void *user) {
  int ret;
  proxy_test *test;
  kr_timer *timer;
  test = (proxy_test *)user;
  if (test == NULL) return -1;
  printk("WebProxyTest: Destroying");
  timer = kr_timer_alloca();
  kr_timer_name_set(timer, "Destroy");
  kr_timer_start(timer);
  if (test->proxy) {
    ret = kr_proxy_destroy(test->proxy);
    if (ret) printke("test proxy destroy");
    test->proxy = NULL;
    free(test->mem);
  }
  if (test->web) {
    kr_web_destroy(test->web);
    test->web = NULL;
  }
  kr_timer_status(timer);
  kr_timer_finish(timer);
  printk("WebProxyTest: Destroyed in %"PRIu64"ms", kr_timer_duration_ms(timer));
  return 0;
}

static int start_proxy_test(kr_app_server *app, void *user) {
  int fail;
  fail = 0;
  proxy_test *test;
  kr_web_setup web_setup;
  test = (proxy_test *)user;
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
    fail = setup_proxy_test(test);
  }
  if (!fail) {
    printk("WebProxyTest: Created");
    return 0;
  }
  stop_proxy_test(test);
  return -1;
}

void run_proxy_test(char *callsign, int listen, int upstream) {
  static proxy_test test;
  static kr_app_server_setup setup;
  printk("WebProxyTest: Creating %s", callsign);
  strncpy(test.callsign, callsign, sizeof(test.callsign));
  strncpy(setup.appname, "proxy_test", sizeof(setup.appname));
  strncpy(setup.sysname, callsign, sizeof(setup.sysname));
  test.listen = listen;
  test.upstream = upstream;
  setup.user = &test;
  setup.startup_handler = start_proxy_test;
  setup.signal_handler = signal_event;
  setup.shutdown_handler = stop_proxy_test;
  krad_app_server(&setup);
}

void print_usage() {
  printk("./web_proxy_test listen upstream");
}

int main(int argc, char *argv[]) {
  char *callsign;
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  if (argc != 3) {
    print_usage();
    return 1;
  }
  callsign = "webproxytest";
  if (!kr_sysname_valid(callsign)) exit(1);
  run_proxy_test(callsign, atoi(argv[1]), atoi(argv[2]));
  return -1;
}
