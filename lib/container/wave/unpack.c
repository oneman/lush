#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <krad/app/debug.h>
#include "unpack.h"

#define  READ_U32_LE(buf)  \
  (((buf)[3]<<24)|((buf)[2]<<16)|((buf)[1]<<8)|((buf)[0]&0xff))

#define  READ_U16_LE(buf)  \
  (((buf)[1]<<8)|((buf)[0]&0xff))

struct kr_wave_parser {
  uint8_t *hdr;
  uint8_t *data;
  kr_wave_info info;
};

size_t kr_wave_parser_sizeof() {
  return sizeof(kr_wave_parser);
}

int kr_wave_parse(kr_wave_parser *w, uint8_t *buf, size_t sz) {
  size_t chunk_sz;
  if (!w || !buf || !sz) return -1;
  if (sz < WAVE_HEADER_MIN_SIZE) {
    printke("wave: wrong buffer size"); return -2;
  }
  w->hdr = buf;
  if (memcmp(buf, "RIFF", 4)) {
    printke("wave: wrong header");
    return -3;
  }
  buf += 4;
  chunk_sz = READ_U32_LE(buf);
  buf += 4;
  if (memcmp(buf, "WAVE", 4)) {
    printke("wave: wrong header");
    return -4;
  }
  //if ( (chunk_sz - 4) != (sz - 12) ) {
  //  printke("wave: chunk sz mismatch");
  //  return -5;
  //}
  buf += 4;
  if (memcmp(buf, "fmt ", 4)) {
    printke("wave: fmt section missing");
    return -6;
  }
  buf += 4;
  chunk_sz = READ_U32_LE(buf);
  buf += 4;
  w->info.fmt = READ_U16_LE(buf);
  if (w->info.fmt == KR_WAVE_EXTENSIBLE) {
    w->info.extensible = 1;
    w->info.fmt = READ_U16_LE(buf + chunk_sz - 16);
  }
  w->info.audio.channels = READ_U16_LE(buf + 2);
  w->info.audio.sample_rate = READ_U32_LE(buf + 4);
  w->info.audio.bit_depth = READ_U16_LE(buf + 14);
  if ( ( buf - w->hdr + chunk_sz ) > sz) return -5;
  buf += chunk_sz;
  if (w->info.fmt != KR_WAVE_PCM) {
    if (memcmp(buf, "fact", 4)) {
      printke("wave: fmt != PCM and fact section missing");
      return -7;
    }
    buf += 12;
  }
  if (!memcmp(buf, "LIST", 4)) {
    buf += 4;
    chunk_sz = READ_U32_LE(buf);
    buf += 4;
    if ( ( buf - w->hdr + chunk_sz ) > sz) return -5;
    buf += chunk_sz;
  }
  if (memcmp(buf, "data", 4)) {
    printke("wave: missing data section");
    return -8;
  }
  buf += 4;
  chunk_sz = READ_U32_LE(buf);
  buf += 4;
  if ( ( buf - w->hdr) > sz) return -5;
  w->info.hdr_sz = buf - w->hdr;
  w->data = buf;
  w->info.data_sz = chunk_sz;
  return 0;
}

int kr_wave_parser_get_info(kr_wave_parser *w, kr_wave_info *info) {
  if (!w || !info) return -1;
  *info = w->info;
  return 0;
}

kr_wave_parser *kr_wave_parser_init(kr_wave_parser_setup *setup) {
  kr_wave_parser *w;
  if (!setup) return NULL;
  w = (kr_wave_parser *)setup->mem;
  memset(w, 0, sizeof(*w));
  return w;
}
