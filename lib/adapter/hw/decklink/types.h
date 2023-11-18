#if !defined(_adapter_hw_decklink_types_H)
# define _adapter_hw_decklink_types_H (1)

typedef enum {
  KR_DL_VIDEO_DEFAULT,
  KR_DL_SDI,
  KR_DL_OPTICAL_SDI,
  KR_DL_HDMI,
  KR_DL_COMPONENT,
  KR_DL_COMPOSITE,
  KR_DL_SVIDEO
} kr_decklink_video_conn_type;

typedef enum {
  KR_DL_AUDIO_DEFAULT,
  KR_DL_EMBEDDED = 1,
  KR_DL_ANALOG,
  KR_DL_AESEBU
} kr_decklink_audio_conn_type;

typedef struct {
  char name[64];
  int num;
} kr_decklink_info;

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t num;
  uint32_t den;
  kr_decklink_video_conn_type video_conn;
  kr_decklink_audio_conn_type audio_conn;
} kr_decklink_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
