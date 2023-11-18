#if !defined(_container_mts_types_H)
# define _container_mts_types_H (1)

#include <krad/codec/types.h>

#define MTS_NPROGRAMS 2
#define MTS_NSTREAMS 8

typedef enum {
  MTS_VIDEO = 1,
  MTS_AUDIO,
  MTS_OTHER
} kr_mts_stream_type;

typedef struct {
  kr_mts_stream_type type;
  int16_t id;
  int8_t track;
  uint64_t pts;
  uint64_t dts;
  uint64_t sz;
  uint64_t pos;
} kr_mts_packet_info;

typedef struct {
  uint16_t id;
  char type_str[64];
  kr_codec codec;
  kr_mts_stream_type type;
  union {
    kr_audio_info audio;
    kr_video_info video;
    int coconut;
  };
} kr_mts_stream_info;

typedef struct {
  uint16_t id;
  kr_mts_stream_info streams[MTS_NSTREAMS];
  uint8_t nstreams;
} kr_mts_program_info;

typedef struct {
  uint64_t npackets;
  kr_mts_program_info programs[MTS_NPROGRAMS];
  uint8_t nprograms;
} kr_mts_info;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
