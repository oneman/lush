#include "hw/v4l2/v4l2.h"
#include "hw/decklink/decklink.h"
#include "hw/alsa/alsa.h"
#ifdef KR_USE_JACK
#include "sw/jack/jack.h"
#endif
#ifdef KR_USE_WAYLAND
#include "sw/wayland/wayland.h"
#endif
#ifdef KR_USE_X11
#include "sw/x11/x11.h"
#endif
#include "sw/aux/aux.h"
#include "adapter.h"

typedef enum {
  KR_ADAPTER_CTX = 1,
  KR_VIDEO_IN,
  KR_VIDEO_OUT,
  KR_AUDIO_IN,
  KR_AUDIO_OUT,
  KR_PIXEL_IN,
  KR_PIXEL_OUT
} kr_xpdr_path_mode;

static const kr_xpdr_type adapter_path_adapter_type[] = {
  [0] = 0,
  [KR_JACK] = KR_JACK,
  [KR_WAYLAND] = KR_WAYLAND,
  [KR_V4L2] = KR_V4L2,
  [KR_DECKLINK] = KR_DECKLINK,
  [KR_X11] = KR_X11,
  [KR_ALSA] = KR_ALSA,
  [KR_AUX] = KR_AUX,
  [KR_JACK_IN] = KR_JACK,
  [KR_JACK_OUT] = KR_JACK,
  [KR_WAYLAND_OUT] = KR_WAYLAND,
  [KR_V4L2_IN] = KR_V4L2,
  [KR_DECKLINK_VIDEO_IN] = KR_DECKLINK,
  [KR_DECKLINK_AUDIO_IN] = KR_DECKLINK,
  [KR_X11_IN] = KR_X11,
  [KR_ALSA_IN] = KR_ALSA,
  [KR_ALSA_OUT] = KR_ALSA,
  [KR_AUX_VIDEO_IN] = KR_AUX,
  [KR_AUX_VIDEO_OUT] = KR_AUX,
  [KR_AUX_AUDIO_IN] = KR_AUX,
  [KR_AUX_AUDIO_OUT] = KR_AUX,
};

static const kr_xpdr_path_mode xpdr_type_modes[] = {
  [0] = 0,
  [KR_JACK] = KR_ADAPTER_CTX,
  [KR_WAYLAND] = KR_ADAPTER_CTX,
  [KR_V4L2] = KR_ADAPTER_CTX,
  [KR_DECKLINK] = KR_ADAPTER_CTX,
  [KR_X11] = KR_ADAPTER_CTX,
  [KR_ALSA] = KR_ADAPTER_CTX,
  [KR_AUX] = KR_ADAPTER_CTX,
  [KR_JACK_IN] = KR_AUDIO_IN,
  [KR_JACK_OUT] = KR_AUDIO_OUT,
  [KR_WAYLAND_OUT] = KR_VIDEO_OUT,
  [KR_V4L2_IN] = KR_VIDEO_IN,
  [KR_DECKLINK_VIDEO_IN] = KR_VIDEO_IN,
  [KR_DECKLINK_AUDIO_IN] = KR_AUDIO_IN,
  [KR_X11_IN] = KR_VIDEO_IN,
  [KR_ALSA_IN] = KR_AUDIO_IN,
  [KR_ALSA_OUT] = KR_AUDIO_OUT,
  [KR_AUX_VIDEO_IN] = KR_VIDEO_IN,
  [KR_AUX_VIDEO_OUT] = KR_VIDEO_OUT,
  [KR_AUX_AUDIO_IN] = KR_AUDIO_IN,
  [KR_AUX_AUDIO_OUT] = KR_AUDIO_OUT,
};

static const kr_adapter_spec adapters[] = {
  [0] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#if KR_USE_JACK
  [KR_JACK] = {
    .lctl = kr_jack_lctl,
    .unlink = kr_jack_unlink,
    .link = kr_jack_link,
    .ctl = kr_jack_ctl,
    .close = kr_jack_close,
    .open = kr_jack_open
  },
#else
  [KR_JACK] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#endif
#if KR_USE_WAYLAND
  [KR_WAYLAND] = {
    .lctl = kr_wl_lctl,
    .unlink = kr_wl_unlink,
    .link = kr_wl_link,
    .ctl = kr_wl_ctl,
    .close = kr_wl_close,
    .open = kr_wl_open
  },
#else
  [KR_WAYLAND] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#endif
#ifdef KR_USE_V4L2
  [KR_V4L2] = {
    .lctl = kr_v4l2_lctl,
    .unlink = kr_v4l2_unlink,
    .link = kr_v4l2_link,
    .ctl = kr_v4l2_ctl,
    .close = kr_v4l2_close,
    .open = kr_v4l2_open
  },
#else
  [KR_V4L2] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#endif
#ifdef KR_USE_DECKLINK
  [KR_DECKLINK] = {
    .lctl = kr_decklink_lctl,
    .unlink = kr_decklink_unlink,
    .link = kr_decklink_link,
    .ctl = kr_decklink_ctl,
    .close = kr_decklink_close,
    .open = kr_decklink_open
  },
#else
  [KR_DECKLINK] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#endif
#ifdef KR_USE_X11
  [KR_X11] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#else
  [KR_X11] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
#endif
  [KR_ALSA] = {
    .lctl = NULL,
    .unlink = NULL,
    .link = NULL,
    .ctl = NULL,
    .close = NULL,
    .open = NULL
  },
  [KR_AUX] = {
    .lctl = kr_aux_lctl,
    .unlink = kr_aux_unlink,
    .link = kr_aux_link,
    .ctl = kr_aux_ctl,
    .close = kr_aux_adapter_close,
    .open = kr_aux_open
  }
};

static const int nadapter_types = (sizeof(adapters) / sizeof(adapters[0]));

void kr_adapters_debug_info() {
  int i;
  char *string;
  i = 0;
  printk("%d Adapter types", nadapter_types - 1);
  for (i = 1; i < nadapter_types; i++) {
    string = kr_xpdr_type_to_str(i);
    if ((strlen(string) > 3) && (memcmp(string, "kr_", 3) == 0)) string += 3;
    if (adapters[i].open == NULL) {
      printk("XPDR: %s adapter type NOT available. (Not compiled or code incomplete)", string);
    } else {
      printk("XPDR: %s adapter type is available.", string);
    }
  }
}
