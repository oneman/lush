#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <krad/app/debug.h>
#include "pack.h"
#include "crc.h"

#define FLAG_NONE 0x00
#define FLAG_CONT 0x01
#define FLAG_BOS 0x02
#define FLAG_EOS 0x04
#define OFFSET_VERSION 4
#define OFFSET_FLAGS 5
#define OFFSET_GRANULEPOS 6
#define OFFSET_SERIAL 14
#define OFFSET_SEQ 18
#define OFFSET_CRC 22
#define OFFSET_SEGMENTS 26
#define OFFSET_LACING 27
#define SEG_SZ 255
#define PAGE_HEADER_SZ 27
#define PAGE_NSEGS 255
#define PAGE_DATA_SZ SEG_SZ * PAGE_NSEGS
#define PAGE_MAX_SZ PAGE_HEADER_SZ + PAGE_NSEGS + PAGE_DATA_SZ

#define NHEADERS_MAX KR_OGG_MUXER_NTRACKS * 3 * 2 * 2

typedef struct {
  int flags;
  int64_t granule_pos;
  uint32_t serial;
  uint32_t seq;
  kr_iov packet[KR_OGG_MUXER_VECLEN];
  int len;
} page_info;

typedef struct {
  int64_t granule_pos;
  uint32_t serial;
  uint32_t seq;
} ogg_track;

struct kr_ogg_muxer {
  ogg_track track[KR_OGG_MUXER_NTRACKS];
  int tracks;
  kr_codec_header codec_header[KR_OGG_MUXER_NTRACKS];
  uint8_t header_data[8192];
  kr_iov header[NHEADERS_MAX];
  int header_len;
};

static void run_crc(uint32_t *crc, uint8_t *data, size_t len) {
  int i;
  uint32_t c;
  c = *crc;
  for (i = 0; i < len; i++) {
    c = (c<<8)^crc_lookup[((c >> 24)&0xFF)^(*data++)];
  }
  *crc = c;
}

static ssize_t gen_page(uint8_t *p, size_t sz, page_info *info) {
  int i;
  int s;
  uint32_t crc;
  int segments;
  size_t packet_sz;
  size_t header_sz;
  s = 0;
  crc = 0;
  segments = 0;
  for (i = 0; i < info->len; i++) {
    segments += (info->packet[i].sz / SEG_SZ) + 1;
  }
  if (segments > PAGE_NSEGS) segments = PAGE_NSEGS;
  header_sz = PAGE_HEADER_SZ + segments;
  if (header_sz > sz) return -1;
  memcpy(p, "OggS", 4);
  p[OFFSET_VERSION] = 0;
  p[OFFSET_FLAGS] = info->flags;
  memcpy(p + OFFSET_GRANULEPOS, &info->granule_pos, 8);
  memcpy(p + OFFSET_SERIAL, &info->serial, 4);
  memcpy(p + OFFSET_SEQ, &info->seq, 4);
  memset(p + OFFSET_CRC, 0, sizeof(crc));
  p[OFFSET_SEGMENTS] = segments;
  s = OFFSET_LACING;
  for (i = 0; i < info->len; i++) {
    packet_sz = info->packet[i].sz;
    if (packet_sz >= PAGE_DATA_SZ) {
      memset(p + OFFSET_LACING, 255, PAGE_NSEGS);
      break;
    }
    for (;;) {
      if (packet_sz >= 255) {
        p[s++] = 255;
        packet_sz -= 255;
      } else {
        p[s++] = packet_sz;
        break;
      }
    }
  }
  run_crc(&crc, p, header_sz);
  for (i = 0; i < info->len; i++) {
    packet_sz = info->packet[i].sz;
    if (packet_sz > PAGE_DATA_SZ) packet_sz = PAGE_DATA_SZ;
    run_crc(&crc, info->packet[i].buf, packet_sz);
  }
  memcpy(p + OFFSET_CRC, &crc, sizeof(crc));
  return header_sz;
}

static ssize_t mux_packets(kr_ogg_muxer *m, kr_ogg_muxer_data *d, int fpos,
 int in_pos, int len) {
  int i;
  ssize_t ret;
  ogg_track *t;
  page_info info;
  memset(&info, 0, sizeof(info));
  t = &m->track[d->track];
  info.flags = FLAG_NONE;
  info.serial = t->serial;
  info.seq = t->seq;
  info.len = len;
  for (i = 0; i < len; i++) {
    info.packet[i].buf = d->in[in_pos + i].buf;
    info.packet[i].sz = d->in[in_pos + i].sz;
  }
  info.granule_pos = d->in[in_pos + len - 1].granule_pos;
  if (t->seq == 0) info.flags = FLAG_BOS;
  ret = gen_page(d->framing_buf + fpos, d->sz - fpos, &info);
  if (ret > 0) {
    t->seq++;
    t->granule_pos = info.granule_pos;
  } else {
    printke("Ogg mux: gen page ret %zd", ret);
    return -1;
  }
  d->out[d->out_len].buf = d->framing_buf + fpos;
  d->out[d->out_len].sz = ret;
  d->out_len++;
  for (i = 0; i < len; i++) {
    d->out[d->out_len].buf = d->in[in_pos + i].buf;
    d->out[d->out_len].sz = d->in[in_pos + i].sz;
    d->out_len++;
  }
  return ret;
}

ssize_t mux_pages(kr_ogg_muxer *m, kr_ogg_muxer_data *d, int fpos, int in_pos) {
  ssize_t ret;
  size_t pos;
  ogg_track *t;
  page_info info;
  memset(&info, 0, sizeof(info));
  t = &m->track[d->track];
  info.flags = FLAG_NONE;
  info.serial = t->serial;
  info.len = 1;
  pos = 0;
  while (d->in[in_pos].sz - pos) {
    printk("position is %zu", pos);
    info.seq = t->seq;
    info.packet[0].buf = d->in[in_pos].buf + pos;
    info.packet[0].sz = d->in[in_pos].sz - pos;
    if (info.packet[0].sz >= PAGE_DATA_SZ) {
      info.granule_pos = -1;
    } else {
      info.granule_pos = d->in[in_pos].granule_pos;
    }
    if (pos >= PAGE_DATA_SZ) info.flags = FLAG_CONT;
    ret = gen_page(d->framing_buf + fpos, d->sz - fpos, &info);
    if (ret < 0) {
      printke("Ogg mux: gen page ret %zd", ret);
      return -1;
    }
    d->out[d->out_len].buf = d->framing_buf + fpos;
    d->out[d->out_len].sz = ret;
    d->out_len++;
    fpos += ret;
    d->out[d->out_len].buf = d->in[in_pos].buf + pos;
    if ((d->in[in_pos].sz - pos) >= PAGE_DATA_SZ) {
      d->out[d->out_len].sz = PAGE_DATA_SZ;
      pos += PAGE_DATA_SZ;
    } else {
      d->out[d->out_len].sz = d->in[in_pos].sz - pos;
      pos += d->out[d->out_len].sz;
    }
    d->out_len++;
    t->seq++;
  }
  t->granule_pos = d->in[in_pos].granule_pos;
  return fpos;
}

ssize_t kr_ogg_mux(kr_ogg_muxer *m, kr_ogg_muxer_data *d) {
  ssize_t ret;
  size_t pos;
  int i;
  int in_pos;
  int len;
  size_t sz;
  size_t total;
  static int nulled_out = 0;
  if (!m) {
    if (!nulled_out) {
      printke("Ogg mux called with null muxer");
      nulled_out++;
    }
    return -1;
  }
  if (!d) {
    if (!nulled_out) {
      printke("Ogg mux called with null data input");
      nulled_out++;
    }
    return -1;
  }
  if (d->track < 0 || d->track >= m->tracks) {
    printke("Ogg mux called with invalid track");
    return -2;
  }
  total = 0;
  pos = 0;
  in_pos = 0;
  len = 0;
  d->out_len = 0;
  printkd("Ogg muxing track %d: %d packets", d->track, d->in_len);
  for (;;) {
    in_pos += len;
    len = 0;
    sz = 0;
    for (i = in_pos; i < d->in_len; i++) {
      if (len && d->in[i].sz + sz >= PAGE_DATA_SZ) break;
      len++;
      sz += d->in[i].sz;
      total += d->in[i].sz;
    }
    if (!len) break;
    if (sz >= PAGE_DATA_SZ && len == 1) {
      /*printk(" N Pages for %zu byte packet", sz);*/
      ret = mux_pages(m, d, pos, in_pos);
      if (ret < 0) printke("Ogg muxer fail");
      pos += ret;
      continue;
    }
    if (sz < PAGE_DATA_SZ && len >= 1) {
      /*printk(" Page: %d packets %zu bytes", len, sz);*/
      ret = mux_packets(m, d, pos, in_pos, len);
      if (ret < 0) printke("Ogg muxer fail");
      pos += ret;
      continue;
    }
    printke("Ogg muxer: fucked up");
    exit(1);
  }
  /*printk(" Ogg muxed: %zu bytes in %d packets with %zu overhead.", total,
   d->in_len, pos);*/
  if (!pos) {
    printke("Ogg muxer: Unknown Kludge");
    pos++;
  }
  return pos;
}

ssize_t kr_ogg_muxer_read_header(kr_ogg_muxer *m, uint8_t *buf, size_t max) {
  int i;
  ssize_t ret;
  kr_iov *hdr;
  if (!m || !buf) return -1;
  ret = 0;
  for (i = 0; i < m->header_len; i++) {
    hdr = &m->header[i];
    if (ret + hdr->sz > max) return -2;
    memcpy(buf + ret, hdr->buf, hdr->sz);
    ret += hdr->sz;
  }
  return ret;
}

static int gen_header(kr_ogg_muxer *m) {
  size_t ret;
  int t;
  int h;
  int first;
  int last;
  kr_ogg_muxer_data od;
  m->header_len = 0;
  memset(&od, 0, sizeof(od));
  od.framing_buf = m->header_data;
  od.sz = sizeof(m->header_data);
  last = 1;
  for (first = 0; first < 2; first++) {
    for (t = 0; t < m->tracks; t++) {
      od.track = t;
      if (first > 0) last = m->codec_header[t].len;
      for (h = first; h < last; h++) {
        if (m->header_len == NHEADERS_MAX) return 0;
        od.in[h - first].buf = m->codec_header[t].data[h].buf;
        od.in[h - first].sz = m->codec_header[t].data[h].sz;
      }
      od.in_len = last - first;
      if (!od.in_len) continue;
      ret = kr_ogg_mux(m, &od);
      if (ret < 1) return 0;
      od.framing_buf += ret;
      od.sz -= ret;
      for (h = 0; h < od.out_len; h++) {
        m->header[m->header_len].buf = od.out[h].buf;
        m->header[m->header_len].sz = od.out[h].sz;
        m->header_len++;
      }
    }
  }
  return 1;
}

size_t kr_ogg_muxer_sizeof() {
  return sizeof(kr_ogg_muxer);
}

kr_ogg_muxer *kr_ogg_muxer_init(kr_ogg_muxer_setup *setup) {
  int t;
  kr_ogg_muxer *m;
  if (!setup || !setup->mem) return NULL;
  if (setup->tracks < 1 || setup->tracks > KR_OGG_MUXER_NTRACKS) return NULL;
  m = (kr_ogg_muxer *)setup->mem;
  m->tracks = setup->tracks;
  m->header_len = 0;
  for (t = 0; t < m->tracks; t++) {
    if (setup->header[t].len < 1) return NULL;
    m->codec_header[t] = setup->header[t];
    m->track[t].seq = 0;
    m->track[t].serial = rand() + time(NULL) * 45;
  }
  if (!gen_header(m)) return NULL;
  return m;
}
