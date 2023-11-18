#include <stdlib.h>
#include <string.h>
#include <krad/app/debug.h>
#include <krad/codec/video/y4m/encoder.h>
#include "pack.h"

struct kr_y4m_muxer {
  uint8_t header[80];
  size_t header_sz;
};

int kr_y4m_mux_ok(kr_codec *track, int ntracks) {
  if (ntracks != 1) return 0;
  if (track[0].type != KR_CODEC_VIDEO) return 0;
  if (track[0].video != KR_VIDEO_CODEC_Y4M) return 0;
  return 1;
}

size_t kr_y4m_muxer_sizeof() {
  return sizeof(kr_y4m_muxer);
}

kr_y4m_muxer *kr_y4m_muxer_init(kr_y4m_muxer_setup *setup) {
  kr_y4m_muxer *m;
  if (!setup || !setup->mem || !setup->header_sz) {
    printke("Y4M Muxer: init failed");
    return NULL;
  }
  m = (kr_y4m_muxer *)setup->mem;
  m->header_sz = setup->header_sz;
  memcpy(m->header, setup->header, m->header_sz);
  return m;
}

size_t kr_y4m_pack_header(kr_y4m_muxer *m, uint8_t *buf, size_t max) {
  if (!m || !buf) return 0;
  if (max < m->header_sz) return 0;
  memcpy(buf, m->header, m->header_sz);
  return m->header_sz;
}

size_t kr_y4m_pack_frame_header(kr_y4m_muxer *m, uint8_t *buf, size_t max) {
  if (!m || !buf) return 0;
  if (max < KR_Y4M_FRAME_HEADER_SIZE) return 0;
  memcpy(buf, KR_Y4M_FRAME_HEADER, KR_Y4M_FRAME_HEADER_SIZE);
  return KR_Y4M_FRAME_HEADER_SIZE;
}
