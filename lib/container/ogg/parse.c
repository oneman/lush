#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include "crc.h"
#include "parse.h"

#define OGG_CAPTURE "OggS"
#define OGG_OFFSET_CAPTURE 0
#define OGG_OFFSET_VERSION 4
#define OGG_OFFSET_FLAGS 5
#define OGG_OFFSET_GRANULEPOS 6
#define OGG_OFFSET_SERIALNO 14
#define OGG_OFFSET_SEQUENCENO 18
#define OGG_OFFSET_CRC 22
#define OGG_OFFSET_SEGMENTS 26
#define OGG_OFFSET_LACING 27
#define OGG_PAGE_MIN_HEADER_SIZE 27

static uint32_t ogg_page_checksum(uint8_t *data, size_t len) {
  int i;
  uint32_t crc;
  crc = 0;
  for (i = 0; i < len; i++) {
    if (i >= OGG_OFFSET_CRC && i < OGG_OFFSET_SEGMENTS) {
      crc = (crc<<8)^crc_lookup[((crc >> 24)&0xff)^0x00];
    } else {
      crc = (crc<<8)^crc_lookup[((crc >> 24)&0xff)^data[i]];
    }
  }
  return crc;
}

static int parse_page(kr_ogg_page *page, uint8_t *buf, size_t sz) {
  int i;
  size_t min_sz;
  min_sz = OGG_PAGE_MIN_HEADER_SIZE;
  if (sz < min_sz) return -45;
  if (memcmp(buf, OGG_CAPTURE, 4)) {
    printke("ogg parser: couldn't find capture!");
    return -2;
  }
  page->capture = buf;
  page->version = *(page->capture + OGG_OFFSET_VERSION);
  page->flags = *(page->capture + OGG_OFFSET_FLAGS);
  page->granulepos = *(uint64_t *)(page->capture + OGG_OFFSET_GRANULEPOS);
  page->serialno = *(uint32_t *)(page->capture + OGG_OFFSET_SERIALNO);
  page->sequenceno = *(uint32_t *)(page->capture + OGG_OFFSET_SEQUENCENO);
  page->crc = *(uint32_t *)(page->capture + OGG_OFFSET_CRC);
  page->segments = *(page->capture + OGG_OFFSET_SEGMENTS);
  page->lacing = page->capture + OGG_OFFSET_LACING;
  page->data = page->lacing + page->segments;
  page->continued = page->flags & 0x01;
  page->bos = (page->flags & 0x02) ? 1 : 0;
  page->eos = (page->flags & 0x04) ? 1 : 0;
  page->length = OGG_OFFSET_LACING + page->segments;
  page->data_length = 0;
  min_sz += page->segments;
  if (sz < min_sz) return -45;
  for (i = 0; i < page->segments; i++) {
    page->data_length += page->lacing[i];
    min_sz += page->lacing[i];
    if (sz < min_sz) {
      return -45;
    }
  }
  printkd("Expecting %zu bytes of data", page->data_length);
  page->length += page->data_length;
  if (page->crc != ogg_page_checksum(page->capture, page->length)) {
    printke("ogg parser: CRC32 failed!\n");
    return -3;
  }
  return 0;
}

ssize_t kr_ogg_scan(uint8_t *buf, size_t sz) {
  ssize_t i;
  if (sz < 4) return -1;
  for (i = 0; i < (sz - 4); i++) {
    if (!memcmp(buf + i, OGG_CAPTURE, 4)) return i;
  }
  return -1;
}

ssize_t kr_ogg_parse(kr_ogg_page *page, uint8_t *buf,
 size_t sz) {
  int ret;
  if (!page || !buf || !sz) return -1;
  memset(page, 0, sizeof(kr_ogg_page));
  ret = parse_page(page, buf, sz);
  return ret;
}

ssize_t kr_ogg_valid(uint8_t *buf, size_t sz) {
  int ret;
  int off;
  kr_ogg_page page;
  if (!buf || !sz) return -1;
  off = 0;
  memset(&page, 0, sizeof(page));
  off = kr_ogg_scan(buf, sz);
  if (off < 0) return -2;
  ret = parse_page(&page, buf + off, sz - off);
  if (ret != 0) return ret;
  return off;
}
