#if !defined(_tasks_avprobe_types_H)
# define _tasks_avprobe_types_H (1)

//#define KR_AVP_MAX_SIZE 512
//#define KR_AVP_MAX_STREAMS 8
#define KR_AVP_MAX_SIZE 32
#define KR_AVP_MAX_STREAMS 1
#include <stdint.h>

typedef enum {
  KR_AUDIO = 1,
  KR_VIDEO = 2
} kr_avprobe_codec_type;

typedef struct {
  char filename[KR_AVP_MAX_SIZE];
  uint32_t nb_streams; /* opt */
  char format_name[KR_AVP_MAX_SIZE]; /* opt */
  char format_long_name[KR_AVP_MAX_SIZE]; /* opt */
  char start_time[KR_AVP_MAX_SIZE]; /* opt */
  char duration[KR_AVP_MAX_SIZE]; /* opt */
  char size[KR_AVP_MAX_SIZE]; /* opt */
  char bit_rate[KR_AVP_MAX_SIZE]; /* opt */
} kr_avprobe_format_info;

typedef struct {
  uint32_t index;
  char codec_name[KR_AVP_MAX_SIZE];
  kr_avprobe_codec_type codec_type;
  uint16_t width; /* opt */
  uint16_t height; /* opt */
  char start_time[KR_AVP_MAX_SIZE]; /* opt */
  char duration[KR_AVP_MAX_SIZE]; /* opt */
  uint32_t channels; /* opt */
  char sample_rate[KR_AVP_MAX_SIZE]; /* opt */
  char display_aspect_ratio[KR_AVP_MAX_SIZE]; /* opt */
  char pix_fmt[KR_AVP_MAX_SIZE]; /* opt */
} kr_avprobe_stream_info;

typedef struct {
  kr_avprobe_format_info format;
  kr_avprobe_stream_info streams[KR_AVP_MAX_STREAMS];
} kr_avprobe_media_info;

typedef struct {
  char filename[KR_AVP_MAX_SIZE];
} kr_avprobe_params_info;

typedef struct {
  kr_avprobe_media_info media;
} kr_avprobe_status_info;

typedef struct {
  kr_avprobe_params_info params;
  kr_avprobe_status_info status;
} kr_avprobe_info;

#include "gen/types_to_text.h"
#include "gen/types_helpers.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
