#if !defined(_mem_mem_H)
# define _mem_mem_H (1)

#include <stddef.h>

#define KR_KB 1024
#define KR_MB (1000 * KR_KB)

/*#define KR_DEBUG_MEM 1*/

#define member_sizeof(type, member) sizeof(((type *)0)->member)

#define kr_amem(y, z) struct { uint8_t (y)[z];}__attribute__((aligned(8)));

#if defined(KR_DEBUG_MEM)
#include <krad/app/debug.h>
#define KR_MEMALLOCSTR "Mem: alloc on %s %d"
#define kr_alloc(x) kr_alloc_real(x); printk(KR_MEMALLOCSTR, __FILE__, __LINE__)
#define KR_MEMALLOCZSTR "Mem: allocz on %s %d"
#define kr_allocz(x, y) kr_allocz_real(x, y); printk(KR_MEMALLOCZSTR, __FILE__, __LINE__)
#else
#define kr_allocz kr_allocz_real
#define kr_alloc kr_alloc_real
#endif

void kr_mem_debug(const char *type, size_t sz);
void *kr_allocz_real(size_t nelem, size_t elsize);
void *kr_alloc_real(size_t size);

#endif
