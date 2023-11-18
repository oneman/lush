#include <sys/eventfd.h>
#include <krad/io/file2.h>
#include "output.h"

#define KR_WS_TRANSMIT_NBLOCKS 24
#define KR_HTTP_TRANSMIT_NBLOCKS 48

static ssize_t transmit_client_read(kr_transmit_client *client, uint8_t *buf,
 size_t len);
static int del_transmit_client(kr_transmit_client *client, void *user);
static int add_transmit_client(kr_transmit_client *client, void *user);
static int create_transmission(kr_studio_path *path);
static int create_recording(kr_studio_path *path);
static int create_uplink(kr_studio_path *path);

static ssize_t transmit_client_read(kr_transmit_client *client, uint8_t *buf,
 size_t len) {
  ssize_t ret;
  kr_studio_path *path;
  kr_mux_output *mux_out;
  mux_out = (kr_mux_output *)client->user;
  if (!mux_out) return -1;
  path = (kr_studio_path *)mux_out->user;
  if (path->info.output.tx.txmtr.type == KR_XFER_TRANSMITTER_WS) {
    ret = kr_mux_read_n(mux_out, buf, len, KR_WS_TRANSMIT_NBLOCKS);
  } else {
    ret = kr_mux_read_n(mux_out, buf, len, KR_HTTP_TRANSMIT_NBLOCKS);
  }
  return ret;
}

static int del_transmit_client(kr_transmit_client *client, void *user) {
  int ret;
  kr_studio_path *path;
  kr_mux_output *mux_output;
  ret = -1;
  path = (kr_studio_path *)user;
  mux_output = (kr_mux_output *)client->user;
  ret = kr_mux_detach(mux_output);
  if (ret) {
    printke("Output: del transmit client mux detach ret %d", ret);
  }
  path->info.output.tx.txmtr.clients--;
  return ret;
}

static int add_transmit_client(kr_transmit_client *client, void *user) {
  kr_studio_path *path;
  kr_mux_output_setup setup;
  studio_output *o;
  path = (kr_studio_path *)user;
  o = (studio_output *)&path->output;
  if (kr_mux_output_sizeof() > sizeof(client->mem)) {
    printk("Output: mux_output is bigger then mem given! %zu vs %zu",
      kr_mux_output_sizeof(), sizeof(client->mem));
    return -1;
  }
  setup.mem = client->mem;
  setup.tc = 0;
  setup.backbuf = client->backbuf;
  setup.user = path;
  client->user = kr_mux_attach(&o->mux_path->mux, &setup);
  if (!client->user) {
    printke("Output: error attaching to mux");
    return -2;
  }
  path->info.output.tx.txmtr.clients++;
  return 0;
}

static int transmit_destroy(void *user) {
  kr_studio_path *path;
  if (user == NULL) return -1;
  path = (kr_studio_path *)user;
  path->output.xfer = NULL;
  return 0;
}

static int create_transmission(kr_studio_path *path) {
  kr_output_info *info;
  kr_xfer_server_setup setup;
  info = &path->info.output;
  setup.txmtr.user = path;
  setup.txmtr.add_client = add_transmit_client;
  setup.txmtr.del_client = del_transmit_client;
  setup.txmtr.client_read = transmit_client_read;
  setup.txmtr.destroy_cb = transmit_destroy;
  setup.info.type = KR_XFER_TRANSMITTER;
  setup.info.len = snprintf(setup.info.path, sizeof(setup.info.path), "%s", info->tx.mount);
  setup.info.transmitter = info->tx.txmtr;
  path->output.xfer = kr_xfer_server(path->studio->xfer, &setup);
  if (!path->output.xfer) {
    printke("Output: failed to make xfer path");
    return -1;
  }
  return 0;
}

static int create_recording(kr_studio_path *path) {
  size_t sz;
  kr_file_set *fs;
  studio_output *o;
  kr_output_info *info;
  o = (studio_output *)&path->output;
  info = &path->info.output;
  fs = kr_xfer_get_file_set(path->studio->xfer);
  if (!fs) printke("Recording: could not get file set");
  sz = 32 * 1000 * 1000;
  o->file = kr_file2_create(fs, info->record.name, info->record.len, sz);
  if (!o->file) printke("Recording: could not create file");
  return 0;
}

static ssize_t uplink_read(void *user, uint8_t *buf,
 size_t len) {
  ssize_t ret;
  kr_studio_path *path;
  path = (kr_studio_path *)user;
  ret = kr_mux_read(path->output.mux_output, buf, len);
  return ret;
}

static int uplink_connect(void *user) {
  kr_studio_path *path;
  kr_mux_output_setup setup;
  if (user == NULL) return -1;
  path = (kr_studio_path *)user;
  setup.mem = path->output.mem;
  setup.tc = 0;
  setup.user = path;
  setup.backbuf = 0;
  path->output.mux_output = kr_mux_attach(&path->output.mux_path->mux, &setup);
  if (!path->output.mux_output) {
    printke("Output: uplink reconnect, mux attach failed");
    return -2;
  }
  printk("Uplink Connect");
  return 0;
}

static int uplink_disconnect(void *user) {
  int ret;
  kr_studio_path *path;
  if (user == NULL) return -1;
  path = (kr_studio_path *)user;
  if (!path->output.mux_output) return 0;
  ret = kr_mux_detach(path->output.mux_output);
  if (ret) printke("Output: uplink reconnect, mux detach ret %d", ret);
  path->output.mux_output = NULL;
  printk("Uplink Disconnect");
  return ret;
}

static int create_uplink(kr_studio_path *path) {
  studio_output *o;
  kr_output_info *info;
  kr_uplink_setup setup;
  kr_xfer_client_setup xc_setup;
  memset(&xc_setup, 0, sizeof(xc_setup));
  o = (studio_output *)&path->output;
  info = &path->info.output;
  setup.read_cb = uplink_read;
  setup.reconnect.connect_cb = uplink_connect;
  setup.reconnect.disconnect_cb = uplink_disconnect;
  setup.reconnect.max_tries = 5;
  setup.reconnect.duration = 1000;
  setup.info = info->uplink;
  setup.user = path;
  xc_setup.user = path;
  xc_setup.uplink = setup;
  xc_setup.info.type = KR_UPLINK;
  o->xfer = kr_xfer_connect(path->studio->xfer, &xc_setup);
  if (!o->xfer) return -2;
  return 0;
}

int studio_output_write(kr_studio_path *path) {
  kr_mux_output *o;
  int len;
  int ret;
  uint8_t buf[256];
  kr_output_type type;
  type = path->info.output.type;
  if (type == KR_STUDIO_TRANSMISSION || type == KR_STUDIO_UPLINK) {
    ret = kr_xfer_data(path->output.xfer);
    return ret;
  }
  o = path->output.mux_output;
  for (;;) {
    len = kr_mux_read(o, buf, sizeof(buf));
    if (len < 1) break;
    if (type == KR_STUDIO_RECORD) {
      kr_file2_append(path->output.file, buf, len);
      path->output.file_pos += len;
      printkd("Recording: wrote %d bytes, total %zu", len,
       path->output.file_pos);
    }
  }
  return 0;
}

int studio_output_of_mux(kr_studio_path *path, kr_studio_path *mux) {
  if (!path || !mux) return 0;
  if (path->output.mux_path != mux) return 0;
  return 1;
}

int studio_output_destroy(kr_studio_path *path) {
  kr_mux_output *out;
  int ret;
  kr_output_type type;
  ret = -1;
  out = path->output.mux_output;
  type = path->info.output.type;
  if (out != NULL && type != KR_STUDIO_TRANSMISSION) {
    ret = kr_mux_detach(out);
    if (ret) printke("Output: destroy, mux detach ret %d", ret);
  }
  ret = -1;
  switch (type) {
  case KR_STUDIO_RECORD:
    kr_file2_truncate(path->output.file, path->output.file_pos);
    printk("Recording: Done recording. %zu Bytes", path->output.file_pos);
    kr_file2_close(path->output.file);
    ret = 0;
    if (out) studio_mux_rm_output(out->mux, path);
    break;
  case KR_STUDIO_UPLINK:
    printk("Uplink: destroy!");
    ret = kr_xfer_remove(path->output.xfer);
    if (out) studio_mux_rm_output(out->mux, path);
    break;
  case KR_STUDIO_TRANSMISSION:
    printk("Transmission: destroy!");
    if (path->output.xfer == NULL) return 0;
    ret = kr_xfer_remove(path->output.xfer);
    studio_mux_rm_output(path->output.mux, path);
    break;
  }
  return ret;
}

int studio_output_create(kr_studio_path *path, kr_studio_path *mux) {
  int ret;
  kr_mux_output_setup setup;
  kr_output_type type;
  studio_output *o;
  o = &path->output;
  type = path->info.output.type;
  o->mux_path = mux;
  o->xfer = NULL;
  o->file_pos = 0;
  ret = -1;
  if (kr_mux_output_sizeof() > sizeof(o->mem)) {
    printke("Output: sizing fatal error %zu vs %zu",
     kr_mux_output_sizeof(), sizeof(o->mem));
    exit(1);
  }
  o->mux = &mux->mux;
  if (type == KR_STUDIO_TRANSMISSION) {
    printk("Output: Stream transmission");
    ret = create_transmission(path);
    if (ret == 0) studio_mux_add_output(&mux->mux, path);
    return ret;
  } else if (type == KR_STUDIO_UPLINK) {
    printk("Output: Stream uplink");
    ret = create_uplink(path);
    if (ret == 0) studio_mux_add_output(&mux->mux, path);
    return ret;
  }
  setup.mem = o->mem;
  setup.tc = 0;
  setup.user = path;
  setup.backbuf = 0;
  o->mux_output = kr_mux_attach(&o->mux_path->mux, &setup);
  if (!o->mux_output) {
    printke("Output: error attaching to mux");
    return -1;
  }
  if (type == KR_STUDIO_RECORD) {
    printk("Output: Recording");
    ret = create_recording(path);
    if (ret == 0) studio_mux_add_output(&mux->mux, path);
  }
  return ret;
}
