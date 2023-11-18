#include <inttypes.h>

typedef struct kr_av_event kr_av_event;

typedef int (kr_av_release_cb)(void *ptr);
typedef int (kr_av_handler)(kr_av_event *);

#if !defined(_xpdr_event_H)
# define _xpdr_event_H (1)

#include <krad/audio/audio.h>
#include <krad/video/frame.h>
#include <krad/image/image.h>

struct kr_av_event {
  union {
    kr_frame *frame;
    kr_audio *audio;
  };
  void *user;
};

typedef union {
  kr_frame *frame;
  kr_audio *audio;
} kr_av;

#endif
