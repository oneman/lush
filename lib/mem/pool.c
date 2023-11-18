#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <assert.h>
#include <signal.h>
/*#include <sys/memfd.h>*/
#include <krad/app/spinlock.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include "pool.h"

#define KR_POOL_MAX 1048576
#define KR_POOL_64 64
#define CACHELINE_SZ KR_POOL_64
#define MM __ATOMIC_SEQ_CST

struct kr_pool {
  uint64_t *use;
  uint8_t *ref;
  void *data;
  uint32_t slices;
  uint32_t active;
  void *map;
  void *overlay;
  size_t slice_size;
  size_t info_size;
  size_t total_size;
  size_t overlay_sz;
  size_t overlay_actual_sz;
  size_t internal_sz;
  uint32_t use_max;
  uint8_t shared;
  int fd;
  kr_spinlock lock;
};

static kr_pool *pool_map(int fd, kr_pool_setup *setup);

int kr_pool_fd(kr_pool *pool) {
  if (pool == NULL) return -1;
  if (pool->shared != 1) return -1;
  return pool->fd;
}

int kr_pool_size(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->total_size;
}

int kr_pool_slice_size(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->slice_size;
}

int kr_pool_offsetof(kr_pool *pool, void *slice) {
  if (pool == NULL) return -1;
  if (slice == NULL) return -1;
  /* FIXME check that slice is in pool */
  return slice - pool->map;
}

int kr_pool_avail(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->slices - pool->active;
}

int kr_pool_active(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->active;
}

int kr_pool_slices(kr_pool *pool) {
  if (pool == NULL) return -1;
  return pool->slices;
}

void *kr_pool_iterate_active(kr_pool *pool, int *count) {
  uint64_t mask;
  int n;
  int i;
  int iter;
  if ((pool == NULL) || (count == NULL)) return NULL;
  if ((*count < 0) || (*count >= pool->slices)) return NULL;
  mask = 1;
  i = 0;
  iter = 0;
  n = *count;
  if (*count >= KR_POOL_64) {
    i = *count / KR_POOL_64;
    n = n % KR_POOL_64;
  }
  mask = mask << n;
  while (*count < pool->slices) {
    if ( (iter > 0) && ((*count % KR_POOL_64) == 0) ) {
      mask = 1;
      i++;
    }
    if ((pool->use[i] & mask) != 0) {
      return pool->data + (pool->slice_size * (*count)++);
    } else {
      (*count)++;
      mask = mask << 1;
    }
    iter++;
  }
  (*count) = 0;
  return NULL;
}

void *kr_pool_slice_num(kr_pool *pool, int num) {
  int i;
  int n;
  uint64_t mask;
  if (pool == NULL) return NULL;
  if ((num < 0) || (num >= pool->slices)) return NULL;
  mask = 1;
  i = 0;
  n = num;
  if (num >= KR_POOL_64) {
    i = num / KR_POOL_64;
    n = num % KR_POOL_64;
  }
  mask = mask << n;
  if ((pool->use[i] & mask) != 0) {
    return pool->data + (pool->slice_size * num);
  } else {
    pool->use[i] = pool->use[i] | mask;
    pool->active++;
    return pool->data + (pool->slice_size * num);
  }
}

/*
void *kr_pool_iterate_state(kr_pool *pool, int *count) {

  return NULL;
}

void *kr_pool_iterate_type(kr_pool *pool, int *count) {

  return NULL;
}

void *kr_pool_iterate_type_state(kr_pool *pool, int *count) {
  return NULL;
}
*/

void *kr_pool_overlay_get(kr_pool *pool) {
  if (pool == NULL) return NULL;
  if (pool->overlay == NULL) return NULL;
  if (pool->overlay_sz == 0) return NULL;
  return pool->overlay;
}

int kr_pool_overlay_set(kr_pool *pool, void *overlay) {
  if (pool == NULL) return -1;
  if (pool->overlay_sz == 0) return -2;
  memcpy(pool->overlay, overlay, pool->overlay_sz);
  return 0;
}

int kr_pool_overlay_get_copy(kr_pool *pool, void *overlay) {
  if ((pool == NULL) || (overlay == NULL)) return -2;
  if (pool->overlay == NULL) return -3;
  memcpy(overlay, pool->overlay, pool->overlay_sz);
  return pool->overlay_sz;
}

int kr_pool_release(kr_pool *pool, void *slice) {
  uint64_t mask;
  void *last;
  int i;
  size_t num;
  if ((pool == NULL) || (slice == NULL)) return -2;
  last = pool->data + (pool->slice_size * (pool->slices - 1));
  if (slice < pool->data || slice > last) {
   return -3;
  }
  kr_spin_lock(&pool->lock);
  num = (slice - pool->data) / pool->slice_size;
  i = 0;
  if (num >= KR_POOL_64) {
    i = num / KR_POOL_64;
    num = num % KR_POOL_64;
  }
  mask = (uint64_t)1 << num;
  if (((pool->use[i] & mask) != 0)) {
      pool->use[i] = pool->use[i] ^ mask;
      pool->active--;
      kr_spin_unlock(&pool->lock);
      return 0;
  }
  kr_spin_unlock(&pool->lock);
  return -1;
}

int kr_pool_slice_ref(kr_pool *pool, void *slice) {
  int i;
  int j;
  uint64_t mask;
  if ((pool == NULL) || (slice == NULL)) return -2;
  mask = 1;
  for (i = j = 0; i < pool->slices; i++) {
    if ( (i > 0) && ((i % KR_POOL_64) == 0)) {
      mask = 1;
      j++;
    }
    if (((pool->use[j] & mask) != 0)
        && (slice == (pool->data + (pool->slice_size * i)))) {
      pool->ref[i]++;
      return pool->ref[i];
    }
    mask = mask << 1;
  }
  return -1;
}

int kr_pool_atomic_release(kr_pool *pool, void *slice) {
  uint64_t mask;
  int i;
  size_t num;
  if ((pool == NULL) || (slice == NULL)) return -2;
  num = (slice - pool->data) / pool->slice_size;
  i = 0;
  if (num >= KR_POOL_64) {
    i = num / KR_POOL_64;
    num = num % KR_POOL_64;
  }
  mask = (uint64_t)1 << num;
  __atomic_xor_fetch(&pool->use[i], mask, MM);
  __atomic_sub_fetch(&pool->active, 1, MM);
  return 0;
}

void *kr_pool_atomic_slice(kr_pool *pool) {
  int i;
  int j;
  int ix;
  int len;
  uint64_t use;
  uint64_t new;
  uint64_t mask;
  if (pool == NULL) return NULL;
  for (i = 0; i < pool->slices; i += 64) {
    mask = 1;
    len = 64;
    if ((i + len) > pool->slices) len = pool->slices - i;
    ix = i / 64;
    __atomic_load(&pool->use[ix], &use, MM);
    for (j = 0; j < len; j++) {
      if ((use & mask) == 0) {
        new = use | mask;
        if (__atomic_compare_exchange(&pool->use[ix], &use, &new, 0, MM, MM)) {
          __atomic_add_fetch(&pool->active, 1, MM);
          return pool->data + (pool->slice_size * (i + j));
        } else {
          i -= 64;
          break;
        }
      }
      mask = mask << 1;
    }
  }
  return NULL;
}

void *kr_pool_slice(kr_pool *pool) {
  int i;
  int j;
  uint64_t mask;
  if (pool == NULL) return NULL;
  mask = 1;
  kr_spin_lock(&pool->lock);
  for (i = j = 0; i < pool->slices; i++) {
    if ( (i > 0) && ((i % KR_POOL_64) == 0)) {
      mask = 1;
      j++;
    }
    if ((pool->use[j] & mask) == 0) {
      pool->use[j] = pool->use[j] | mask;
      pool->active++;
      kr_spin_unlock(&pool->lock);
      return pool->data + (pool->slice_size * i);
    }
    mask = mask << 1;
  }
  kr_spin_unlock(&pool->lock);
  return NULL;
}

void kr_pool_debug(kr_pool *pool) {
  int i;
  if (pool == NULL) return;
  printk("\npool info");
  printk("pool slices: %d", kr_pool_slices(pool));
  printk("pool active: %d", kr_pool_active(pool));
  printk("pool avail: %d", kr_pool_avail(pool));
  printk("pool use_max: %d", pool->use_max);
  if (pool->use_max > 1) {
    for (i = 0; i < pool->use_max; i++) {
      if (pool->use[i])
        printk("pool use[%d]: %"PRIu64"", i, pool->use[i]);
    }
  } else {
    printk("pool use[0]: %"PRIu64"", pool->use[0]);
  }
  printk("pool info size: %zu", sizeof(kr_pool));
  printk("pool info act size: %zu", pool->info_size);
  printk("pool overlay size: %zu", pool->overlay_sz);
  printk("pool overlay act size: %zu", pool->overlay_actual_sz);
  printk("pool slice size: %zu", pool->slice_size);
  printk("pool slices size: %zu", pool->slice_size * pool->slices);
  printk("pool internal size: %zu", pool->internal_sz);
  printk("pool total size: %zu\n", pool->total_size);
}

int kr_pool_destroy(kr_pool *pool) {
  int ret;
  if (pool == NULL) return -1;
  /*kr_pool_debug(pool);*/
  if (pool->shared == 1) {
    close(pool->fd);
  }
  ret = munlock(pool->map, pool->total_size);
  if (ret) printke("Pool munlock: ret %d", ret);
  ret = munmap(pool->map, pool->total_size);
  if (ret) printke("Pool destroy: ret %d", ret);
  return ret;
}

static kr_pool *pool_map(int fd, kr_pool_setup *setup) {
  char filename[] = "/tmp/krad-shm-XXXXXX";
  static const int prot = PROT_READ | PROT_WRITE;
  int flags;
  int mod;
  int ret;
  kr_pool pool;
  if (setup == NULL) return NULL;
  if (setup->slices == 0) return NULL;
  if (setup->slices > KR_POOL_MAX) return NULL;
  if (setup->size == 0) return NULL;
  memset(&pool, 0, sizeof(kr_pool));
  pool.overlay_sz = setup->overlay_sz;
  pool.overlay_actual_sz = pool.overlay_sz;
  pool.info_size = sizeof(kr_pool);
  mod = pool.info_size % CACHELINE_SZ;
  if (mod) {
    pool.info_size += CACHELINE_SZ - mod;
  }
  mod = pool.overlay_sz % CACHELINE_SZ;
  if (mod) {
    pool.overlay_actual_sz += CACHELINE_SZ - mod;
  }
  pool.slices = setup->slices;
  pool.slice_size = setup->size;
  mod = pool.slice_size % CACHELINE_SZ;
  if (mod) {
    pool.slice_size += CACHELINE_SZ - mod;
  }
  pool.use_max = (1 + ( (pool.slices - 1) / KR_POOL_64));
  pool.internal_sz = sizeof(uint64_t) * pool.use_max;
  pool.internal_sz += sizeof(uint8_t) * pool.slices;
  mod = pool.internal_sz % CACHELINE_SZ;
  if (mod) {
    pool.internal_sz += CACHELINE_SZ - mod;
  }
  pool.total_size = (pool.slices * pool.slice_size);
  pool.total_size += pool.info_size;
  pool.total_size += pool.overlay_actual_sz;
  pool.total_size += pool.internal_sz;
  mod = pool.total_size % KR_PAGESIZE;
  if (mod) {
    pool.total_size += KR_PAGESIZE - mod;
  }
  if (!setup->shared) {
    pool.shared = 0;
    pool.fd = -1;
    flags = MAP_PRIVATE | MAP_ANONYMOUS;
  } else {
    pool.shared = 1;
    flags = MAP_SHARED;
    if (fd != -1) {
      pool.fd = fd;
    } else {
      if (0) {
        /*pool.fd = memfd_create("kr_pool", MFD_CLOEXEC | MFD_ALLOW_SEALING);*/
      } else {
        pool.fd = mkostemp(filename, O_CLOEXEC);
        if (pool.fd < 0) {
          printke("Pool: open %s failed: %m\n", filename);
          return NULL;
        }
        unlink(filename);
      }
    }
    if (ftruncate(pool.fd, pool.total_size) < 0) {
      printke("ftruncate failed: %m\n");
      close(pool.fd);
      return NULL;
    }
  }
  pool.map = mmap(NULL, pool.total_size, prot, flags, pool.fd, 0);
  if (pool.map == MAP_FAILED) {
    printke("Pool: mmap");
    if (pool.shared) close(pool.fd);
    return NULL;
  }
  ret = mlock(pool.map, pool.total_size);
  if (ret) printke("Pool mlock: %s", strerror(ret));
  kr_mem_debug("pool", pool.total_size);
  pool.data = pool.map + pool.internal_sz;
  pool.data += (pool.info_size + pool.overlay_actual_sz);
  if (pool.overlay_sz != 0) {
    pool.overlay = pool.map + pool.internal_sz + pool.info_size;
    if (setup->overlay != NULL) {
      kr_pool_overlay_set(&pool, setup->overlay);
    }
  }
  pool.use = pool.map;
  pool.ref = pool.map + ( sizeof(uint64_t) * pool.slices );
  memcpy(pool.map + pool.internal_sz, &pool, sizeof(kr_pool));
  return (kr_pool *)(pool.map + pool.internal_sz);
}

kr_pool *kr_pool_open(int fd, kr_pool_setup *setup) {
  if (fd < 0 || setup->shared != 1) return NULL;
  return pool_map(fd, setup);
}

kr_pool *kr_pool_create(kr_pool_setup *setup) {
  return pool_map(-1, setup);
}
