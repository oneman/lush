#if !defined(_container_occ_occ_H)
# define _container_occ_occ_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <krad/container/packet.h>
#include <krad/codec/types.h>

#define KR_OCC_HEADER_LEN 5
#define KR_OCC_MAX_PKTS 12

typedef struct {
  struct {
    uint8_t hdr[KR_OCC_HEADER_LEN];
    uint8_t *buf;
    size_t len;
  } pkts[KR_OCC_MAX_PKTS];
  size_t npkts;
} kr_occ_data;

int kr_occ_mux_ok(kr_codec *codecs, int ntracks);
int kr_occ_sync_check(uint8_t *buf, size_t len);
int kr_occ_pack(uint8_t *buf, size_t len, kr_occ_data *data);
int kr_occ_unpack(uint8_t *buf, size_t len, kr_occ_data *data);

#endif
