#if !defined(_ops_media_pool_H)
#define _ops_media_pool_H (1)

typedef struct kr_media_pool kr_media_pool;

#include <krad/ops/media/media.h>

typedef struct {
  int nmedia;
  kr_loop *loop;
  kr_media_event_cb *event_cb;
} kr_media_pool_setup;

typedef struct {
  void *user;
} kr_media_setup;

int kr_media_pool_run_event(kr_media_pool *pool, kr_media_event *event);

kr_media *kr_media_find(kr_media_pool *pool, uuid_t *id);
kr_media *kr_media_pool_iterate(kr_media_pool *pool, int *iter);
int kr_media_pool_active(kr_media_pool *pool);
int kr_media_destroy(kr_media_pool *pool, kr_media *media);
kr_media *kr_media_create(kr_media_pool *pool, kr_media_setup *setup);
int kr_media_pool_destroy(kr_media_pool *pool);
kr_media_pool *kr_media_pool_create(kr_media_pool_setup *setup);

#endif
