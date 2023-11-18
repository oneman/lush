#if !defined(_container_mkv_unpack_H)
# define _container_mkv_unpack_H (1)

#include <inttypes.h>
#include <krad/codec/header.h>
#include <krad/container/packet.h>
#include <krad/container/types.h>
#include <krad/track/types.h>

#define KR_MKV_DEMUXER_NTRACKS 8

typedef struct kr_mkv_demuxer kr_mkv_demuxer;

typedef struct {
  uint8_t *mem;
} kr_mkv_demuxer_setup;

typedef struct {
  int ntracks;
  kr_codec_header header[KR_MKV_DEMUXER_NTRACKS];
  kr_track_info track[KR_MKV_DEMUXER_NTRACKS];
} kr_mkv_header;

typedef struct {
  uint32_t track;
  uint8_t *buf;
  uint8_t flags;
  size_t sz;
  uint64_t tc;
  uint8_t keyframe;
} kr_mkv_block;

size_t kr_mkv_demuxer_sizeof();
kr_mkv_demuxer *kr_mkv_demuxer_init(kr_mkv_demuxer_setup *setup);
ssize_t kr_mkv_start(kr_mkv_demuxer *d, kr_mkv_header *h, uint8_t *buf,
 size_t sz);
ssize_t kr_mkv_demux(kr_mkv_demuxer *d, kr_mkv_block *b, uint8_t *buf,
 size_t sz);

#endif
