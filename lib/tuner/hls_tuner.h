#if !defined(_tuner_hls_H)
# define _tuner_hls_H (1)

#include <krad/xfer/xfer.h>
#include "hls_types.h"

typedef enum {
  KR_HLS_TUNER_MASTER = 1,
  KR_HLS_TUNER_MEDIA
} kr_hls_tuner_event_type;

typedef struct {
  kr_hls_tuner_event_type type;
  void *user;
} kr_hls_tuner_event;

typedef int (kr_hls_tuner_event_cb)(kr_hls_tuner_event *);

typedef struct {
  char *url;
  size_t url_len;
  kr_xfer *xfer;
  kr_hls_tuner_event_cb *event_cb;
  void *user;
} kr_hls_tuner_setup;

typedef struct kr_hls_tuner kr_hls_tuner;

int kr_hls_tuner_tune_out(kr_hls_tuner *tuner, kr_hls_playlist *plst);
int kr_hls_tuner_tune_in(kr_hls_tuner *tuner, kr_hls_playlist *plst);
int kr_hls_tuner_get_info(kr_hls_tuner *tuner, kr_hls_tuner_info *info);
int kr_hls_tuner_destroy(kr_hls_tuner *tuner);
kr_hls_tuner *kr_hls_tuner_create(kr_hls_tuner_setup *setup);
int m3u_to_hls(kr_hls_playlist *plst, kr_m3u_info *info);

#endif
