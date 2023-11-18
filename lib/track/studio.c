#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <krad/mem/mem.h>
#include <krad/mem/pool.h>
#include <krad/app/debug.h>
#include <krad/io/file2.h>
#include <krad/compositor/types.h>
#include <krad/compositor/io.h>
#include <krad/audio/audio.h>
#include <krad/image/image.h>
#include <krad/mixer/mixer.h>
#include <krad/compositor/compositor.h>

#include "studio.h"
#include "capture.h"
#include "mux.h"
#include "demux.h"
#include "encode.h"
#include "play.h"
#include "output.h"
#include "render.h"
#include "source.h"
#include "generate.h"

struct kr_studio_path {
  kr_studio *studio;
  kr_studio_path_info info;
  union {
    studio_source source;
    studio_generator generator;
    studio_capture capture;
    studio_render render;
    studio_encode encode;
    studio_play play;
    studio_mux mux;
    studio_demux demux;
    studio_output output;
  };
  void *user;
};

struct kr_studio {
  kr_studio_event_cb *event_cb;
  void *user;
  kr_pool *pool;
  kr_tracker *tracker;
  kr_xfer *xfer;
  kr_compositor *compositor;
  kr_mixer *mixer;
  kr_image_stash *stash;
};

static void run_event(kr_studio_path *path, kr_studio_event_type type);
static int studio_add(kr_studio *studio, kr_studio_path *path,
 kr_studio_path_info *info, void *user);

#include "media.c"
#include "capture.c"
#include "encode.c"
#include "play.c"
#include "mux.c"
#include "demux.c"
#include "output.c"
#include "render.c"
#include "source.c"
#include "generate.c"

static void run_event(kr_studio_path *path, kr_studio_event_type type) {
  kr_studio_event event;
  event.user = path->studio->user;
  event.user_path = path->user;
  event.path = path;
  event.type = type;
  event.info = path->info;
  path->studio->event_cb(&event);
  path->user = event.user_path;
}

ssize_t kr_studio_source_read(kr_studio_path *path,
 uint8_t *buf, size_t len) {
  if (!path) return -1;
  if (path->info.type != KR_STUDIO_SOURCE) return -2;
  if (path->info.source.type != KR_SOURCE_STREAM) return -3;
  if (path->info.source.stream.type != KR_SOURCE_STREAM_ACCEPT) return -4;
  return source_stream_read(path, buf, len);
}

int kr_studio_source_destroy(kr_studio_path *path) {
  int ret;
  printk("Source (accept): destroy");
  if (path->source.demuxer) {
    ret = kr_studio_remove(path->source.demuxer);
    if (ret != 0) return ret;
  } else {
    printk("Source (accept): no demuxer found.");
  }
  run_event(path, KR_STUDIO_REMOVE);
  kr_pool_release(path->studio->pool, path);
  return 0;
}

int kr_studio_source_create(kr_studio *studio,
 kr_studio_path_info *info, void *user) {
  int ret;
  kr_name name;
  kr_studio_path *new;
  kr_studio_path_info spinfo;
  if (!studio || !info || !user) return -1;
  new = kr_pool_slice(studio->pool);
  if (new == NULL) return -1;
  new->studio = studio;
  new->user = user;
  new->info = *info;
  new->source.state = SOURCE_INIT;
  memset(&spinfo, 0, sizeof(spinfo));
  spinfo.type = KR_STUDIO_DEMUX;
  spinfo.demux.source = new->info.source.stream.num;
  name.len = snprintf(name.buf, sizeof(name.buf), "Source%dDemux",
    new->info.source.stream.num);
  ret = kr_studio_connect(new, &spinfo, &name);
  if (ret != 0) printke("Source: studio_add failed!");
  if (ret == 0) run_event(new, KR_STUDIO_ADD);
  if (ret) {
    kr_pool_release(studio->pool, new);
    printke("Studio: add failed %d", ret);
  }
  return ret;
}

int kr_studio_source_decode_image(kr_studio_path *path) {
  if (!path) return -1;
  if (path->info.type != KR_STUDIO_SOURCE) return -2;
  if (path->info.source.type != KR_SOURCE_LOCAL_FILE) return -3;
  return source_decode_image(path);
}

int kr_studio_path_info_get(kr_studio_path *path, kr_studio_path_info *info) {
  if (!path || !info) return -1;
  *info = path->info;
  return 0;
}

int kr_studio_ctl(kr_studio_path *path, kr_patchset *patchset) {
  if (!path || !patchset) return -1;
  printk("Studio: ctl");
  switch (path->info.type) {
    case KR_STUDIO_CAPTURE:
      kr_patchset_apply(&path->info.capture, patchset);
      break;
    case KR_STUDIO_ENCODE:
      kr_patchset_apply(&path->info.encode, patchset);
      break;
    case KR_STUDIO_MUX:
      kr_patchset_apply(&path->info.mux, patchset);
      break;
    default:
      return -1;
      break;
  }
  run_event(path, KR_STUDIO_PATCH);
  return 0;
}

int kr_studio_remove(kr_studio_path *path) {
  kr_studio_path *src;
  int ret;
  if (!path) return -1;
  printk("Studio: remove");
  ret = 0;
  switch (path->info.type) {
  case KR_STUDIO_SOURCE:
    printk("studio rm source");
    ret = studio_source_destroy(path);
    break;
  case KR_STUDIO_GENERATE:
    ret = studio_generator_destroy(path);
    break;
  case KR_STUDIO_CAPTURE:
    ret = studio_capture_destroy(path);
    break;
  case KR_STUDIO_RENDER:
    ret = studio_render_destroy(path);
    break;
  case KR_STUDIO_PLAY:
    ret = studio_play_destroy(path);
    break;
  case KR_STUDIO_ENCODE:
    ret = studio_encode_destroy(path);
    break;
  case KR_STUDIO_DECODE:
    /* FIXME */
    printk("Studio: decode path type not implemented");
    break;
  case KR_STUDIO_DEMUX:
    printk("studio rm demux");
    if (path->demux.source != NULL) {
      printk("studio: need to handle this properly!");
      src = path->demux.source;
      path->demux.source = NULL;
      ret = kr_studio_remove(path);
      if (ret != 0) return ret;
      src->source.demuxer = NULL;
      ret = kr_studio_remove(src);
      return ret;
    } else {
      ret = studio_demux_destroy(path);
    }
    break;
  case KR_STUDIO_MUX:
    printk("studio rm mux");
    ret = studio_mux_destroy(path);
    break;
  case KR_STUDIO_OUTPUT:
    printk("studio rm output");
    ret = studio_output_destroy(path);
    break;
  default:
    printke("Studio: unknown path remove type");
    return -1;
  }
  run_event(path, KR_STUDIO_REMOVE);
  kr_pool_release(path->studio->pool, path);
  return ret;
}

static int studio_add(kr_studio *studio, kr_studio_path *path,
 kr_studio_path_info *info, void *user) {
  int ret;
  kr_studio_path *new;
  if (!studio || !info || !user) return -1;
  ret = -1;
  printk("Studio: add");
  new = kr_pool_slice(studio->pool);
  if (new == NULL) return -1;
  new->studio = studio;
  new->user = user;
  new->info = *info;
  switch (new->info.type) {
  case KR_STUDIO_SOURCE:
    ret = studio_source_create(new);
    break;
  case KR_STUDIO_GENERATE:
    ret = studio_generator_create(new);
    break;
  case KR_STUDIO_CAPTURE:
    ret = studio_capture_create(new);
    break;
  case KR_STUDIO_RENDER:
    ret = studio_render_create(new);
    break;
  case KR_STUDIO_PLAY:
    ret = studio_play_create(new);
    break;
  case KR_STUDIO_ENCODE:
    ret = studio_encode_create(new);
    break;
  case KR_STUDIO_DECODE:
    /* FIXME */
    printk("Studio: decode path type not implemented");
    ret = -1;
    break;
  case KR_STUDIO_DEMUX:
    ret = studio_demux_create(new);
    if (path && path->info.type == KR_STUDIO_SOURCE) {
      path->source.demuxer = new;
      path->source.demuxer->demux.source = path;
    }
    break;
  case KR_STUDIO_MUX:
    ret = studio_mux_create(new);
    break;
  case KR_STUDIO_OUTPUT:
    if (path) {
      ret = studio_output_create(new, path);
    } else printke("Studio Output: path missing :(");
    break;
  default:
    printke("Studio: unknown path type");
    ret = -1;
    break;
  }
  if (ret == 0) run_event(new, KR_STUDIO_ADD);
  if (ret) {
    kr_pool_release(studio->pool, new);
    printke("Studio: add failed %d", ret);
  }
  return ret;
}

int kr_studio_connect(kr_studio_path *p, kr_studio_path_info *i, void *user) {
  if (!p) return -1;
  return studio_add(p->studio, p, i, user);
}

int kr_studio_add(kr_studio *studio, kr_studio_path_info *info, void *user) {
  return studio_add(studio, NULL, info, user);
}

int kr_studio_destroy(kr_studio *studio) {
  int i;
  kr_studio_path *path;
  if (!studio) return -1;
  printk("Studio: Destroying");
  i = 0;
  while ((path = kr_pool_iterate_active(studio->pool, &i))) {
    kr_studio_remove(path);
  }
  kr_pool_destroy(studio->pool);
  printk("Studio: Destroyed");
  return 0;
}

kr_studio *kr_studio_create(kr_studio_setup *setup) {
  kr_studio *studio;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (!setup) return NULL;
  if (!setup->user) return NULL;
  printk("Studio: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*studio);
  pool_setup.size = sizeof(kr_studio_path);
  pool_setup.slices = setup->npaths;
  pool = kr_pool_create(&pool_setup);
  if (!pool) return NULL;
  studio = kr_pool_overlay_get(pool);
  studio->pool = pool;
  studio->user = setup->user;
  studio->mixer = setup->mixer;
  studio->compositor = setup->compositor;
  studio->xfer = setup->xfer;
  studio->stash = setup->stash;
  studio->tracker = setup->tracker;
  studio->event_cb = setup->event_cb;
  printk("Studio: Created");
  return studio;
}
