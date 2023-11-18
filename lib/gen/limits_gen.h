#if !defined(_gen_limits_H)
# define _gen_limits_H (1)

#include <krad/radio/limits.h>
#include <krad/radio/limit_types.h>

typedef enum {
  KR_RADIO_CLIENTS_MAX_LIMIT = 0,
  KR_MIXER_NPATHS_LIMIT = 1,
  KR_XFER_NPATHS_LIMIT = 2,
  KR_TRANSMIT_CLIENTS_MAX_LIMIT = 3,
  KR_WEB_NPATHS_LIMIT = 4,
  KR_XPDR_NPATHS_LIMIT = 5,
  KR_TRACKER_NTRACKS_LIMIT = 6,
  KR_TRACK_NRDHEADS_LIMIT = 7,
  KR_TRACK_NWRHEADS_LIMIT = 8,
  KR_TRACK_NHEADS_LIMIT = 9,
  KR_TRACK_NSECTORS_LIMIT = 10,
  KR_STUDIO_NPATHS_LIMIT = 11,
  KR_STUDIO_NPROCS_LIMIT = 12,
  KR_DEMUXER_NTRACKS_LIMIT = 13,
  KR_LIMIT_LAST = 14
} kr_limit_type;

static const kr_limits_info kr_limits = {
  .len = 14,
  .limits = {
    [KR_RADIO_CLIENTS_MAX_LIMIT] = {
      .label = "RADIO_CLIENTS_MAX",
      .value = 128
    },
    [KR_MIXER_NPATHS_LIMIT] = {
      .label = "MIXER_NPATHS",
      .value = 16
    },
    [KR_XFER_NPATHS_LIMIT] = {
      .label = "XFER_NPATHS",
      .value = KR_RADIO_CLIENTS_MAX
    },
    [KR_TRANSMIT_CLIENTS_MAX_LIMIT] = {
      .label = "TRANSMIT_CLIENTS_MAX",
      .value = 64
    },
    [KR_WEB_NPATHS_LIMIT] = {
      .label = "WEB_NPATHS",
      .value = KR_RADIO_CLIENTS_MAX
    },
    [KR_XPDR_NPATHS_LIMIT] = {
      .label = "XPDR_NPATHS",
      .value = 16
    },
    [KR_TRACKER_NTRACKS_LIMIT] = {
      .label = "TRACKER_NTRACKS",
      .value = 16
    },
    [KR_TRACK_NRDHEADS_LIMIT] = {
      .label = "TRACK_NRDHEADS",
      .value = 64
    },
    [KR_TRACK_NWRHEADS_LIMIT] = {
      .label = "TRACK_NWRHEADS",
      .value = 4
    },
    [KR_TRACK_NHEADS_LIMIT] = {
      .label = "TRACK_NHEADS",
      .value = KR_TRACK_NRDHEADS + KR_TRACK_NWRHEADS
    },
    [KR_TRACK_NSECTORS_LIMIT] = {
      .label = "TRACK_NSECTORS",
      .value = 1
    },
    [KR_STUDIO_NPATHS_LIMIT] = {
      .label = "STUDIO_NPATHS",
      .value = 16
    },
    [KR_STUDIO_NPROCS_LIMIT] = {
      .label = "STUDIO_NPROCS",
      .value = 16
    },
    [KR_DEMUXER_NTRACKS_LIMIT] = {
      .label = "DEMUXER_NTRACKS",
      .value = 1
    },
  }
};

#endif