#if !defined(_container_mkv_pack_H)
# define _container_mkv_pack_H (1)

#include <inttypes.h>
#include <krad/codec/header.h>
#include <krad/container/packet.h>
#include <krad/container/types.h>
#include <krad/track/types.h>

#define KR_MKV_MUXER_NTRACKS 8

typedef struct kr_mkv_muxer kr_mkv_muxer;

typedef struct {
  uint8_t *mem;
  char *title;
  kr_container_type type;
  kr_codec_header header[KR_MKV_MUXER_NTRACKS];
  kr_track_info info[KR_MKV_MUXER_NTRACKS];
  int tracks;
} kr_mkv_muxer_setup;

typedef struct {
  int track;
  uint8_t *framing;
  size_t sz;
  size_t len;
  size_t pkt_len;
  int64_t tc;
  uint8_t keyframe;
  uint32_t frames;
  int sync;
} kr_mkv_muxer_data;

ssize_t kr_mkv_mux(kr_mkv_muxer *m, kr_mkv_muxer_data *data);
ssize_t kr_mkv_muxer_pack_header(kr_mkv_muxer *m, uint8_t *buf, size_t sz);
size_t kr_mkv_muxer_sizeof();
kr_mkv_muxer *kr_mkv_muxer_init(kr_mkv_muxer_setup *setup);

#endif
