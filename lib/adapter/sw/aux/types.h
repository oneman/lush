#if !defined(_adapter_sw_aux_types_H)
# define _adapter_sw_aux_types_H (1)

#define KR_AUX_NPORTS 16
#define KR_AUX_NFDS 3
#define KR_AUX_PORT_NFRAMES 4
#define KR_AUX_PORT_NPERIODS 4

#define KR_AUX_KLUDGE_WIDTH 640
#define KR_AUX_KLUDGE_HEIGHT 360
#define KR_AUX_KLUDGE_FMT KR_PIXELS_YUV444
#define KR_AUX_KLUDGE_NCHANNELS 2
#define KR_AUX_KLUDGE_PERIOD_NFRAMES 960

typedef enum {
  KR_AUX_PORT_DISCONNECTED,
  KR_AUX_PORT_CONNECTED
} kr_aux_port_state;

typedef struct {
  int state;
} kr_aux_info;

typedef struct {
  kr_aux_port_state state;
} kr_aux_path_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"
#include "gen/types_helpers.h"

#endif
