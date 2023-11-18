#include <stdlib.h>
#include <time.h>
#include <krad/app/debug.h>
#include <krad/loop/loop.h>
#include <krad/app/spinlock.h>
#include <krad/app/seq.h>
#include <krad/app/types.h>
#include "tracker.h"

#define KR_TRACK_NHEADERS 3

struct kr_tracker {
  kr_loop *loop;
  kr_pool *tracks;
  void *user;
  kr_track_event_cb *track_ev_cb;
};

typedef struct kr_track_sector kr_track_sector;

struct kr_track_sector {
  kr_seq *seq;
  kr_amem(mem, 16)
  kr_track_block *block;
  uint64_t last_sync;
};

struct kr_track_head {
  kr_track *track;
  kr_track_head_type type;
  kr_track_sector *sector;
  void *user;
  kr_seq *seq;
  kr_amem(mem, 16)
  kr_track_event_cb *track_ev_cb;
  kr_track_erase_cb *erase_cb;
};

struct kr_track {
  kr_tracker *tracker;
  void *user;
  kr_track_head head[KR_TRACK_NHEADS];
  kr_track_sector sector[KR_TRACK_NSECTORS];
  kr_spinlock lock;
  uint64_t muxed_video;
  kr_track_info info;
  kr_codec_header header;
};

static void run_track_event(kr_track *track,
 kr_track_event_type type);
static kr_track_sector *find_sector(kr_track *track);
static kr_track_sector *get_free_sector(kr_track *track);
static void sector_reap(kr_track_head *head);

static const kr_track_head_type head_type_for_event_type[] = {
  [KR_TRACK_READ] = KR_TRACK_WRITE_HEAD,
  [KR_TRACK_WRITE] = KR_TRACK_READ_HEAD
};

static void run_track_event(kr_track *track,
 kr_track_event_type type) {
  int i;
  kr_track_event event;
  kr_track_head_type event_head_type;
  kr_track_head *head;
  event.user = track->tracker->user;
  event.tracker = track->tracker;
  event.track = track;
  event.info = track->info;
  event.type = type;
  if (type == KR_TRACK_ADD || type == KR_TRACK_REMOVE) {
    track->tracker->track_ev_cb(&event);
    if (type == KR_TRACK_ADD) return;
    for (i = 0; i < KR_TRACK_NHEADS; i++) {
      head = &track->head[i];
      if (!head->track_ev_cb) continue;
      event.user = head->user;
      head->track = NULL;
      head->user = NULL;
      head->track_ev_cb(&event);
    }
    return;
  }
  if (type != KR_TRACK_READ && type != KR_TRACK_WRITE) {
    printke("Tracker: Unknown event, what could it be?");
    return;
  }
  event_head_type = head_type_for_event_type[type];
  /* FIXME fire event only for heads on the same sector */
  for (i = 0; i < KR_TRACK_NHEADS; i++) {
    head = &track->head[i];
    if (head->type != event_head_type) continue;
    if (!head->track_ev_cb) continue;
    event.user = head->user;
    head->track_ev_cb(&event);
  }
}

static kr_track_sector *find_sector(kr_track *track) {
  kr_track_sector *sector;
  /* fixme.. we just return the first one for the moment */
  sector = &track->sector[0];
  return sector;
}

static kr_track_sector *get_free_sector(kr_track *track) {
  int i;
  int j;
  int used;
  kr_track_sector *sector;
  for (i = 0; i < KR_TRACK_NSECTORS; i++) {
    used = 0;
    sector = &track->sector[i];
    for (j = 0; j < KR_TRACK_NHEADS; j++) {
      if (track->head[j].sector == sector) {
        used = 1;
        break;
      }
    }
    if (!used) {
      return sector;
    }
  }
  return NULL;
}

static void sector_reap(kr_track_head *head) {
  uint64_t blocknum;
  kr_track_sector *sector;
  sector = head->sector;
  if (!kr_seq_horizon(sector->seq, &blocknum)) return;
  if (head->erase_cb) {
    head->erase_cb(head->user, sector->block[blocknum].user);
  }
  sector->block[blocknum].sync = 0;
}

static size_t sector_search(kr_track_sector *sec, kr_track_block_type type, size_t seek) {
  /* Ok so this works for AUDIO ONLY, need slightly more effort to extend to video
     but not needed now */
  int i;
  size_t hz;
  size_t len;
  size_t slen;
  size_t rate;
  ssize_t s_tc;
  ssize_t e_tc;
  ssize_t tc_diff;
  ssize_t tc;
  ssize_t start_bn;
  ssize_t end_bn;
  if (type == KR_TRACK_MUX) {
    hz = 0;
    end_bn = 0;
    start_bn = 0;
    slen = kr_seq_len(sec->seq);
    kr_seq_horizon(sec->seq, &hz);
    if (hz) start_bn = hz - 1;
    end_bn = kr_seq_prev(sec->seq);
    i = start_bn;
    while (i != end_bn) {
      if (i == slen) i = 0;
      if (sec->block[i].type == KR_CODEC_BLOCK_AUDIO) {
        printkd("Start codec block found!");
        start_bn = i;
        break;
      }
      i++;
    }
    i = end_bn;
    while (i != start_bn) {
      if (i == -1) i = slen - 1;
      if (sec->block[i].type == KR_CODEC_BLOCK_AUDIO) {
        printkd("End codec block found!");
        end_bn = i;
        break;
      }
      i--;
    }
    s_tc = sec->block[start_bn].coded_audio.tc;
    e_tc = sec->block[end_bn].coded_audio.tc;
    rate = sec->block[start_bn].coded_audio.rate;
    if (!rate || !e_tc) {
      printke("Tracker: wrong rate/timecode %zu/%llu", rate, e_tc);
      return 0;
    }
    tc_diff = e_tc - s_tc;
    if (tc_diff >= 0) {
      len = (size_t)( (double)(tc_diff * 1000) / (double)rate );
      if (len > seek) {
        tc = e_tc - ((seek * rate) / 1000);
        i = end_bn;
        seek = 0;
        while (i != start_bn) {
          if (i == -1) i = slen - 1;
          if (sec->block[i].type == KR_CODEC_BLOCK_AUDIO) {
            if (sec->block[i].coded_audio.tc <= tc) seek = 1;
          }
          if (seek && sec->block[i].sync) {
            return ((kr_seq_total_pos(sec->seq) - i - 1) % slen);
          }
          i--;
        }
      } else {
        printke("Tracker: track not long enough to seek that far back.");
      }
    } else printke("Tracker: sector_length fail!");
  }
  return 0;
}

static int video_mux_read_sync(kr_track_head *head, int64_t seek) {
  kr_spin_lock(&head->track->lock);
  kr_seq_reset(head->seq);
  kr_seq_seek(head->seq, head->sector->last_sync);
  kr_spin_unlock(&head->track->lock);
  return 0;
}

static int video_mux_read_head(kr_track_head *head) {
  if (head->type != KR_TRACK_READ_HEAD
   || head->track->info.type != KR_TRACK_MUX) {
    return 0;
  }
  return head->track->muxed_video;
}

static int find_sync_block(kr_track_sector *sec, int seek) {
  int i;
  size_t len;
  size_t tpos;
  len = kr_seq_len(sec->seq);
  tpos = kr_seq_total_pos(sec->seq) - 1;
  for (i = seek; i < tpos; i++) {
    if (sec->block[i % len].sync) {
      seek = i;
      break;
    }
  }
  if (i == tpos) {
    printke("Tracker: could not find sync block! %d/%d", seek, tpos);
  }
  return seek;
}

static int head_seek(kr_track_head *head, int64_t seek) {
  size_t ret;
  size_t tpos;
  size_t len;
  if (!head) return -1;
  if (head->type != KR_TRACK_READ_HEAD) return -2;
  kr_spin_lock(&head->track->lock);
  tpos = kr_seq_total_pos(head->sector->seq);
  len = kr_seq_len(head->sector->seq);
  if (tpos < 1) {
    kr_spin_unlock(&head->track->lock);
    return 0;
  }
  tpos--;
  if (seek > 0) {
    ret = sector_search(head->sector, head->track->info.type, seek);
    seek = tpos - ret;
    if (seek == tpos) seek = tpos;
  } else if (seek < 0) {
    if (tpos <= len) seek = 0;
    else seek = tpos - len;
  } else if (seek == 0) {
    seek = tpos;
  }
  seek = find_sync_block(head->sector, seek);
  printk("seeking %d/%d", seek, tpos);
  kr_seq_reset(head->seq);
  kr_seq_seek(head->seq, seek);
  kr_spin_unlock(&head->track->lock);
  return 0;
}

int kr_track_head_offset(kr_track_head *head) {
  int a;
  int b;
  if (!head) return -1;
  a = kr_seq_total_pos(head->sector->seq);
  b = kr_seq_total_pos(head->seq);
  if (a < 0 || b < 0) return -2;
  if (a < b) return -3;
  return a - b;
}

kr_track *kr_tracker_iter(kr_tracker *tracker, int *count) {
  kr_track *track;
  if (!tracker || !count) return NULL;
  while ((track = kr_pool_iterate_active(tracker->tracks, count))) {
    return track;
  }
  return NULL;
}

kr_track *kr_track_find_by_name(kr_tracker *tracker, char *name, size_t len) {
  int i;
  kr_track *track;
  if (!tracker || !name || !len) return NULL;
  i = 0;
  while ((track = kr_pool_iterate_active(tracker->tracks, &i))) {
    if (track->info.track.len == len
     && !memcmp(track->info.track.name, name, len))
     return track;
  }
  return NULL;
}

int kr_track_codec_header_get(kr_track_head *head, kr_codec_header *header) {
  if (!head || !header) return -1;
  *header = head->track->header;
  return 0;
}

int kr_track_codec_header_set(kr_track_head *head, kr_codec_header *header) {
  if (!head || !header) return -1;
  head->track->header = *header;
  return 0;
}

int kr_track_read_lite(kr_track_head *head, kr_track_block *block) {
  int ret;
  int res;
  uint64_t blocknum;
  kr_track_sector *sector;
  if (!head || !block) return -1;
  if (head->type != KR_TRACK_READ_HEAD) return -2;
  ret = 0;
  sector = head->sector;
  kr_spin_lock(&head->track->lock);
  res = kr_seq_sync_check(sector->seq, kr_seq_total_pos(head->seq));
  if (res) {
    printke("Tracker: read head behind by %d blocks, fixing", res);
    kr_seq_seek(head->seq, res);
  }
  blocknum = kr_seq_pos(head->seq);
  if (blocknum == kr_seq_pos(sector->seq)) {
    ret = 0;
  } else {
    block->data = sector->block[blocknum].data;
    block->len = sector->block[blocknum].len;
    kr_seq_advance(head->seq);
    ret = 1;
  }
  kr_spin_unlock(&head->track->lock);
  return ret;
}

int kr_track_read(kr_track_head *head, kr_track_block *block) {
  int ret;
  int res;
  uint64_t blocknum;
  kr_track_sector *sector;
  if (!head || !block) return -1;
  if (head->type != KR_TRACK_READ_HEAD) return -2;
  ret = 0;
  sector = head->sector;
  kr_spin_lock(&head->track->lock);
  res = kr_seq_sync_check(sector->seq, kr_seq_total_pos(head->seq));
  if (res) {
    printke("Tracker: read head behind by %d blocks, fixing", res);
    kr_seq_seek(head->seq, res);
  }
  blocknum = kr_seq_pos(head->seq);
  if (blocknum == kr_seq_pos(sector->seq)) {
    ret = 0;
  } else {
    *block = sector->block[blocknum];
    kr_seq_advance(head->seq);
    run_track_event(head->track, KR_TRACK_READ);
    ret = 1;
  }
  kr_spin_unlock(&head->track->lock);
  return ret;
}

int kr_track_write(kr_track_head *head, kr_track_block *block) {
  kr_track_sector *sector;
  uint64_t blocknum;
  if (!head || !block) return -1;
  if (head->type != KR_TRACK_WRITE_HEAD) return -2;
  sector = head->sector;
  kr_spin_lock(&head->track->lock);
  sector_reap(head);
  blocknum = kr_seq_pos(sector->seq);
  sector->block[blocknum] = *block;
  if (!head->track->muxed_video) {
    if (block->type == KR_CODEC_BLOCK_VIDEO) head->track->muxed_video = 1;
  }
  if (sector->block[blocknum].type == KR_CONTAINER_BLOCK) {
    sector->block[blocknum].data = sector->block[blocknum].buf;
  }
  if (block->sync) sector->last_sync = kr_seq_total_pos(sector->seq);
  kr_seq_advance(sector->seq);
  kr_spin_unlock(&head->track->lock);
  run_track_event(head->track, KR_TRACK_WRITE);
  return 0;
}

int kr_track_close(kr_track_head *head) {
  if (!head) return -1;
  head->user = NULL;
  head->track_ev_cb = NULL;
  return 0;
}

kr_track_head *kr_track_open(kr_track *track, kr_track_head_setup *setup) {
  int i;
  int ret;
  uint64_t pos;
  kr_track_head *head;
  head = NULL;
  if (!track || !setup || !setup->user) return NULL;
  if (setup->type == KR_TRACK_WRITE_HEAD) {
    for (i = 0; i < KR_TRACK_NHEADS; i++) {
      head = &track->head[i];
      if (head->user == NULL) continue;
      if (head->type == KR_TRACK_WRITE_HEAD) return NULL;
    }
  }
  head = NULL;
  for (i = 0; i < KR_TRACK_NHEADS; i++) {
    if (track->head[i].user != NULL) continue;
    head = &track->head[i];
    break;
  }
  if (!head) return NULL;
  head->user = setup->user;
  head->type = setup->type;
  //head->track_ev_cb = setup->track_ev_cb;
  if (head->type == KR_TRACK_WRITE_HEAD && setup->erase_cb) {
    head->erase_cb = setup->erase_cb;
  } else {
    head->erase_cb = NULL;
  }
  head->sector = NULL;
  head->track = track; /* fixme, containerof? */
  if (head->type == KR_TRACK_WRITE_HEAD) {
    head->sector = get_free_sector(head->track);
  } else {
    head->sector = find_sector(head->track);
    if (kr_seq_sizeof() > sizeof(head->mem)) {
      printke("Tracker: mem size fail!");
      exit(1);
    }
    head->seq = (kr_seq *)head->mem;
    printkd("track open duration: %d ol nblocks 30", track->info.duration);
    kr_seq_init(head->seq, track->info.duration);
    pos = kr_seq_total_pos(head->sector->seq);
    if (head->track->info.type == KR_TRACK_MUX) {
      if (video_mux_read_head(head)) {
        video_mux_read_sync(head, setup->seek);
      } else {
        ret = head_seek(head, setup->seek);
        if (ret != 0) {
          printke("Tracker: head_seek failed %d", ret);
        }
      }
    } else {
      kr_seq_seek(head->seq, pos);
    }
  }
  head->track_ev_cb = setup->track_ev_cb;
  return head;
}

int kr_track_info_get(kr_track *track, kr_track_info *info) {
  if (!track || !info) return -1;
  *info = track->info;
  return 0;
}

size_t kr_track_name_get(kr_track *track, char *buf, size_t sz) {
  if (!track || !buf || !sz) return -1;
  if (sz < track->info.track.len) return -2;
  memcpy(buf, track->info.track.name, track->info.track.len);
  return track->info.track.len;
}

int kr_track_remove(kr_track *track) {
  int i;
  int ret;
  kr_tracker *tracker;
  if (!track) return -1;
  tracker = track->tracker;
  run_track_event(track, KR_TRACK_REMOVE);
  for (i = 0; i < KR_TRACK_NSECTORS; i++) {
    free(track->sector[i].block);
  }
  ret = kr_pool_release(tracker->tracks, track);
  return ret;
}

kr_track *kr_track_create(kr_tracker *tracker, kr_track_setup *setup) {
  int i;
  kr_track *track;
  kr_track_sector *sector;
  if (!tracker || !setup || !setup->info || !setup->user) return NULL;
  if ((setup->info->type == KR_TRACK_CODED_VIDEO
   || setup->info->type == KR_TRACK_CODED_AUDIO)
   && (setup->header.len > KR_TRACK_NHEADERS)) return NULL;
  track = kr_pool_slice(tracker->tracks);
  if (!track) return NULL;
  memset(track, 0, sizeof(*track));
  track->tracker = tracker;
  track->user = setup->user;
  track->info = *setup->info;
  kr_spin_init(&track->lock);
  if (track->info.type == KR_TRACK_CODED_VIDEO
   || track->info.type == KR_TRACK_CODED_AUDIO) {
    track->header = setup->header;
  }
  for (i = 0; i < KR_TRACK_NSECTORS; i++) {
    sector = &track->sector[i];
    printk("Tracker: duration: %d ol-nblock %d", track->info.duration, 30);
    sector->block = kr_allocz(track->info.duration, sizeof(kr_track_block));
    if (kr_seq_sizeof() > sizeof(sector->mem)) {
      printke("Tracker: mem size fail!");
      exit(1);
    }
    sector->seq = (kr_seq *)sector->mem;
    kr_seq_init(sector->seq, track->info.duration);
  }
  run_track_event(track, KR_TRACK_ADD);
  return track;
}

int kr_tracker_loop_add(kr_tracker *tracker, kr_event *handler) {
  return kr_loop_add(tracker->loop, handler);
}

int kr_tracker_destroy(kr_tracker *tracker) {
  int i;
  kr_track *track;
  if (!tracker) return -1;
  printk("Tracker: Destroying");
  i = 0;
  while ((track = kr_pool_iterate_active(tracker->tracks, &i))) {
    kr_track_remove(track);
  }
  kr_loop_destroy(tracker->loop);
  kr_pool_destroy(tracker->tracks);
  printk("Tracker: Destroyed");
  return 0;
}

kr_tracker *kr_tracker_create(kr_tracker_setup *setup) {
  kr_tracker *tracker;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  kr_loop_setup loop_setup;
  if (!setup) return NULL;
  if (setup->ntracks < 1) return NULL;
  if (!setup->user) return NULL;
  //if (!setup->event_cb) return NULL;
  printk("Tracker: Creating");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*tracker);
  pool_setup.size = sizeof(kr_track);
  pool_setup.slices = setup->ntracks;
  pool = kr_pool_create(&pool_setup);
  if (!pool) return NULL;
  tracker = kr_pool_overlay_get(pool);
  memset(tracker, 0, sizeof(*tracker));
  tracker->tracks = pool;
  tracker->user = setup->user;
  tracker->track_ev_cb = setup->track_ev_cb;
  memset(&loop_setup, 0, sizeof(loop_setup));
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_tracker");
  loop_setup.master = setup->master_loop;
  tracker->loop = kr_loop_create(&loop_setup);
  printk("Tracker: Created");
  return tracker;
}
