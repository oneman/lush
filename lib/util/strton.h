#if !defined(_util_strton_H)
# define _util_strton_H (1)

#include <stdlib.h>
#include <stdint.h>

int kr_strtou64(uint64_t *u, char *s, size_t sz);
int kr_strtoi64(int64_t *i, char *s, size_t sz);
int kr_strtou32(uint32_t *u, char *s, size_t sz);
int kr_strtoi32(int32_t *i, char *s, size_t sz);
int kr_strtou16(uint16_t *u, char *s, size_t sz);
int kr_strtoi16(int16_t *i, char *s, size_t sz);
int kr_strtou8(uint8_t *u, char *s, size_t sz);
int kr_strtoi8(int8_t *i, char *s, size_t sz);

#endif
