#include <stdlib.h>
#include <stdint.h>

int kr_strtou64(uint64_t *u, char *s, size_t sz) {
  uint64_t sum;
  sum = 0;
  /* if (sz > 20) return -1;*/
  if (!sz || sz > 19) return -1; /* I'm saying nuff */
  while (*s >= '0' && *s <= '9' && sz--) {
    sum = sum * 10 + (*s - '0');
    s++;
  }
  if (sz) return -1;
  *u = sum;
  return 0;
}

int kr_strtoi64(int64_t *i, char *s, size_t sz) {
  int ret;
  uint64_t u;
  if (!sz || sz > 20) return -1; /* I'm saying nuff */
  if (*s == '-') {
    ret = kr_strtou64(&u, ++s, --sz);
    if (ret) return ret;
    if (u > 9223372036854775808ULL) return -1;
    *i = u * -1;
    return 0;
  }
  if (sz > 19) return -1; /* I'm saying nuff */
  ret = kr_strtou64(&u, s, sz);
  if (ret) return ret;
  if (u > 9223372036854775807) return -1;
  *i = u;
  return 0;
}

int kr_strtou32(uint32_t *u, char *s, size_t sz) {
  int ret;
  uint64_t uu;
  if (!sz || sz > 10) return -1;
  ret = kr_strtou64(&uu, s, sz);
  if (ret) return ret;
  if (uu > 4294967295) return -1;
  *u = uu;
  return 0;
}

int kr_strtoi32(int32_t *i, char *s, size_t sz) {
  int ret;
  int64_t ii;
  if (!sz || sz > 11) return -1;
  ret = kr_strtoi64(&ii, s, sz);
  if (ret) return ret;
  if (ii > 2147483647) return -1;
  if (ii < -2147483648) return -1;
  *i = ii;
  return 0;
}

int kr_strtou16(uint16_t *u, char *s, size_t sz) {
  int ret;
  uint64_t uu;
  if (!sz || sz > 5) return -1;
  ret = kr_strtou64(&uu, s, sz);
  if (ret) return ret;
  if (uu > 65535) return -1;
  *u = uu;
  return 0;
}

int kr_strtoi16(int16_t *i, char *s, size_t sz) {
  int ret;
  int64_t ii;
  if (!sz || sz > 6) return -1;
  ret = kr_strtoi64(&ii, s, sz);
  if (ret) return ret;
  if (ii > 32767) return -1;
  if (ii < -32768) return -1;
  *i = ii;
  return 0;
}

int kr_strtou8(uint8_t *u, char *s, size_t sz) {
  int ret;
  uint64_t uu;
  if (!sz || sz > 3) return -1;
  ret = kr_strtou64(&uu, s, sz);
  if (ret) return ret;
  if (uu > 256) return -1;
  *u = uu;
  return 0;
}

int kr_strtoi8(int8_t *i, char *s, size_t sz) {
  int ret;
  int64_t ii;
  if (!sz || sz > 4) return -1;
  ret = kr_strtoi64(&ii, s, sz);
  if (ret) return ret;
  if (ii > 127) return -1;
  if (ii < -128) return -1;
  *i = ii;
  return 0;
}
