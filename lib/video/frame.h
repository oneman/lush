#include <stdlib.h>
#include <sys/time.h>

typedef struct kr_frame kr_frame;
typedef struct kr_frame_info kr_frame_info;

#if !defined(_video_frame_H)
# define _video_frame_H (1)

#include <krad/image/image.h>

struct kr_frame_info {
  union {
    uint64_t tc;
    struct timeval ts;
  };
  uint8_t keyframe;
};

struct kr_frame {
  kr_frame_info info;
  kr_image_type type;
  union {
    kr_coded_image coded;
    kr_image image;
  };
};

void kr_frame_debug_time(kr_frame *frame, char *str, size_t sz);
uint64_t kr_frame_addtime(kr_frame *frame, uint64_t tc);
int kr_frame_copytime(kr_frame *out, kr_frame *in);

#endif
