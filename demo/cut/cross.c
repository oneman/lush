#include "com.c"

void crosscut(uint8_t *bs, size_t sz) {
  size_t i;
  size_t start;
  size_t len;
  i = 0;
  start = 0;
  len = 0;
  printf(" line len %lu\n", sz);
  for (i = 0; i < sz; i++) {
    len = i - start;
    if (bs[i] == '/') {
      if (i == 0) {
        start = i + 1;
        continue;
      }
      if (!len) {
        /*printf("nothing!\n");*/
        start = i + 1;
        continue;
      }
      comcut(bs + start, len);
      start = i + 1;
      continue;
    }
  }
  len = i - start;
  if (len) comcut(bs + start, len);
  printf("\n");
}
