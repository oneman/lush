#if !defined(_container_mpeg_mpga_id3_H)
# define _container_mpga_mpga_id3_H (1)

#include <inttypes.h>

typedef struct kr_id3_parser kr_id3_parser;

typedef struct {
  size_t sz;
  uint8_t version;
} kr_id3_info;

typedef struct {
  void *mem;
} kr_id3_parser_setup;

size_t kr_id3_parser_sizeof();
kr_id3_parser *kr_id3_parser_init(kr_id3_parser_setup *setup);
int kr_id3_parser_get_info(kr_id3_parser *p, kr_id3_info *info);
int kr_id3_parse(kr_id3_parser *p, uint8_t *buf, size_t sz);
ssize_t kr_id3_scan(uint8_t *buf, size_t sz, size_t *read);

#endif
