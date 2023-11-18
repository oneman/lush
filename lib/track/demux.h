#if !defined(_track_demux_H)
# define _track_demux_H (1)

#include <krad/radio/version.h>
#include <krad/app/types.h>
#include <krad/app/spinlock.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/radio/limits.h>
#include <krad/container/mpeg/ts/unpack.h>
#include <krad/container/mkv/unpack.h>
#include <krad/container/ogg/unpack.h>
#include <krad/container/y4m/unpack.h>
#include <krad/container/flac/unpack.h>
#include <krad/container/mpeg/mpga/parse.h>
#include <krad/container/mpeg/adts/parse.h>
#include <krad/container/occ/occ.h>
#include "types.h"

typedef struct studio_demux kr_demux;
typedef struct studio_demux studio_demux;

struct studio_demux {
  struct {
    kr_track *track[KR_DEMUXER_NTRACKS];
    kr_track_head *head[KR_DEMUXER_NTRACKS];
  } output;
  union {
    kr_mts *ts;
    kr_ogg_unpacker *ogg;
    kr_y4m_demuxer *y4m;
    kr_mkv_demuxer *mkv;
    kr_flac_demuxer *flac;
  };
  kr_spinlock lock;
  kr_amem(mem, 1024 * 1024 * 2)
  kr_amem(header, 1024 * 1024)
  kr_amem(buffer, 8192 * 8)
  size_t buffer_len;
  kr_pool *coded;
  uint64_t frames;
  kr_studio_path *source;
};

typedef struct {
  uint8_t *buf;
  size_t len;
  char ext[8];
} studio_demux_probe_input;

int studio_demux_destroy(kr_studio_path *path);
int studio_demux_create(kr_studio_path *path);
int studio_demux_probe(kr_studio_path *path, studio_demux_probe_input  *in);
int studio_demux_read(kr_studio_path *path, uint8_t *buf, size_t len);

#endif
