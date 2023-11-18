#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <krad/app/debug.h>
#include "unpack.h"

#define FLAC_HDR_MIN_SZ 4 + 4 + 34
#define FLAC_BLOCK_HDR_SZ 4
#define FLAC_FRAME_HDR_MIN_SZ 6
#define FLAC_FRAME_SYNC_LENGTH 2

struct kr_flac_demuxer {
  uint8_t header[1024 * 1024];
  size_t header_sz;
  kr_flac_info info;
};

typedef struct {
  uint8_t last;
  uint8_t type;
  uint len;
} block_hdr;

typedef struct {
  int blocksize;
  size_t len;
} frame_hdr;

#include "crc.c"

static uint8_t get_crc8(uint8_t *buf, size_t len) {
  uint8_t crc;
  crc = 0;
  while (len--) crc = crc8_table[crc ^ *buf++];
  return crc;
}

static uint16_t get_crc16(uint8_t *buf, size_t len) {
  uint16_t crc;
  crc = 0;
  while (len--) {
    crc = ((crc << 8) ^ crc16_table[(crc >> 8) ^ *buf++]) & 0xffff;
  }
  return crc;
}

static uint8_t utf8_sz(uint8_t byte) {
  int i;
  uint8_t sz;
  sz = 0;
  for (i = 7; i > 0; i--) {
    if (!((byte >> i) & 0x01)) break;
    sz++;
  }
  if (sz > 0) sz--;
  return sz;
}

static uint64_t read_uint_be(uint8_t *buf, uint8_t len) {
  int i;
  uint64_t val;
  if (!buf || len > (sizeof(val))) return -1;
  val = 0;
  for (i = 0; i < len; i++) {
    val |= buf[i] << (8 * (len - i - 1));
  }
  return val;
}

static int flac_parse_block_hdr(block_hdr *hdr, uint8_t *buf) {
  hdr->last = buf[0] >> 7;
  hdr->type = buf[0] & 0x7F;
  hdr->len = read_uint_be(buf + 1, 3);
  return 0;
}

static int flac_get_header(kr_flac_demuxer *d, uint8_t *buf, size_t len) {
  block_hdr hdr;
  if (len < FLAC_HDR_MIN_SZ) return -1;
  if (memcmp(buf, "fLaC", 4)) return -2;
  memset(&hdr, 0, sizeof(hdr));
  flac_parse_block_hdr(&hdr, buf + 4);
  if (hdr.type != 0) return -3;
  if (hdr.len != 34) return -4;
  d->info.sample_rate = read_uint_be(buf + 18, 3) >> 4;
  d->info.channels = ((read_uint_be(buf + 20, 1) >> 1) & 0x7) + 1;
  d->info.bit_depth = (((buf[20] & 1) << 4) | (buf[21] >> 4)) + 1;
  d->header_sz = FLAC_HDR_MIN_SZ;
  for (;;) {
    if (hdr.last) break;
    if (d->header_sz + 4 > len) return -5;
    memset(&hdr, 0, sizeof(hdr));
    flac_parse_block_hdr(&hdr, buf + d->header_sz);
    printkd("FLAC Demuxer: found block hdr of type %u", hdr.type);
    if (hdr.type > 6) return -6;
    d->header_sz += FLAC_BLOCK_HDR_SZ;
    d->header_sz += hdr.len;
  }
  if (d->header_sz > len) {
    printke("FLAC Demuxer: need more buffer to read whole header");
    return -7;
  }
  if (d->header_sz > sizeof(d->header)) {
    printke("FLAC Demuxer: header is too big for me!");
    return -8;
  }
  memcpy(d->header, buf, d->header_sz);
  return 0;
}

static int flac_frame_check_sync(uint8_t *buf) {
  if (buf[0] == 0xFF && (buf[1] >> 1) == 0x7C) return 1;
  return 0;
}

static ssize_t flac_frame_header_parse(uint8_t *buf, size_t len) {
  uint8_t val;
  uint8_t crc;
  size_t frame_hdr_len;
  frame_hdr_len = FLAC_FRAME_HDR_MIN_SZ;
  if (len < FLAC_FRAME_HDR_MIN_SZ) return 0;
  val = buf[2] >> 4;
  if (val == 0) return -1;
  else if (val == 6) frame_hdr_len += 1;
  else if (val == 7) frame_hdr_len += 2;
  val = buf[2] & 0x0F;
  if (val == 12) frame_hdr_len += 1;
  else if (val == 13 || val == 14) frame_hdr_len += 2;
  else if (val == 15) return -2;
  val = buf[3] >> 4;
  if (val >= 11) return -3;
  val = (buf[3] >> 1) & 0x07;
  if (val == 0x03 || val == 0x07) return -4;
  val = buf[3] & 0x01;
  if (val) return -5;
  val = utf8_sz(buf[4]);
  frame_hdr_len += val;
  if (len < frame_hdr_len) return 0;
  crc = get_crc8(buf, frame_hdr_len - 1);
  if (crc != buf[frame_hdr_len - 1]) return -6;
  return frame_hdr_len;
}

static ssize_t flac_frame_scan(uint8_t *buf, size_t len, size_t *read) {
  int i;
  if (!read) return -1;
  *read = 0;
  if (!buf) return -2;
  if (len < FLAC_FRAME_SYNC_LENGTH) return -3;
  for (i = 0; i <= (len - FLAC_FRAME_SYNC_LENGTH); i++) {
    if (flac_frame_check_sync(buf + i)) {
      *read = i + FLAC_FRAME_SYNC_LENGTH;
      return i;
    }
  }
  *read = i;
  return -4;
}

int kr_flac_demux(kr_flac_demuxer *d, kr_flac_data *data) {
  int i;
  int ret;
  size_t rd;
  size_t len;
  uint8_t *hdr;
  uint8_t *buf;
  uint16_t crc;
  if (!d || !data) return -1;
  if (!data->in.buf || !data->in.len) return -2;
  data->read = 0;
  len = data->in.len;
  buf = data->in.buf;
  if (len < FLAC_FRAME_HDR_MIN_SZ) return 0;
  if (!flac_frame_check_sync(buf)) return -3;
  ret = flac_frame_header_parse(buf, len);
  if (ret < 0) return -4;
  if (ret == 0) return 0;
  hdr = buf;
  len -= ret;
  buf += ret;
  if (len < FLAC_FRAME_HDR_MIN_SZ) return 0;
  for (i = 0;;) {
    ret = flac_frame_scan(buf, len, &rd);
    if (ret < 0) {
      buf = hdr;
      break;
    }
    len -= ret;
    buf += ret;
    crc = get_crc16(hdr, buf - hdr - 2);
    if (crc != be16toh(*(uint16_t *)(buf - 2))) {
      len--;
      buf++;
      continue;
    }
    ret = flac_frame_header_parse(buf, len);
    if (ret < 0) {
      len--;
      buf++;
      continue;
    }
    if (ret == 0) break;
    data->out.frames[i].buf = hdr;
    data->out.frames[i].len = buf - hdr;
    hdr = buf;
    len -= ret;
    buf += ret;
    i++;
  }
  data->out.len = i;
  data->read = buf - data->in.buf;
  return 0;
}

size_t kr_flac_demuxer_sizeof() {
  return sizeof(kr_flac_demuxer);
}

int kr_flac_demuxer_get_info(kr_flac_demuxer *d, kr_flac_info *info) {
  if (!d || !info) return -1;
  *info = d->info;
  return 0;
}

kr_flac_demuxer *kr_flac_demuxer_init(kr_flac_demuxer_setup *setup) {
  int ret;
  kr_flac_demuxer *d;
  if (!setup || !setup->mem) return NULL;
  if (!setup->buf || !setup->len) return NULL;
  d = (kr_flac_demuxer *)setup->mem;
  memset(d, 0, sizeof(kr_flac_demuxer));
  ret = flac_get_header(d, setup->buf, setup->len);
  if (ret != 0) {
    printke("FLAC Demuxer: flac_get_header failed %d", ret);
    return NULL;
  }
  return d;
}

ssize_t kr_flac_unpack_header(kr_flac_demuxer *d, uint8_t *buf, size_t max) {
  if (!d || !buf || !max) return -1;
  if (max < d->header_sz) {
    printke("FLAC Demuxer: header is %zu but given buffer is only %zu",
     d->header_sz, max);
    return -2;
  }
  memcpy(buf, d->header, d->header_sz);
  return d->header_sz;
}
