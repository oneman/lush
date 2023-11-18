#if !defined(_adapter_sw_jack_types_H)
# define _adapter_sw_jack_types_H (1)

#include <inttypes.h>

typedef enum {
  KR_JACK_INPUT = 1,
  KR_JACK_OUTPUT
} kr_jack_direction;

typedef enum {
  KR_JACK_OFFLINE,
  KR_JACK_ONLINE
} kr_jack_state;

typedef enum {
  KR_JACK_PATH_FAIL,
  KR_JACK_PATH_ACTIVE
} kr_jack_path_state;

typedef struct kr_jack_path_info kr_jack_input_info;
typedef struct kr_jack_path_info kr_jack_output_info;

typedef struct {
  char client_name[64];
  char server_name[64];
  kr_jack_state state;
  uint32_t inputs;
  uint32_t outputs;
  uint32_t sample_rate;
  uint32_t period_size;
  uint32_t xruns;
  uint64_t frames;
} kr_jack_info;

typedef struct {
  char name[64];
  int channels;
  kr_jack_direction direction;
} kr_jack_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
