#include "cross.c"

void deepcut(uint8_t *bs, size_t sz) {
  size_t i;
  for (i = 0; i < sz; i++) {
    if (bs[i] == LF) {
      crosscut(bs, i);
      bs = bs + i;
      i = 0;
      continue;
    }
  }
}
