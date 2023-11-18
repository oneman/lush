#if !defined(_container_packet_H)
# define _container_packet_H (1)

#include <stdint.h>

typedef struct {
  uint8_t *buf;
  size_t sz;
  size_t len;
  int64_t tc;
} kr_codec_packet;

#endif
