#include <krad/radio/version.h>
#include <krad/radio/limits.h>
#include <krad/mem/pool.h>
#include <krad/app/types.h>
#include <krad/io/buffer.h>
#include <krad/hash/sha1.h>
#include <krad/hash/base64.h>
#include <krad/loop/loop.h>
#include <krad/codec/header.h>
#include <krad/video/frame.h>

#include "types.h"

#if !defined(_track_tracker_H)
# define _track_tracker_H (1)

typedef struct kr_tracker kr_tracker;
typedef struct kr_track kr_track;
typedef struct kr_track_head kr_track_head;
typedef struct kr_track_event kr_track_event;
typedef int (kr_track_event_cb)(kr_track_event *);
typedef int (kr_track_erase_cb)(void *head_user, void *block_user);

typedef struct {
  int ntracks;
  void *user;
  kr_track_event_cb *track_ev_cb;
  kr_loop *master_loop;
} kr_tracker_setup;

struct kr_track_event {
  kr_track_event_type type;
  void *user;
  kr_tracker *tracker;
  kr_track *track;
  kr_track_info info;
};

typedef struct {
  kr_track_info *info;
  void *user;
  kr_codec_header header;
} kr_track_setup;

typedef enum {
  KR_TRACK_WRITE_HEAD = 1,
  KR_TRACK_READ_HEAD,
} kr_track_head_type;

typedef struct {
  kr_track_head_type type;
  void *user;
  kr_track_event_cb *track_ev_cb;
  kr_track_erase_cb *erase_cb;
  int seek;
} kr_track_head_setup;

typedef enum {
  KR_CONTAINER_BLOCK = 1,
  KR_CODEC_BLOCK_AUDIO,
  KR_CODEC_BLOCK_VIDEO,
  KR_MUXED_BLOCK
} kr_muxed_track_block_type;

typedef struct {
  void *user;
  kr_muxed_track_block_type type;
  union {
    kr_coded_audio coded_audio;
    kr_audio audio;
    kr_frame frame;
    uint8_t buf[256];
  };
  struct {
    uint8_t *data;
    size_t len;
  };
  int sync;
} kr_track_block;

int kr_track_codec_header_set(kr_track_head *head, kr_codec_header *header);
int kr_track_codec_header_get(kr_track_head *head, kr_codec_header *header);
int kr_track_read(kr_track_head *head, kr_track_block *block);
int kr_track_read_lite(kr_track_head *head, kr_track_block *block);
int kr_track_write(kr_track_head *head, kr_track_block *block);
int kr_track_close(kr_track_head *head);
kr_track_head *kr_track_open(kr_track *track, kr_track_head_setup *setup);
int kr_track_head_offset(kr_track_head *head);
size_t kr_track_name_get(kr_track *track, char *buf, size_t sz);
kr_track *kr_tracker_iter(kr_tracker *tracker, int *count);
kr_track *kr_track_find_by_name(kr_tracker *tracker, char *name, size_t len);
int kr_track_info_get(kr_track *track, kr_track_info *info);
int kr_track_remove(kr_track *track);
kr_track *kr_track_create(kr_tracker *tracker, kr_track_setup *setup);
int kr_tracker_loop_add(kr_tracker *tracker, kr_event *handler);
int kr_tracker_destroy(kr_tracker *tracker);
kr_tracker *kr_tracker_create(kr_tracker_setup *setup);

#endif
