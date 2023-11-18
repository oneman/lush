#if !defined(_container_ogg_parse_H)
# define _container_ogg_parse_H (1)

#include <stdint.h>

typedef struct {
  uint8_t *capture;
  uint8_t version;
  uint8_t flags;
  int64_t granulepos;
  uint32_t serialno;
  uint32_t sequenceno;
  uint32_t crc;
  uint8_t segments;
  uint8_t *lacing;
  uint8_t *data;
  uint8_t continued;
  uint8_t bos;
  uint8_t eos;
  size_t length;
  size_t data_length;
} kr_ogg_page;

ssize_t kr_ogg_scan(uint8_t *buf, size_t sz);
ssize_t kr_ogg_parse(kr_ogg_page *page,
  uint8_t *buf, size_t sz);
ssize_t kr_ogg_valid(uint8_t *buf, size_t sz);

#endif
