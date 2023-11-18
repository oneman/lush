#if !defined(_container_flac_unpack_H)
# define _container_flac_unpack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>
#include <krad/codec/audio/flac/decoder.h>

typedef struct kr_flac_demuxer kr_flac_demuxer;

typedef struct {
  uint8_t *mem;
  uint8_t *buf;
  size_t len;
} kr_flac_demuxer_setup;

typedef struct {
  uint8_t *buf;
  size_t len;
} kr_flac_frame_data;

typedef struct {
  struct {
    uint8_t *buf;
    size_t len;
  } in;
  struct {
    kr_flac_frame_data frames[512];
    size_t len;
  } out;
  size_t read;
} kr_flac_data;

size_t kr_flac_demuxer_sizeof();
kr_flac_demuxer *kr_flac_demuxer_init(kr_flac_demuxer_setup *setup);
ssize_t kr_flac_unpack_header(kr_flac_demuxer *d, uint8_t *buf, size_t max);
int kr_flac_demux(kr_flac_demuxer *d, kr_flac_data *data);
int kr_flac_demuxer_get_info(kr_flac_demuxer *d, kr_flac_info *info);

#endif
