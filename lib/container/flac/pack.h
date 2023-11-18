#if !defined(_container_flac_pack_H)
# define _container_flac_pack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>
#include <krad/codec/audio/flac/encoder.h>

typedef struct kr_flac_muxer kr_flac_muxer;

typedef struct {
  uint8_t *header;
  size_t header_sz;
  uint8_t *mem;
} kr_flac_muxer_setup;

int kr_flac_mux_ok(kr_codec *track, int ntracks);
size_t kr_flac_muxer_sizeof();
kr_flac_muxer *kr_flac_muxer_init(kr_flac_muxer_setup *setup);
size_t kr_flac_pack_header(kr_flac_muxer *m, uint8_t *buf, size_t max);

#endif
