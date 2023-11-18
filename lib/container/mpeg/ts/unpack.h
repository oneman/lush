#if !defined(_container_mpeg_ts_unpack_H)
# define _container_mpeg_ts_unpack_H (1)

#include "types.h"

typedef struct kr_mts kr_mts;

typedef struct {
  void *mem;
} kr_mts_setup;

size_t kr_mts_sizeof();
ssize_t kr_mts_parse(kr_mts *mts, kr_mts_packet_info *pkt,
 uint8_t *buf, size_t sz);
int kr_mts_output_set(kr_mts *mts, uint8_t *buf, size_t sz);
int kr_mts_info_get(kr_mts *mts, kr_mts_info *info);
int kr_mts_resync(kr_mts *mts);
kr_mts *kr_mts_init(kr_mts_setup *setup);

#endif
