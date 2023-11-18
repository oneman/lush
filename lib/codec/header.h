#if !defined(_codec_header_H)
# define _codec_header_H (1)

#include <stddef.h>

#define KR_CODEC_NHEADERS 3

typedef struct {
  uint8_t *buf;
  size_t sz;
} kr_iov;

typedef struct {
  kr_iov data[KR_CODEC_NHEADERS];
  size_t len;
} kr_codec_header;

#endif
