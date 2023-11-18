#include <inttypes.h>
#include <sys/types.h>

#if !defined(_tuner_hls_types_H)
# define _tuner_hls_types_H (1)

//#define KR_HLS_MAX_SEGMENTS 16
//#define KR_HLS_MAX_VARIANTS 8
//#define KR_HLS_MAX_RENDITIONS 8

#define KR_HLS_MAX_SEGMENTS 2
#define KR_HLS_MAX_VARIANTS 2
#define KR_HLS_MAX_RENDITIONS 2

typedef enum {
  KR_HLS_PLAYLIST_MASTER = 1,
  KR_HLS_PLAYLIST_MEDIA
} kr_hls_playlist_type;

typedef enum {
  HLS_AUDIO = 1,
  HLS_VIDEO,
  HLS_SUBTITLES,
  HLS_CLOSED_CAPTIONS
} kr_hls_rendition_type;

typedef enum {
  HLS_VOD = 1,
  HLS_LIVE
} kr_hls_media_playlist_type;

typedef struct {
  char name[256]; /* opt */
  char url[256];
  char codecs[128]; /* opt */
  uint32_t bitrate;
  uint width; /* opt */
  uint height; /* opt */
  uint audio; /* opt */
  uint video; /* opt */
  uint subtitles; /* opt */
} kr_hls_variant;

typedef struct {
  char name[256];
  char group[256];
  char url[256];
  kr_hls_rendition_type type;
  uint def;
} kr_hls_rendition;

typedef struct {
  char url[256];
  uint nrenditions;
  uint nvariants;
  kr_hls_rendition rendition[KR_HLS_MAX_RENDITIONS];
  kr_hls_variant variant[KR_HLS_MAX_VARIANTS];
} kr_hls_master_playlist;

typedef struct {
  char url[256];
  uint sequence_num;
  float duration;
} kr_hls_media_segment;

typedef struct {
  kr_hls_media_playlist_type type;
  uint max_duration;
  uint sequence_num;
  uint nsegments;
  kr_hls_media_segment segment[KR_HLS_MAX_SEGMENTS];
} kr_hls_media_playlist;

typedef struct {
  kr_hls_playlist_type type;
  union {
    kr_hls_master_playlist master;
    kr_hls_media_playlist media;
  };
} kr_hls_playlist;

typedef struct {

} kr_hls_tuner_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
