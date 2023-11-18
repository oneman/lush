#if !defined(_container_ogg_unpack_H)
# define _container_ogg_unpack_H (1)

#include <stdint.h>
#include <krad/codec/types.h>
#include <krad/container/packet.h>
#include "parse.h"

typedef struct kr_ogg_unpacker kr_ogg_unpacker;

typedef struct {
  void *mem;
} kr_ogg_unpacker_setup;

typedef struct {
  uint64_t num;
  uint8_t track;
  uint8_t hdr;
  size_t sz;
  size_t truncated;
  int64_t granule_pos;
  kr_codec codec;
} kr_ogg_pkt_info;

size_t kr_ogg_unpacker_sizeof();
kr_ogg_unpacker *kr_ogg_unpacker_init(kr_ogg_unpacker_setup *setup);
int kr_ogg_unpacker_output_set(kr_ogg_unpacker *u,
 kr_codec_packet *pkts, size_t npkts);
int kr_ogg_unpack(kr_ogg_unpacker *u, kr_ogg_pkt_info *pkt,
 uint8_t *buf, size_t sz);
void kr_ogg_unpacker_resync(kr_ogg_unpacker *u);

#endif
