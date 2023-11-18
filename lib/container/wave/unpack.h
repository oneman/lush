#if !defined(_container_wave_unpack_H)
# define _container_wave_unpack_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include "common.h"

typedef struct kr_wave_parser kr_wave_parser;

typedef struct {
  uint8_t *mem;
} kr_wave_parser_setup;

size_t kr_wave_parser_sizeof();
int kr_wave_parse(kr_wave_parser *w, uint8_t *buf, size_t sz);
kr_wave_parser *kr_wave_parser_init(kr_wave_parser_setup *setup);
int kr_wave_parser_get_info(kr_wave_parser *w, kr_wave_info *info);

#endif
