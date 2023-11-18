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
  char *path;
} xf_upload_test;

static int xfer_upload_test(kr_xfer *xfer, xf_upload_test *test) {
  kr_xfer_client_setup setup;
  kr_upload_info info;
  printk("xfer_upload_test started.");
  memset(&setup, 0, sizeof(setup));
  info.type = KR_UPLOAD_FILE;
  info.file.url_len = snprintf(info.file.url,
   sizeof(info.file.url), "http://requestb.in/pays7opa");
  info.file.path_len = snprintf(info.file.path,
   sizeof(info.file.path), "%s", test->path);
  setup.user = test;
  setup.info.type = KR_UPLOAD;
  setup.upload = info;
  test->xfer = xfer;
  test->xf = kr_xfer_connect(test->xfer, &setup);
  if (test->xf == NULL) {
    printke("xfer_upload_test failed.");
    return -1;
  }
  return 0;
}
