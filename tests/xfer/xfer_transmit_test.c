#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <krad/web/web.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/app/common.h>
#include <krad/xfer/xfer.h>
#include <krad/util/strton.h>

typedef struct {
  kr_web *web;
  kr_xfer *xfer;
  kr_xfer_path *xf;
} xf_transmit_test;

static ssize_t transmit_client_read(kr_transmit_client *client, uint8_t *buf,
 size_t len) {
  ssize_t ret;
  //printk("transmit_client_read");
  ret = read(0, buf, len);
  return ret;
}

static int del_transmit_client(kr_transmit_client *client, void *user) {
  printk("xfer_transmit_test: del client.");
  return 0;
}

static int add_transmit_client(kr_transmit_client *client, void *user) {
  printk("xfer_transmit_test: add client.");
  return 0;
}

static int xfer_transmit_test(kr_web *web, kr_xfer *xfer,
 xf_transmit_test *test) {
  int ret;
  kr_web_path_setup_info wp;
  char *web_port;
  kr_xfer_server_setup setup;
  printk("xfer_transmit_test started.");
  memset(&setup, 0, sizeof(setup));
  test->web = web;
  test->xfer = xfer;
  web_port = getenv("KRAD_WEB_PORT");
  if (web_port == NULL) return -1;
  memset(&wp, 0, sizeof(wp));
  wp.type = KR_WEB_SERVER;
  wp.server.type = KR_WEB_SERVER_HTTP;
  snprintf(wp.server.iface, sizeof(wp.server.iface), "0.0.0.0");
  ret = kr_strtou16(&wp.server.port, web_port, strlen(web_port));
  if (ret != 0) return -2;
  ret = kr_web_path_create(test->web, &wp, "coconutptr", NULL);
  memset(&setup, 0, sizeof(setup));
  setup.txmtr.user = test;
  setup.txmtr.add_client = add_transmit_client;
  setup.txmtr.del_client = del_transmit_client;
  setup.txmtr.client_read = transmit_client_read;
  setup.info.type = KR_XFER_TRANSMITTER;
  setup.info.len = snprintf(setup.info.path,
   sizeof(setup.info.path), "live.ogg");
  test->xf = kr_xfer_server(test->xfer, &setup);
  if (!test->xf) {
    printke("xfer_transmit_test failed.");
    return -3;
  }
  return 0;
}
