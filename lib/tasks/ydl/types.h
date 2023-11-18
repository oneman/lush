#if !defined(_tasks_ydl_types_H)
# define _tasks_ydl_types_H (1)

#include <stdint.h>

//#define KR_YDL_MAX_FORMATS 32
#define KR_YDL_MAX_FORMATS 1

typedef struct {
  char url[1024];
  char ext[16];
  char format_id[32];
  char format[256]; /* opt */
  char vcodec[32]; /* opt */
  uint16_t width; /* opt */
  uint16_t height; /* opt */
  uint32_t abr; /* opt */
} kr_ydl_format_info;

typedef struct {
  uint32_t duration;
  char description[4096];
  char uploader_id[64];
  char uploader[64];
  uint32_t view_count;
  char title[256];
  char ext[16];
  char id[32];
  char thumbnail[256];
  uint16_t width;
  uint16_t height;
  char upload_date[16];
  kr_ydl_format_info formats[KR_YDL_MAX_FORMATS];
} kr_ydl_video_info;

typedef struct {
  char url[512];
} kr_ydl_params_info;

typedef struct {
  kr_ydl_video_info video;
} kr_ydl_status_info;

typedef struct {
  kr_ydl_params_info params;
  kr_ydl_status_info status;
} kr_ydl_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
