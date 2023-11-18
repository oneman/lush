#include <math.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <krad/app/debug.h>
#include <krad/mem/pool.h>

static int slice_check(void *slice) {
  //printf("slice addres: %p\n", slice);
  if ((uint64_t)slice % 64) {
    fprintf(stderr, "Unaligned slice!\n");
    exit(1);
  }
  return 0;
}

static int pool_test(int sz, int len, int atomic) {
  int i;
  int ret;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  void *slice;
  slice = NULL;
  memset(&pool_setup, 0, sizeof(kr_pool_setup));
  pool_setup.shared = 1;
  pool_setup.size = sz;
  pool_setup.slices = len;
  pool = kr_pool_create(&pool_setup);
  if (pool == NULL) {
    fprintf(stderr, "failed to make pool\n");
    return 1;
  }
  kr_pool_debug(pool);
  for (i = 0; i < len; i++) {
    if (atomic) {
      slice = kr_pool_atomic_slice(pool);
    } else {
      slice = kr_pool_slice(pool);
    }
    if (slice == NULL) {
      fprintf(stderr, "failed to get slice\n");
      return 1;
    }
    slice_check(slice);
    memcpy(slice, "coconut", 8);
  }
  i = 0;
  while ((slice = kr_pool_iterate_active(pool, &i))) {
    //printf("%s\n", (char *)aslice);
  }
  kr_pool_debug(pool);
  i = 0;
  while ((slice = kr_pool_iterate_active(pool, &i))) {
    if (atomic) {
      ret = kr_pool_atomic_release(pool, slice);
    } else {
      ret = kr_pool_release(pool, slice);
    }
    if (ret) printke("Atomic: %d Ret: %d", atomic, ret);
  }
  kr_pool_debug(pool);
  kr_pool_destroy(pool);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  int slices;
  int slice_sz;
  int i;
  ret = 0;
  srand(time(NULL));
  for (i = 0; i < 1; i++) {
    slices = 1 + rand() % 30;
    slice_sz = rand() % 1024 + 8;
    printk("Pool Test Reg\n\n");
    ret = pool_test(slice_sz, slices, 0);
    if (ret) break;
    printk("\n\n");
    printk("Pool Test Atomic\n\n");
    ret = pool_test(slice_sz, slices, 1);
    if (ret) break;
    printk("\n\n");
  }
  return ret;
}
