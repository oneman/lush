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
} xf_downlink_test;

static ssize_t downlink_write(void *user, uint8_t *buf,
 size_t len) {
  xf_downlink_test *test;
  ssize_t ret;
  test = (xf_downlink_test *)user;
  ret = write(1, buf, len);
  if (ret < 0) return -1;
  //printk("downlink: wrote %zu bytes.", len);
  test->sz += ret;
  return ret;
}

static int xfer_downlink_test(kr_xfer *xfer, xf_downlink_test *test) {
  kr_downlink_setup dl_setup;
  kr_xfer_client_setup setup;
  //printk("xfer_downlink_test started.");
  memset(&setup, 0, sizeof(setup));
  memset(&dl_setup, 0, sizeof(dl_setup));
  dl_setup.len = snprintf(dl_setup.url, sizeof(dl_setup.url),
    "http://ai-radio.org:8000/radio.opus");
  dl_setup.user = test;
  dl_setup.write_cb = downlink_write;
  setup.user = test;
  setup.info.type = KR_DOWNLINK;
  setup.downlink = dl_setup;
  test->xfer = xfer;
  test->xf = kr_xfer_connect(test->xfer, &setup);
  if (test->xf == NULL) {
    printke("xfer_downlink_test failed.");
    return -1;
  }
  return 0;
}
