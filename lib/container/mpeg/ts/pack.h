#if !defined(_container_mpeg_ts_pack_H)
# define _container_mpeg_ts_pack_H (1)

#include <inttypes.h>
#include <krad/codec/header.h>
#include <krad/container/packet.h>
#include <krad/container/types.h>
#include <krad/track/types.h>

#define KR_MTS_MUXER_NSTREAMS 2
#define KR_MTS_MUXER_VECLEN 256

typedef struct kr_mts_muxer kr_mts_muxer;

typedef struct {
  uint8_t *mem;
  kr_track_info track_info[KR_MTS_MUXER_NSTREAMS];
  int tracks;
} kr_mts_muxer_setup;

typedef struct {
  int track;
  uint8_t *input;
  size_t sz;
  struct {
    uint8_t *hdr;
    size_t hdr_sz;
    uint8_t *buf;
    size_t sz;
  } pkts[KR_MTS_MUXER_VECLEN];
  int npkts;
  int64_t pts;
  int64_t dts;
} kr_mts_muxer_data;

size_t kr_mts_muxer_sizeof();
ssize_t kr_mts_muxer_pack_header(kr_mts_muxer *m, uint8_t *buf, size_t sz);
int kr_mts_mux(kr_mts_muxer *m, kr_mts_muxer_data *data);
kr_mts_muxer *kr_mts_muxer_init(kr_mts_muxer_setup *setup);
int kr_mts_mux_ok(kr_codec *codecs, int ntracks);

#endif
