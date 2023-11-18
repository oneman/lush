#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <krad/mem/mem.h>
#include <krad/app/seq.h>
#include <krad/container/ogg/pack.h>
#include <krad/container/ogg/unpack.h>

#include <krad/track/tracker.h>

#define MAX_TRACKS 8
#define MAX_HEADS 8

typedef struct {
  kr_track *track;
  kr_track_head *rhead;
  kr_track_head *whead;
} track_test_st;

typedef struct {
  int out_fd;
  kr_ogg_muxer *ogg;
  uint8_t *buffer;
  size_t sz;
  kr_tracker *tracker;
  track_test_st tracks[MAX_TRACKS];
  uint8_t ntracks;
} tracker_test_st;

static int track_ev(kr_track_event *e);

static int ogg_track_mux(tracker_test_st *tracker_st, kr_track_head *head) {
  int ret;
  kr_track_block block;
  //kr_codec_packet pkt;
  //kr_ogg_packer_input pinput;
  memset(&block, 0, sizeof(block));
  ret = kr_track_read(head, &block);
  if (ret != 1) {
    printf("ret is %d\n", ret);
    return -1;
  }
  //printf("Read block of sz %zu\n", block.sz);
  //pkt.buf = block.data;
  //pkt.sz = block.sz;
  //pkt.len = block.sz;
  //pinput.pkt = &pkt;
  //pinput.granule_pos = block.tc;
  //pinput.track = 0;
  //ret = kr_ogg_pack(tracker_st->ogg, tracker_st->buffer,
  // tracker_st->sz, &pinput);
  if (ret <= 0) {
    printf("mmh\n");
    return -1;
  }
  printf("packed %d bytes of page\n", ret);
  ret = write(tracker_st->out_fd, tracker_st->buffer, ret);
  if (ret < 0) {
    printf("bad write!!!\n");
    return -1;
  }
  return 0;
}

static int find_track_no(track_test_st *tracks, kr_track *track) {
  int i;
  for (i = 0; i < MAX_TRACKS; i++) {
    if (tracks[i].track == track) return i;
  }
  return -1;
}

static int muxer_init(tracker_test_st *tracker_st, uint8_t track_no) {
  kr_track_head_setup head_setup;
  kr_track_head *rhead;
  memset(&head_setup, 0, sizeof(head_setup));
  head_setup.user = tracker_st;
  head_setup.type = KR_TRACK_READ_HEAD;
  head_setup.track_ev_cb = track_ev;
  rhead = kr_track_open(tracker_st->tracks[track_no].track, &head_setup);
  if (!rhead) return -1;
  tracker_st->tracks[track_no].rhead = rhead;
  return 0;
}

static int track_ev(kr_track_event *e) {
  tracker_test_st *tracker_st;
  int ret;
  if (e->type == KR_TRACK_ADD) {
    printf("Tracker add track event fired\n");
    return 0;
  }
  if (e->type == KR_TRACK_REMOVE) {
    printf("Tracker rm track event fired\n");
    return 0;
  }
  if (e->type == KR_TRACK_READ) {
    printf("Track read event fired\n");
  }
  if (e->type == KR_TRACK_WRITE) {
    printf("Track write event fired\n");
    tracker_st = e->user;
    ret = find_track_no(tracker_st->tracks, e->track);
    if (ret < 0) return -1;
    ret = ogg_track_mux(tracker_st, tracker_st->tracks[ret].rhead);
    if (ret != 0) return -1;
  }
  return 0;
}

static int ogg_track_demux(tracker_test_st *tracker_st, char const *fpath) {
  int i;
  int fd;
  int status;
  size_t size;
  struct stat s;
  uint8_t *mapped;
  ssize_t ret;
  size_t n;
  kr_track_head_setup head_setup;
  kr_ogg_unpacker *u;
  kr_ogg_unpacker_setup unpack_setup;
  kr_ogg_pkt_info info;
  kr_codec_packet pkts[8];
  kr_track_block block;
  kr_track *track;
  kr_track_head *whead;
  kr_track_info track_info;
  kr_track_setup track_setup;
  memset(&track_setup, 0, sizeof(track_setup));
  memset(&head_setup, 0, sizeof(head_setup));
  memset(&track_info, 0, sizeof(track_info));
  fd = open (fpath, O_RDONLY);
  if (fd < 0) return -1;
  status = fstat (fd, & s);
  if (status < 0) return -1;
  size = s.st_size;
  mapped = mmap (NULL, size, PROT_READ, MAP_PRIVATE | MAP_NONBLOCK, fd, 0);
  if (mapped == MAP_FAILED) {
    fprintf(stderr, "File: mmap %s\n", strerror(errno));
    close(fd);
    return -1;
  }
  memset(pkts, 0, sizeof(kr_codec_packet) * 8);
  for (i = 0; i < 8; i++) {
    pkts[i].sz = 8192 * 8;
    pkts[i].buf = kr_alloc(pkts[i].sz);
  }
  unpack_setup.mem = kr_alloc(kr_ogg_unpacker_sizeof());
  u = kr_ogg_unpacker_init(&unpack_setup);
  if (!u) return -1;
  kr_ogg_unpacker_output_set(u, pkts, 8);
  n = 0;
  for (;;) {
    ret = kr_ogg_unpack(u, &info, mapped, size);
    if (ret < 0) return -1;
    if (ret == 1) break;
    //printf("  pkt num %"PRIu64", sz %zu, track %u, hdr %u, codec %d\n",
     //info.num, info.sz, info.track, info.hdr, info.codec);
    if (info.hdr) {
      //printf("  pkt len %zu, buf %.*s\n", pkts[info.track].len, 8,
        //pkts[info.track].buf);
      track_info.type = KR_TRACK_CODED_AUDIO;
      track_info.mode = KR_TRACK_FINITE;
      track_info.duration = 42;
      track_setup.user = tracker_st;
      track_setup.info = &track_info;
      track = kr_track_create(tracker_st->tracker, &track_setup);
      if (!track) return -1;
      tracker_st->tracks[info.track].track = track;
      tracker_st->ntracks++;
      head_setup.user = tracker_st;
      head_setup.type = KR_TRACK_WRITE_HEAD;
      head_setup.track_ev_cb = track_ev;
      whead = kr_track_open(track, &head_setup);
      if (!whead) return -1;
      tracker_st->tracks[info.track].whead = whead;
      printf("NEW TRACK %d CREATED\n", info.track);
      ret = muxer_init(tracker_st, info.track);
      if (ret != 0) return -1;
    }
    memset(&block, 0, sizeof(block));
    //block.sz = pkts[info.track].len;
    //block.data = pkts[info.track].buf;
    //block.tc = info.granule_pos;
    ret = kr_track_write(tracker_st->tracks[info.track].whead, &block);
    if (ret != 0) {
      printf("kr_track_write error\n");
      return -1;
    }
    n++;
  }
  munmap(mapped, size);
  close(fd);
  for (i = 0; i < 8; i++) {
    free(pkts[i].buf);
  }
  free(u);
  return 0;
}

static int tracker_test(char const *out_path, char const *in_path) {
  int ret;
  int i;
  tracker_test_st tracker_st;
  kr_tracker *tracker;
  kr_tracker_setup tracker_setup;
  void *user;
  memset(&tracker_st, 0, sizeof(tracker_st));
  memset(&tracker_setup, 0, sizeof(tracker_setup));
  user = "monkey";
  tracker_setup.ntracks = 32;
  tracker_setup.user = user;
  //tracker_setup.tracker_ev_cb = tracker_ev;
  tracker_setup.track_ev_cb = track_ev;
  tracker_st.ogg = kr_alloc(kr_ogg_muxer_sizeof());
  tracker_st.sz = 256 * 256;
  tracker_st.buffer = kr_alloc(tracker_st.sz);
  tracker = kr_tracker_create(&tracker_setup);
  if (!tracker) return -1;
  tracker_st.tracker = tracker;
  tracker_st.out_fd = open (out_path, O_CREAT | O_EXCL | O_RDWR, 0640);
  if (tracker_st.out_fd < 0) return -1;
  ret = ogg_track_demux(&tracker_st, in_path);
  if (ret != 0) return -1;
  for (i = 0; i < MAX_TRACKS; i++) {
    if (tracker_st.tracks[i].rhead)
      kr_track_close(tracker_st.tracks[i].rhead);
    if (tracker_st.tracks[i].whead)
      kr_track_close(tracker_st.tracks[i].whead);
    if (tracker_st.tracks[i].track)
      kr_track_remove(tracker_st.tracks[i].track);
  }
  kr_tracker_destroy(tracker);
  free(tracker_st.ogg);
  free(tracker_st.buffer);
  close(tracker_st.out_fd);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 3) {
    fprintf(stderr, "Give me an ogg file and an output file\n");
    return -1;
  }
  ret = tracker_test(argv[2], argv[1]);
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
