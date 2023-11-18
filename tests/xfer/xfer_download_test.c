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
} xf_download_test;

static int xfer_download_test(kr_xfer *xfer, xf_download_test *test) {
  kr_download_info info;
  kr_xfer_client_setup setup;
  printk("xfer_download_test started.");
  memset(&info, 0, sizeof(info));
  memset(&setup, 0, sizeof(setup));
  info.type = KR_DOWNLOAD_FILE;
  info.file.url_len = snprintf(info.file.url, sizeof(info.file.url),
    "http://kr4d.com/rack/uploads/MarsDesert-Yoshi2-kzjLeKM3P.jpg");
  info.file.path_len = snprintf(info.file.path,
   sizeof(info.file.path), "%s", test->path);
  setup.user = test;
  setup.info.type = KR_DOWNLOAD;
  setup.download = info;
  test->xfer = xfer;
  test->xf = kr_xfer_connect(test->xfer, &setup);
  if (test->xf == NULL) {
    printke("xfer_download_test failed.");
    return -1;
  }
  return 0;
}
