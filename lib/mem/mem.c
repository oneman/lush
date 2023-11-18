#include <stdlib.h>
#include <krad/app/debug.h>
#include "mem.h"

#if defined(KR_DEBUG_MEM)
static size_t kr_mem_sz = 0;
#endif

void kr_mem_debug(const char *type, size_t sz) {
#if defined(KR_DEBUG_MEM)
  size_t total;
  total = __atomic_add_fetch(&kr_mem_sz, sz, __ATOMIC_SEQ_CST);
  if (sz < KR_KB) {
    printk("Mem: %s %zu Bytes", type, sz);
  } else if (sz < KR_MB) {
    printk("Mem: %s %zu KB", type, sz / KR_KB);
  } else {
    printk("Mem: %s %zu MB", type, sz / KR_MB);
  }
  printk("Mem: total %zu MB", total / KR_MB);
#endif
}

void *kr_allocz_real(size_t nelem, size_t elsize) {
  kr_mem_debug("allocZ", nelem * elsize);
  return calloc(nelem, elsize);
}

void *kr_alloc_real(size_t size) {
  kr_mem_debug("alloc", size);
  return malloc(size);
}
