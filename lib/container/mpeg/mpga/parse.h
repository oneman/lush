#if !defined(_container_mpeg_mpga_parse_H)
# define _container_mpga_mpga_parse_H (1)

#include <inttypes.h>

typedef struct {
  uint8_t version;
  uint8_t layer;
  uint8_t channels;
  uint8_t protected;
  uint32_t bit_rate;
  uint16_t sample_rate;
  size_t frames;
  size_t frames_sz;
} kr_mpga_info;

int kr_mpga_parse(kr_mpga_info *info, uint8_t *buf, size_t sz);
ssize_t kr_mpga_scan(uint8_t *buf, size_t sz, size_t *read);

#endif
