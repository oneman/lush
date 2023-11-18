#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <krad/web/web.h>
#include <krad/app/server.h>
#include <krad/time/timer.h>
#include <krad/app/common.h>
#include <krad/xfer/xfer.h>

typedef struct {
  kr_xfer *xfer;
  kr_xfer_path *xf;
  size_t sz;
} xf_uplink_test;

static ssize_t uplink_read(void *user, uint8_t *buf,
 size_t len) {
  xf_uplink_test *test;
  ssize_t ret;
  test = (xf_uplink_test *)user;
  ret = read(0, buf, len);
  if (ret < 0) return -1;
  printk("uplink: read %zu bytes.", ret);
  test->sz += ret;
  return ret;
}

static int xfer_uplink_test(kr_xfer *xfer, xf_uplink_test *test) {
  kr_uplink_setup ul_setup;
  kr_xfer_client_setup setup;
  printk("xfer_uplink_test started.");
  memset(&setup, 0, sizeof(setup));
  memset(&ul_setup, 0, sizeof(ul_setup));
  ul_setup.info.len = snprintf(ul_setup.info.url, sizeof(ul_setup.info.url),
    "http://source:coconut@178.79.143.168:8888/live.opus");
  ul_setup.user = test;
  ul_setup.read_cb = uplink_read;
  setup.user = test;
  setup.info.type = KR_UPLINK;
  setup.uplink = ul_setup;
  test->xfer = xfer;
  test->xf = kr_xfer_connect(test->xfer, &setup);
  if (test->xf == NULL) {
    printke("xfer_uplink_test failed.");
    return -1;
  }
  return 0;
}
