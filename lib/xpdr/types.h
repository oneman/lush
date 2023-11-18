#if !defined(_xpdr_types_H)
# define _xpdr_types_H (1)

#include <krad/adapter/hw/alsa/types.h>
#include <krad/adapter/hw/v4l2/types.h>
#include <krad/adapter/hw/decklink/types.h>
#include <krad/adapter/sw/jack/types.h>
#include <krad/adapter/sw/wayland/types.h>
#include <krad/adapter/sw/x11/types.h>
#include <krad/adapter/sw/aux/types.h>

typedef struct kr_xpdr_path_info kr_xpdr_path_info;

typedef enum {
  KR_JACK = 1,
  KR_WAYLAND,
  KR_V4L2,
  KR_DECKLINK,
  KR_X11,
  KR_ALSA,
  KR_AUX,
  KR_JACK_IN,
  KR_JACK_OUT,
  KR_WAYLAND_OUT,
  KR_V4L2_IN,
  KR_DECKLINK_VIDEO_IN,
  KR_DECKLINK_AUDIO_IN,
  KR_X11_IN,
  KR_ALSA_IN,
  KR_ALSA_OUT,
  KR_AUX_VIDEO_IN,
  KR_AUX_VIDEO_OUT,
  KR_AUX_AUDIO_IN,
  KR_AUX_AUDIO_OUT,
} kr_xpdr_type;

struct kr_xpdr_path_info {
  kr_xpdr_type type;
  union {
    kr_jack_info jack;
    kr_wayland_info wl;
    kr_v4l2_info v4l2;
    kr_decklink_info dl;
    kr_x11_info x11;
    kr_alsa_info alsa;
    kr_aux_info aux;
    kr_jack_path_info jack_in;
    kr_jack_path_info jack_out;
    kr_wayland_path_info wl_out;
    kr_v4l2_path_info v4l2_in;
    kr_decklink_path_info dlv_in;
    kr_decklink_path_info dla_in;
    kr_x11_path_info x11_in;
    kr_alsa_path_info alsa_in;
    kr_alsa_path_info alsa_out;
    kr_aux_path_info aux_video_in;
    kr_aux_path_info aux_video_out;
    kr_aux_path_info aux_audio_in;
    kr_aux_path_info aux_audio_out;
  };
};

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_helpers.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
