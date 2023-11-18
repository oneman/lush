#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include "crc.h"
#include "unpack.h"

#define OGG_MAX_TRACKS 8

enum ogg_parsing_state {
  OGG_NEW_PAGE,
  OGG_IN_SEGMENTS
};

enum ogg_stream_state {
  OGG_IN_HEADERS,
  OGG_IN_STREAM
};

typedef struct {
  kr_ogg_page curr_page;
  uint8_t *input;
  size_t input_pos;
  size_t input_sz;
  uint64_t packets;
  int pstate;
  int sstate;
  uint8_t next_seg;
  uint8_t continued;
} ogg_stream_st;

typedef struct {
  uint32_t serial;
  uint8_t num;
  kr_codec_packet *curr_pkt;
  size_t pkt_pos;
  uint8_t *pkt_data;
  kr_codec codec;
  uint64_t npkts;
} ogg_track_depacketizer;

struct kr_ogg_unpacker {
  ogg_stream_st stream_st;
  ogg_track_depacketizer tracks[OGG_MAX_TRACKS];
  uint8_t ntracks;
  uint8_t out_pkts;
  uint8_t resync;
};

static ogg_track_depacketizer *get_track(kr_ogg_unpacker *u, uint32_t serial) {
  int i;
  for (i = 0; i < u->ntracks; i++) {
    if (u->tracks[i].serial == serial) return &u->tracks[i];
  }
  return NULL;
}

static kr_codec ogg_pkt_get_codec(uint8_t *pkt, size_t sz) {
  kr_codec c;
  memset(&c, 0, sizeof(c));
  c.type = 0;
  for (;;) {
    if (sz > 8 && !memcmp(pkt, "fishead\0", 8)) {
      break;
    }
    if (sz > 5 && !memcmp(pkt + 1, "FLAC", 4)) {
      c.type = KR_CODEC_AUDIO;
      c.audio = KR_AUDIO_CODEC_FLAC;
      break;
    }
    if (sz > 8 && !memcmp(pkt, "OpusHead", 8)) {
      c.type = KR_CODEC_AUDIO;
      c.audio = KR_AUDIO_CODEC_OPUS;
      break;
    }
    if (sz > 8 && pkt[0] == 0x01 && !memcmp(pkt + 1, "vorbis", 6)) {
      c.type = KR_CODEC_AUDIO;
      c.audio = KR_AUDIO_CODEC_VORBIS;
      break;
    }
    if (sz > 8 && pkt[0] == 0x80 && !memcmp(pkt + 1, "theora", 6)) {
      c.type = KR_CODEC_VIDEO;
      c.video = KR_VIDEO_CODEC_THEORA;
      break;
    }
    break;
  }
  return c;
}

static int ogg_get_truncated(kr_ogg_pkt_info *info, kr_ogg_page *page,
 kr_ogg_unpacker *u) {
  ogg_track_depacketizer *track;
  track = get_track(u, page->serialno);
  info->truncated = u->stream_st.input_sz - u->stream_st.input_pos;
  if (track == NULL) {
    if (u->stream_st.sstate != OGG_IN_HEADERS) return 0;
    track = &u->tracks[u->ntracks];
    track->num = u->ntracks;
    u->tracks[u->ntracks++].serial = page->serialno;
  }
  info->track = track->num;
  info->num = u->stream_st.packets;
  return 0;
}

static ssize_t ogg_page_packet_output(kr_ogg_pkt_info *info, kr_ogg_page *page,
 kr_ogg_unpacker *u) {
  int i;
  ogg_track_depacketizer *track;
  kr_codec_packet *pkt;
  track = get_track(u, page->serialno);
  if (track == NULL) {
    if (u->stream_st.sstate != OGG_IN_HEADERS) return -2;
    printk("Ogg Unpack: new serial %zu", page->serialno);
    track = &u->tracks[u->ntracks];
    track->num = u->ntracks;
    u->tracks[u->ntracks++].serial = page->serialno;
    if (u->out_pkts == 1) {
      track = &u->tracks[0];
      track->num = 0;
      u->tracks[0].serial = page->serialno;
      u->ntracks = 1;
    }
  }
  if (track->curr_pkt == NULL) {
    printke("No output set for track of serial %zu", page->serialno);
    return -3;
  }
  pkt = track->curr_pkt;
  pkt->tc = page->granulepos;
  if (u->stream_st.pstate == OGG_NEW_PAGE) {
    if (!page->segments) return -1;
    info->track = track->num;
    info->granule_pos = page->granulepos;
    if (page->bos) {
      info->hdr = 1;
      info->codec.type = 0;
      track->codec = ogg_pkt_get_codec(page->data, page->lacing[0]);
      info->codec = track->codec;
      u->stream_st.sstate = OGG_IN_HEADERS;
    } else u->stream_st.sstate = OGG_IN_STREAM;
    u->stream_st.pstate = OGG_IN_SEGMENTS;
    u->stream_st.next_seg = 0;
    u->stream_st.input_pos += page->length;
    track->pkt_data = page->data;
  }
  if (u->stream_st.pstate == OGG_IN_SEGMENTS) {
    info->codec = track->codec;
    info->track = track->num;
    info->granule_pos = page->granulepos;
    pkt->len = 0;
    for (i = u->stream_st.next_seg; i < page->segments; i++) {
      if (pkt) {
        pkt->len += page->lacing[i];
      }
      if (page->lacing[i] < 255) {
        info->num = u->stream_st.packets++;
        track->npkts++;
        u->stream_st.next_seg = ++i;
        if (pkt) {
          if ((track->pkt_pos + pkt->len) > pkt->sz) return -1;
          memcpy(pkt->buf + track->pkt_pos, track->pkt_data, pkt->len);
          track->pkt_data += pkt->len;
          track->pkt_pos += pkt->len;
          info->sz += pkt->len;
          track->pkt_pos = 0;
        }
        break;
      }
    }
    if (i == page->segments) u->stream_st.pstate = OGG_NEW_PAGE;
    if (i == page->segments && page->lacing[i-1] == 255) {
      u->stream_st.continued = 1;
      if (pkt) {
      if ((track->pkt_pos + pkt->len) > pkt->sz) return -1;
        memcpy(pkt->buf + track->pkt_pos, track->pkt_data, pkt->len);
        info->sz += pkt->len;
        track->pkt_pos += pkt->len;
      }
    } else {
      u->stream_st.continued = 0;
      if (pkt) pkt->len = info->sz;
      return 1;
    }
  }
  return 0;
}

void kr_ogg_unpacker_resync(kr_ogg_unpacker *u) {
  int i;
  memset(&u->stream_st, 0, sizeof(ogg_stream_st));
  for (i = 0; i < u->ntracks; i++) {
    u->tracks[i].pkt_pos = 0;
    if (u->tracks[i].curr_pkt) u->tracks[i].curr_pkt->len = 0;
  }
  u->resync = 1;
}

int kr_ogg_unpacker_output_set(kr_ogg_unpacker *u,
 kr_codec_packet *pkts, size_t npkts) {
  int i;
  if (!u || !pkts) return -1;
  if (!npkts || npkts > OGG_MAX_TRACKS) return -1;
  for (i = 0; i < OGG_MAX_TRACKS; i++) {
    if (i < npkts)
      u->tracks[i].curr_pkt = &pkts[i];
    else
      u->tracks[i].curr_pkt = NULL;
  }
  u->out_pkts = npkts;
  return 0;
}

size_t kr_ogg_unpacker_sizeof() {
  return sizeof(kr_ogg_unpacker);
}

kr_ogg_unpacker *kr_ogg_unpacker_init(kr_ogg_unpacker_setup *setup) {
  kr_ogg_unpacker *u;
  if (!setup || !setup->mem) return NULL;
  u = (kr_ogg_unpacker *)setup->mem;
  memset(u, 0, sizeof(*u));
  return u;
}

int kr_ogg_unpack(kr_ogg_unpacker *u, kr_ogg_pkt_info *pkt,
 uint8_t *buf, size_t sz) {
  ssize_t ret;
  if (!u || !pkt || !buf || !sz) return -1;
  memset(pkt, 0, sizeof(*pkt));
  u->stream_st.input = buf;
  u->stream_st.input_sz = sz;
  if (u->resync) {
    ret = kr_ogg_scan(buf, sz);
    if (ret < 0) {
      printke("ogg scan: couldn't find capture!");
      return -45;
    }
    u->stream_st.input_pos += ret;
    u->resync = 0;
  }
  for (;;) {
    if (u->stream_st.pstate == OGG_NEW_PAGE) {
      printkd("input %d/%d", u->stream_st.input_pos, u->stream_st.input_sz);
      if (u->stream_st.input_pos >= u->stream_st.input_sz) return 1;
      ret = kr_ogg_parse(&u->stream_st.curr_page, u->stream_st.input + u->stream_st.input_pos,
        u->stream_st.input_sz - u->stream_st.input_pos);
      if (ret == -45) {
        ret = ogg_get_truncated(pkt, &u->stream_st.curr_page, u);
        return ret;
      }
      if (ret != 0) return ret;
    }
    ret = ogg_page_packet_output(pkt, &u->stream_st.curr_page, u);
    if (u->stream_st.continued) continue;
    if (ret < 0) return ret;
    if (ret) return 0;
  }
  return 0;
}
