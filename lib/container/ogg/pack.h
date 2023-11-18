#if !defined(_container_ogg_pack_H)
# define _container_ogg_pack_H (1)

#include <inttypes.h>
#include <krad/codec/header.h>
#include <krad/container/packet.h>

#define KR_OGG_MUXER_NTRACKS 2
#define KR_OGG_MUXER_VECLEN 128
#define KR_OGG_MUXER_MAX_PACKET_SZ KR_OGG_MUXER_VECLEN / 2 * (255 * 255)

typedef struct kr_ogg_muxer kr_ogg_muxer;

typedef enum {
  KR_OGG_HEADER = 1,
  KR_OGG_CODEC_HEADER,
  KR_OGG_CODEC_PACKET
} kr_ogg_muxer_output_type;

typedef struct {
  uint8_t *mem;
  kr_codec_header header[KR_OGG_MUXER_NTRACKS];
  int tracks;
} kr_ogg_muxer_setup;

typedef struct {
  uint8_t *framing_buf;
  size_t sz;
  int track;
  struct {
    int64_t granule_pos;
    uint8_t *buf;
    size_t sz;
  } in[KR_OGG_MUXER_VECLEN];
  int in_len;
  struct {
    kr_ogg_muxer_output_type type;
    uint8_t *buf;
    size_t sz;
  } out[KR_OGG_MUXER_VECLEN];
  int out_len;
} kr_ogg_muxer_data;

ssize_t kr_ogg_mux(kr_ogg_muxer *m, kr_ogg_muxer_data *data);
ssize_t kr_ogg_muxer_read_header(kr_ogg_muxer *m, uint8_t *buf, size_t max);
size_t kr_ogg_muxer_sizeof();
kr_ogg_muxer *kr_ogg_muxer_init(kr_ogg_muxer_setup *setup);

#endif
