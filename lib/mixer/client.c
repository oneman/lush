#include "client.h"

int kr_mixer_make(kr_client *client, char *name, kr_mixer_path_info *info) {
  int ret;
  kr_crate crate;
  if (client == NULL) return -1;
  if (name == NULL) return -2;
  if (info == NULL) return -3;
  crate.addr_len = snprintf(crate.address, sizeof(crate.address), "/mixer/%s", name);
  crate.method = KR_PUT;
  crate.type = KR_MIXER_PATH_INFO;
  crate.info.mixer_path_info = *info;
  char json[4096];
  ret = kr_crate_pack_json(json, &crate, sizeof(json));
  printk("%s", json);
  ret = kr_crate_send(client, &crate);
  return ret;
}
