#include <krad/radio/client.h>
#include <krad/app/debug.h>
#include "types.h"

int kr_xpdr_make(kr_client *client, char *name, kr_xpdr_path_info *info) {
  int ret;
  kr_crate crate;
  if (client == NULL) return -1;
  if (name == NULL) return -2;
  if (info == NULL) return -3;
  crate.addr_len = snprintf(crate.address, sizeof(crate.address), "/xpdr/%s", name);
  crate.method = KR_PUT;
  crate.type = KR_XPDR_PATH_INFO;
  crate.info.xpdr_path_info = *info;
  char json[4096];
  ret = kr_crate_pack_json(json, &crate, sizeof(json));
  printk("%s", json);
  ret = kr_crate_send(client, &crate);
  return ret;
}
