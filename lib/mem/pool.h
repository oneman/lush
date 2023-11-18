#if !defined(_mem_pool_H)
# define _mem_pool_H (1)

#define KR_PAGESIZE 4096

typedef struct kr_pool kr_pool;

#include <inttypes.h>
#include <stddef.h>

typedef struct {
  uint32_t slices;
  size_t size;
  int shared;
  void *overlay;
  size_t overlay_sz;
} kr_pool_setup;

int kr_pool_fd(kr_pool *pool);
int kr_pool_size(kr_pool *pool);
int kr_pool_slice_size(kr_pool *pool);
int kr_pool_offsetof(kr_pool *pool, void *slice);
void *kr_pool_iterate_active(kr_pool *pool, int *count);
int kr_pool_slice_ref(kr_pool *pool, void *slice);
int kr_pool_release(kr_pool *pool, void *slice);
int kr_pool_atomic_release(kr_pool *pool, void *slice);
void *kr_pool_atomic_slice(kr_pool *pool);
void *kr_pool_slice(kr_pool *pool);
void *kr_pool_slice_num(kr_pool *pool, int num);
int kr_pool_avail(kr_pool *pool);
int kr_pool_active(kr_pool *pool);
int kr_pool_slices(kr_pool *pool);
int kr_pool_overlay_get_copy(kr_pool *pool, void *overlay);
void *kr_pool_overlay_get(kr_pool *pool);
int kr_pool_overlay_set(kr_pool *pool, void *overlay);
int kr_pool_destroy(kr_pool *pool);
kr_pool *kr_pool_open(int fd, kr_pool_setup *setup);
kr_pool *kr_pool_create(kr_pool_setup *setup);

void kr_pool_debug(kr_pool *pool);

#endif
