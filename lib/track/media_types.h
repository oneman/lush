#if !defined(_track_media_types_H)
# define _track_media_types_H (1)

typedef enum {
  MEDIA_SOURCE_MTS = 1
} kr_media_source_format;

typedef struct {
  char path[256];
} kr_media_source_info;

typedef enum {
  KR_PKT_HEADER = 1,
  KR_PKT_KEY,
  KR_PKT_NORMAL
} kr_media_packet_type;

typedef struct {
  kr_media_packet_type type;
  int track;
  uint64_t pts;
  uint64_t dts;
  uint8_t *buf;
  uint64_t sz;
} kr_media_packet;

#include "gen/media_types_to_json.h"
#include "gen/media_types_from_json.h"
#include "gen/media_types_to_ebml.h"
#include "gen/media_types_from_ebml.h"
#include "gen/media_types_to_text.h"
#include "gen/media_types_helpers.h"

#endif
