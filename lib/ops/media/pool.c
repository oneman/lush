#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <krad/mem/pool.h>
#include <krad/loop/loop.h>
#include "types.h"
#include "media.h"
#include "pool.h"

struct kr_media_pool {
  kr_pool *pool;
  kr_loop *loop;
  kr_media_event_cb *event_cb;
};

int kr_media_pool_run_event(kr_media_pool *pool, kr_media_event *event) {
  if (!pool) return -1;
  return pool->event_cb(event);
}

kr_media *kr_media_find(kr_media_pool *pool, uuid_t *id) {
  int i;
  int ret;
  uuid_t *media_id;
  kr_media *media;
  i = 0;
  if (kr_pool_active(pool->pool) > 0) {
    while ((media = kr_pool_iterate_active(pool->pool, &i))) {
      media_id = kr_media_get_id(media);
      ret = uuid_compare(*media_id, *id);
      if (ret == 0) return media;
    }
  }
  return NULL;
}

kr_media *kr_media_pool_iterate(kr_media_pool *pool, int *iter) {
  kr_media *media;
  media = kr_pool_iterate_active(pool->pool, iter);
  return media;
}

int kr_media_pool_active(kr_media_pool *pool) {
  return kr_pool_active(pool->pool);
}

int kr_media_destroy(kr_media_pool *pool, kr_media *media) {
  int ret;
  ret = kr_media_clear(media);
  if (ret != 0) {
    printke("Media: failed media-specific destroy");
  }
  ret = kr_pool_release(pool->pool, media);
  return ret;
}

kr_media *kr_media_create(kr_media_pool *pool, kr_media_setup *setup) {
  int ret;
  kr_media *media;
  media = kr_pool_slice(pool->pool);
  if (media == NULL) {
    printke("Ops: cannot create new media.");
    return NULL;
  }
  ret = kr_media_init(media, pool, setup);
  if (ret == 0) return media;
  kr_pool_release(pool->pool, media);
  return NULL;
}

int kr_media_pool_destroy(kr_media_pool *pool) {
  if (!pool) return -1;
  kr_pool_destroy(pool->pool);
  return 0;
}

kr_media_pool *kr_media_pool_create(kr_media_pool_setup *setup) {
  kr_media_pool *media_pool;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (!setup) return NULL;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*media_pool);
  pool_setup.size = kr_media_sizeof();
  pool_setup.slices = setup->nmedia;
  pool = kr_pool_create(&pool_setup);
  media_pool = kr_pool_overlay_get(pool);
  memset(media_pool, 0, sizeof(*media_pool));
  media_pool->pool = pool;
  media_pool->loop = setup->loop;
  media_pool->event_cb = setup->event_cb;
  return media_pool;
}
