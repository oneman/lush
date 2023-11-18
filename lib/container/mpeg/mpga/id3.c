#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <krad/app/debug.h>
#include "id3.h"

#define ID3_HEADER_SIZE 10

struct kr_id3_parser {
  kr_id3_info info;
};

size_t kr_id3_parser_sizeof() {
  return sizeof(kr_id3_parser);
}

kr_id3_parser *kr_id3_parser_init(kr_id3_parser_setup *setup) {
  kr_id3_parser *p;
  if (!setup || !setup->mem) return NULL;
  p = (kr_id3_parser *)setup->mem;
  memset(p, 0, sizeof(kr_id3_parser));
  return p;
}

int kr_id3_parser_get_info(kr_id3_parser *p, kr_id3_info *info) {
  if (!p || !info) return -1;
  *info = p->info;
  return 0;
}

ssize_t kr_id3_scan(uint8_t *buf, size_t sz, size_t *read) {
  int i;
  if (!read) return -1;
  *read = 0;
  if (!buf) return -2;
  if (sz < 3) return -3;
  for (i = 0; i <= (sz - 3); i++) {
    if (!memcmp(buf + i, "ID3", 3)) {
      *read = i + 3;
      return i;
    }
  }
  *read = i;
  return -4;
}

int kr_id3_parse(kr_id3_parser *p, uint8_t *buf, size_t sz) {
  if (!p || !buf) return -1;
  if (sz < ID3_HEADER_SIZE) return -2;
  if (memcmp(buf, "ID3", 3)) return -3;
  p->info.version = buf[3];
  if (buf[4]) return -4;
  if (buf[5] & 0x0F) return -5;
  if ((buf[6] >> 7) || (buf[7] >> 7)) return -6;
  if ((buf[8] >> 7) || (buf[9] >> 7)) return -6;
  p->info.sz = buf[6] * (1 << 21) + buf[7] * (1 << 14) + buf[8] * (1 << 7) + buf[9];
  p->info.sz += ID3_HEADER_SIZE;
  return 0;
}
