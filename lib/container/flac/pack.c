#include <stdlib.h>
#include <string.h>
#include <krad/app/debug.h>
#include "pack.h"

struct kr_flac_muxer {
  uint8_t header[8192 * 2];
  size_t header_sz;
};

int kr_flac_mux_ok(kr_codec *track, int ntracks) {
  if (ntracks != 1) return 0;
  if (track[0].type != KR_CODEC_AUDIO) return 0;
  if (track[0].audio != KR_AUDIO_CODEC_FLAC) return 0;
  return 1;
}

size_t kr_flac_muxer_sizeof() {
  return sizeof(kr_flac_muxer);
}

kr_flac_muxer *kr_flac_muxer_init(kr_flac_muxer_setup *setup) {
  kr_flac_muxer *m;
  if (!setup || !setup->mem || !setup->header_sz) {
    printke("FLAC Muxer: init failed");
    return NULL;
  }
  m = (kr_flac_muxer *)setup->mem;
  m->header_sz = setup->header_sz;
  if (setup->header_sz > sizeof(m->header)) {
    printke("FLAC Muxer: header too big for me");
    return NULL;
  }
  memcpy(m->header, setup->header, m->header_sz);
  return m;
}

size_t kr_flac_pack_header(kr_flac_muxer *m, uint8_t *buf, size_t max) {
  if (!m || !buf) return 0;
  if (max < m->header_sz) return 0;
  memcpy(buf, m->header, m->header_sz);
  return m->header_sz;
}
