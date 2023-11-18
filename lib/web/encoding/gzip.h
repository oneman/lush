#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#if !defined(_web_encoding_gzip_H)
# define _web_encoding_gzip_H (1)

typedef struct kr_gzip kr_gzip;

typedef enum {
  KR_GZIP_DEFLATE = 1,
  KR_GZIP_INFLATE
} kr_gzip_type;

typedef struct {
  void *mem;
  kr_gzip_type type;
} kr_gzip_setup;

typedef struct {
  uint8_t *in;
  size_t in_sz;
  size_t read;
  uint8_t *out;
  size_t out_sz;
  size_t wrote;
} kr_gzip_data;

size_t kr_gzip_sizeof();
int kr_gzip_inflate(kr_gzip *gz, kr_gzip_data *data, int flush);
int kr_gzip_deflate(kr_gzip *gz, kr_gzip_data *data, int flush);
kr_gzip *kr_gzip_init(kr_gzip_setup *setup);

#endif
