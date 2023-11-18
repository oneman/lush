#if !defined(_playlist_m3u_types_H)
# define _playlist_m3u_types_H (1)

#define M3U_MAX_EL 2

typedef enum {
  M3U_URI = 1,
  M3U_TAG
} m3u_el_type;

typedef enum {
  EXTM3U = 1,
  EXTINF,
  EXT_X_BYTERANGE,
  EXT_X_TARGETDURATION,
  EXT_X_MEDIA_SEQUENCE,
  EXT_X_KEY,
  EXT_X_PROGRAM_DATE_TIME,
  EXT_X_ALLOW_CACHE,
  EXT_X_PLAYLIST_TYPE,
  EXT_X_STREAM_INF,
  EXT_X_I_FRAME_STREAM_INF,
  EXT_X_I_FRAMES_ONLY,
  EXT_X_MAP,
  EXT_X_MEDIA,
  EXT_X_ENDLIST,
  EXT_X_DISCONTINUITY,
  EXT_X_DISCONTINUITY_SEQUENCE,
  EXT_X_INDEPENDENT_SEGMENTS,
  EXT_X_START,
  EXT_X_VERSION
} kr_m3u_tag_name;

typedef enum {
  M3U_TAG_VAL_PRIMITIVE = 1,
  M3U_TAG_VAL_ATTR_LIST
} kr_m3u_tag_val_type;

typedef enum {
  M3U_ATTR_METHOD = 1,
  M3U_ATTR_URI,
  M3U_ATTR_IV,
  M3U_ATTR_KEYFORMAT,
  M3U_ATTR_KEYFORMATVERSIONS,
  M3U_ATTR_TYPE,
  M3U_ATTR_GROUP_ID,
  M3U_ATTR_PROGRAM_ID,
  M3U_ATTR_LANGUAGE,
  M3U_ATTR_ASSOC_LANGUAGE,
  M3U_ATTR_NAME,
  M3U_ATTR_DEFAULT,
  M3U_ATTR_AUTOSELECT,
  M3U_ATTR_FORCED,
  M3U_ATTR_INSTREAM_ID,
  M3U_ATTR_CHARACTERISTICS,
  M3U_ATTR_BANDWIDTH,
  M3U_ATTR_CODECS,
  M3U_ATTR_RESOLUTION,
  M3U_ATTR_AUDIO,
  M3U_ATTR_VIDEO,
  M3U_ATTR_SUBTITLES,
  M3U_ATTR_CLOSED_CAPTIONS,
  M3U_ATTR_BYTERANGE,
  M3U_ATTR_TIME_OFFEST,
  M3U_ATTR_PRECISE
} kr_m3u_tag_attr_name;

typedef struct {
  char value[256];
} kr_m3u_tag_prim_info;

typedef struct {
  kr_m3u_tag_attr_name name;
  char value[256];
} kr_m3u_tag_alist_info;

typedef struct {
  kr_m3u_tag_name name;
  kr_m3u_tag_val_type vtype;
  union {
    kr_m3u_tag_prim_info prim[16];
    kr_m3u_tag_alist_info alist[16];
  };
  uint32_t count;
} m3u_tag_info;

typedef struct {
  m3u_el_type type;
  union {
    char uri[512];
    m3u_tag_info tag;
  };
} m3u_el_info;

typedef struct {
  m3u_el_info elements[M3U_MAX_EL];
  uint32_t el_count;
} kr_m3u_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

#endif
