#include <krad/radio/version.h>
#include <krad/app/types.h>
#include <krad/app/spinlock.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include <krad/codec/types.h>
#include <krad/codec/header.h>
#include <krad/container/txt/pack.h>
#include <krad/container/y4m/pack.h>
#include <krad/container/flac/pack.h>
#include <krad/container/ogg/pack.h>
#include <krad/container/mkv/pack.h>
#include <krad/container/mpeg/ts/pack.h>
#include <krad/container/occ/occ.h>

#define KR_MUXER_MAX_NTRACKS 4
#define KR_MUXER_MAX_OUTPUTS 4

#include "types.h"

#if !defined(_track_mux_H)
# define _track_mux_H (1)

typedef struct studio_mux kr_mux;
typedef struct studio_mux studio_mux;

#include "mux_output.h"

struct studio_mux {
  kr_track *track[KR_MUXER_MAX_NTRACKS];
  kr_track_head *head[KR_MUXER_MAX_NTRACKS];
  union {
    void *mkv;
    kr_y4m_muxer *y4m;
    kr_ogg_muxer *ogg;
    kr_flac_muxer *flac;
    kr_txt_muxer *txt;
    kr_mts_muxer *mts;
  };
  kr_spinlock lock;
  kr_amem(mem, 8192 * 8)
  kr_amem(header, 8192 * 3)
  size_t header_sz;
  kr_studio_path *outputs[KR_MUXER_MAX_OUTPUTS];
  int noutputs;
  struct {
    kr_track *track;
    kr_track_head *head;
  } output;
};

int studio_mux_add_output(studio_mux *mux, kr_studio_path *out);
int studio_mux_rm_output(studio_mux *mux, kr_studio_path *out);
int studio_mux_destroy(kr_studio_path *path);
int studio_mux_create(kr_studio_path *path);

#endif
