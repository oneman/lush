#if !defined(_container_mpeg_adts_parse_H)
# define _container_adts_adts_parse_H (1)

#include <inttypes.h>

typedef enum {
  ACC_MAIN = 1,
  AAC_LC = 2,
  AAC_SSR = 3,
  AAC_LTP = 4,
  AAC_SBR = 5
} kr_adts_profile;

typedef struct {
  uint8_t version;
  uint8_t channels;
  uint8_t profile;
  uint8_t protected;
  uint32_t bit_rate;
  uint16_t sample_rate;
  size_t frames;
  size_t frames_sz;
} kr_adts_info;

int kr_adts_parse(kr_adts_info *info, uint8_t *buf, size_t sz);
ssize_t kr_adts_scan(uint8_t *buf, size_t sz, size_t *read);

#endif
