#if !defined(_container_y4m_pack_H)
# define _container_y4m_pack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>
#include "common.h"

typedef struct kr_y4m_muxer kr_y4m_muxer;

typedef struct {
  uint8_t *header;
  size_t header_sz;
  uint8_t *mem;
} kr_y4m_muxer_setup;

int kr_y4m_mux_ok(kr_codec *track, int ntracks);
size_t kr_y4m_muxer_sizeof();
kr_y4m_muxer *kr_y4m_muxer_init(kr_y4m_muxer_setup *setup);
size_t kr_y4m_pack_header(kr_y4m_muxer *m, uint8_t *buf, size_t max);
size_t kr_y4m_pack_frame_header(kr_y4m_muxer *m, uint8_t *buf, size_t max);

#endif
