#if !defined(_mixer_types_H)
# define _mixer_types_H (1)

#define KR_MXR_PATHS_DEF 12
#define KR_MXR_MAX_CHANNELS 2
#define KR_MXR_SRATE_MIN 44100
#define KR_MXR_SRATE_MAX 48000
#define KR_MXR_PERIOD_MIN 16
#define KR_MXR_PERIOD_MAX 1920

#define KR_MXR_SRATE_DEF 48000
#define KR_MXR_PERIOD_DEF 960

#include <krad/audio/audio.h>
#include <krad/audio/types.h>

typedef struct kr_mixer_path_info kr_mixer_path_info;
typedef struct kr_mixer_path_info kr_mixer_source_info;
typedef struct kr_mixer_path_info kr_mixer_input_info;
typedef struct kr_mixer_path_info kr_mixer_bus_info;
typedef struct kr_mixer_path_info kr_mixer_output_info;

typedef enum {
  KR_MONO = 1,
  KR_STEREO,
  KR_THREE,
  KR_QUAD,
  KR_FIVE,
  KR_SIX,
  KR_SEVEN,
  KR_EIGHT
} kr_mixer_channels;

typedef enum {
  KR_MXR_SOURCE = 1,
  KR_MXR_INPUT,
  KR_MXR_BUS,
  KR_MXR_OUTPUT
} kr_mixer_path_type;

#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_helpers.h"

struct kr_mixer_path_info {
  kr_mixer_path_type type; /* MXR_SOURCE */
  kr_mixer_channels channels; /* STEREO */
  kr_afx_info afx[KR_MXR_MAX_CHANNELS];
};

#endif
