#if !defined(_tasks_quvi_types_H)
# define _tasks_quvi_types_H (1)

#define KR_QUVI_NVERSIONS 1

typedef struct {
  char url[2048];
  char id[256];
  char container[32];
  char video_encoding[32];
  char audio_encoding[32];
  double video_bitrate;
  double audio_bitrate;
  double video_height;
  double video_width;
} kr_media_version;

typedef struct {
  char id[256];
  char title[256];
  char thumbnail_url[2048];
  double start_time_ms;
  double duration_ms;
  int versions;
  kr_media_version version[KR_QUVI_NVERSIONS];
} kr_quvi_media_info;

typedef struct {
  kr_quvi_media_info media_info;
} kr_quvi_status_info;

typedef struct {
  char url[2048];
} kr_quvi_params_info;

typedef struct {
  kr_quvi_params_info params;
  kr_quvi_status_info status;
} kr_quvi_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
