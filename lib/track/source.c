#include <sys/eventfd.h>
#include <krad/app/notice.h>
#include <krad/io/file2.h>
#include <krad/image/image.h>
#include "source.h"

static int studio_source_file_read(kr_studio_path *path, kr_iov *v);

static int source_file_open(kr_studio_path *path) {
  kr_file_set *fs;
  studio_source *src;
  kr_studio_source_info *info;
  kr_file2_info finfo;
  src = (studio_source *)&path->source;
  info = &path->info.source;
  fs = kr_xfer_get_file_set(path->studio->xfer);
  if (!fs) {
    printke("Source: could not get file set");
    return -1;
  } else {
    src->file = kr_file2_open(fs, info->file.path, sizeof(info->file.path));
  }
  if (!src->file) {
    info->file.size = -1;
    printke("Source: could not open file");
  } else {
    kr_file2_get_info(src->file, &finfo);
    info->file.size = finfo.sz;
    info->file.lastmod = finfo.lastmod;
  }
  return 0;
}

static int source_decode_image(kr_studio_path *path) {
  int ret;
  size_t len;
  kr_iov v;
  kr_stash_image simage;
  kr_image_stash_item *item;
  kr_studio_source_info *info;
  memset(&simage, 0, sizeof(simage));
  info = &path->info.source;
  simage.enc.info.image.fmt = 0;
  len = strlen(info->file.path);
  if (len < 5) return -1;
  if (!strncasecmp(info->file.path + len - 4, ".jpg", 4) ||
    (len > 5 && !strncasecmp(info->file.path + len - 5, ".jpeg", 5))) {
    simage.enc.info.fmt = KR_CODED_IMAGE_JPG;
  } else if (!strncasecmp(info->file.path + len - 4, ".png", 4)) {
    simage.enc.info.fmt = KR_CODED_IMAGE_PNG;
  } else return -2;
  ret = studio_source_file_read(path, &v);
  if (ret != 0) return -3;
  simage.type = KR_IMAGE_ENC;
  simage.enc.data = v.buf;
  simage.enc.info.sz = v.sz;
  item = kr_stash_decode_put(path->studio->stash, &simage);
  if (item == NULL) return -4;
  return 0;
}

static void source_parse_path_ext(kr_source_stream_info *info,
 char *out, size_t sz) {
  char *ext;
  int len;
  ext = strrchr(info->path, '.');
  if (!ext) return;
  len = info->len - (ext - info->path);
  if (len <= 1) return;
  if (len > sz) return;
  memcpy(out, ext + 1, len);
}

static ssize_t source_stream_read(kr_studio_path *path, uint8_t *buf,
 size_t len) {
  int ret;
  studio_source *src;
  studio_demux_probe_input input;
  if (!path || !buf || !len) return -1;
  memset(&input, 0, sizeof(input));
  src = &path->source;
  if (src->state == SOURCE_INIT) {
    printk("Source: start probing");
    src->state = SOURCE_PROBING;
    src->len = 0;
  }
  if (src->state == SOURCE_PROBING) {
    if (len + src->len > sizeof(src->buffer)) {
      printke("Source: stop probing, buffer full of garbage");
      return -2;
    }
    memcpy(src->buffer + src->len, buf, len);
    src->len += len;
    input.buf = src->buffer;
    input.len = src->len;
    source_parse_path_ext(&path->info.source.stream, input.ext,
     sizeof(input.ext));
    ret = studio_demux_probe(path->source.demuxer, &input);
    if (ret == 0) {
      printk("Source: probing succeeded, start demuxing");
      src->state = SOURCE_DEMUXING;
      return len;
    }
  }
  if (src->state == SOURCE_DEMUXING) {
    ret = studio_demux_read(path->source.demuxer, buf, len);
    if (ret != 0) {
      printke("Source: demux_read failed!");
      return ret;
    }
  }
  path->info.source.stream.bytes += len;
  return len;
}

static ssize_t downlink_write(void *user, uint8_t *buf,
 size_t len) {
  ssize_t ret;
  kr_studio_path *path;
  path = (kr_studio_path *)user;
  if (len == 0) return 0;
  ret = source_stream_read(path, buf, len);
  return ret;
}

static int downlink_connect(void *user) {
  int ret;
  kr_name name;
  kr_name *source_name;
  kr_studio_path *path;
  kr_studio_path_info spinfo;
  if (user == NULL) return -1;
  path = (kr_studio_path *)user;
  memset(&spinfo, 0, sizeof(spinfo));
  spinfo.type = KR_STUDIO_DEMUX;
  source_name = (kr_name *)path->user;
  name.len = snprintf(name.buf, sizeof(name.buf), "%.*sDemux",
    source_name->len, source_name->buf);
  ret = kr_studio_connect(path, &spinfo, &name);
  if (ret != 0) return ret;
  printk("Downlink Connect");
  return 0;
}

static int downlink_disconnect(void *user) {
  int ret;
  kr_studio_path *path;
  if (user == NULL) return -1;
  path = (kr_studio_path *)user;
  if (!path->source.demuxer) return 0;
  ret = kr_studio_remove(path->source.demuxer);
  if (ret != 0) return ret;
  path->source.demuxer = NULL;
  path->source.state = SOURCE_INIT;
  printk("Downlink Disconnect");
  return 0;
}

static int source_stream_create(kr_studio_path *path) {
  kr_downlink_setup dl_setup;
  kr_xfer_client_setup xc_setup;
  if (!path) return -1;
  if (path->info.source.stream.type == KR_SOURCE_STREAM_ACCEPT) return -2;
  path->source.state = SOURCE_INIT;
  memset(&xc_setup, 0, sizeof(xc_setup));
  memset(&dl_setup, 0, sizeof(dl_setup));
  dl_setup.write_cb = downlink_write;
  dl_setup.reconnect.connect_cb = downlink_connect;
  dl_setup.reconnect.disconnect_cb = downlink_disconnect;
  dl_setup.reconnect.max_tries = 6;
  dl_setup.reconnect.duration = 10000;
  memcpy(dl_setup.url, path->info.source.stream.path,
   sizeof(path->info.source.stream.path));
  dl_setup.len = path->info.source.stream.len;
  dl_setup.user = path;
  xc_setup.user = path;
  xc_setup.downlink = dl_setup;
  xc_setup.info.type = KR_DOWNLINK;
  path->source.xfer = kr_xfer_connect(path->studio->xfer, &xc_setup);
  if (!path->source.xfer) return -3;
  return 0;
}

static int source_stream_destroy(kr_studio_path *path) {
  int ret;
  if (!path) return -1;
  printk("Source: destroy");
  ret = kr_xfer_remove(path->source.xfer);
  if (ret != 0) return ret;
  if (path->source.demuxer) {
    ret = kr_studio_remove(path->source.demuxer);
    if (ret != 0) return ret;
  }
  return 0;
}

static int studio_source_file_read(kr_studio_path *path, kr_iov *v) {
  int ret;
  studio_source *src;
  kr_file2_info finfo;
  ret = -1;
  src = (studio_source *)&path->output;
  if (path->info.source.type == KR_SOURCE_LOCAL_FILE) {
    kr_file2_get_info(src->file, &finfo);
    v->buf = kr_file2_get_data(src->file);
    v->sz = finfo.sz;
    if (!v->buf || !v->sz ) ret = -1;
    ret = 0;
  }
  return ret;
}

int studio_source_destroy(kr_studio_path *path) {
  int ret;
  ret = -1;
  switch (path->info.source.type) {
    case KR_SOURCE_LOCAL_FILE:
      kr_file2_close(path->source.file);
      ret = 0;
      break;
    case KR_SOURCE_REMOTE_FILE:
      break;
    case KR_SOURCE_STREAM:
      ret = source_stream_destroy(path);
      break;
    default: break;
  }
  return ret;
}

int studio_source_create(kr_studio_path *path) {
  int ret;
  kr_name *source_name;
  ret = -1;
  memset(&path->source, 0, sizeof(path->source));
  source_name = (kr_name *)path->user;
  if (source_name->len > 6 && !memcmp("Source", source_name->buf, 6)) {
    printke("'Source' prefix is reserved for incoming sources!");
    return -2;
  }
  switch (path->info.source.type) {
    case KR_SOURCE_LOCAL_FILE:
      ret = source_file_open(path);
      if (ret != 0) break;
      break;
    case KR_SOURCE_REMOTE_FILE:
      break;
    case KR_SOURCE_STREAM:
      ret = source_stream_create(path);
      break;
    default: break;
  }
  return ret;
}
