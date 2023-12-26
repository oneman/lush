#include "cross.c"

void deepcut(uint8_t *bs, size_t sz) {
  size_t i;
  size_t start;
  size_t len;
  i = 0;
  start = 0;
  len = 0;
  printf("len %lu\n", sz);
  for (i = 0; i < sz; i++) {
    if (bs[i] == LF) {
      len = i - start;
      if (len) crosscut(bs + start, len);
      start = i + 1;
    }
  }
}
