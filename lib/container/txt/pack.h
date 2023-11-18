#if !defined(_container_txt_pack_H)
# define _container_txt_pack_H (1)

#include <inttypes.h>
#include <krad/codec/header.h>
#include <krad/container/packet.h>
#include <krad/container/types.h>
#include <krad/track/types.h>

#define KR_TXT_MUXER_NTRACKS 8
#define KR_TXT_COMMENT_SZ 96
#define KR_TXT_VERSION 1

typedef struct kr_txt_muxer kr_txt_muxer;

typedef struct {
  kr_codec_header header;
  kr_track_info info;
} kr_txt_track_setup;

typedef struct {
  uint8_t *mem;
  char comment[KR_TXT_COMMENT_SZ];
  size_t comment_len;
  int tracks;
  kr_txt_track_setup track[KR_TXT_MUXER_NTRACKS];
} kr_txt_muxer_setup;

typedef struct {
  int track;
  struct {
    size_t sz;
    int64_t tc;
    union {
      uint16_t keyframe;
      uint16_t nframes;
    };
  } in;
  struct {
    uint8_t *framing;
    size_t used;
    size_t space;
    int sync;
  } out;
} kr_txt_muxer_data;

ssize_t kr_txt_mux(kr_txt_muxer *m, kr_txt_muxer_data *data);
ssize_t kr_txt_muxer_pack_header(kr_txt_muxer *m, uint8_t *buf, size_t sz);
size_t kr_txt_muxer_sizeof();
kr_txt_muxer *kr_txt_muxer_init(kr_txt_muxer_setup *setup);

#endif
