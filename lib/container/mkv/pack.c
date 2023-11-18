#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <krad/ebml/ebml.h>
#include <krad/io/buffer.h>
#include <krad/io/file.h>
#include <krad/io/stream.h>
#include <krad/image/image.h>
#include <krad/radio/version.h>
#include <krad/app/debug.h>
#include "ids.h"
#include "pack.h"

#define KR_MKV_VERSION "Krad MKV Version 8"
#define KR_MKV_TRACK_HEADERS_MAX 3

typedef enum {
  MKV_TRACK_VIDEO = 1,
  MKV_TRACK_AUDIO = 2,
  MKV_TRACK_SUBTITLE = 0x11
} mkv_track_type;

typedef struct {
  uint8_t num;
  uint64_t uid;
  kr_codec codec;
  mkv_track_type type;
  uint64_t frames;
  int64_t timecode;
  kr_track_info info;
  kr_codec_header codec_header;
} mkv_track;

struct kr_mkv_muxer {
  mkv_track tracks[KR_MKV_MUXER_NTRACKS];
  int ntracks;
  int audio_only;
  int64_t tc_scale;
  int64_t cluster_tc;
  int64_t segment_tc;
  int64_t segment_duration;
  kr_ebml ebml;
  uint64_t nblocks;
  uint8_t *segment;
  uint8_t *cluster;
  uint8_t hdr[8192];
  int hdr_len;
};

static char *kr_codec_to_mkv_codec(kr_codec codec) {
  if (codec.type == KR_CODEC_VIDEO) {
    switch (codec.video) {
      case KR_VIDEO_CODEC_DAALA: return "V_DAALA";
      case KR_VIDEO_CODEC_VPX: return "V_VP8";
      case KR_VIDEO_CODEC_H264: return "V_MPEG4/ISO/AVC";
      case KR_VIDEO_CODEC_THEORA: return "V_THEORA";
      case KR_VIDEO_CODEC_Y4M: return "V_Y4M";
      default: break;
    }
  }
  if (codec.type == KR_CODEC_AUDIO) {
    switch (codec.audio) {
      case KR_AUDIO_CODEC_FLAC: return "A_FLAC";
      case KR_AUDIO_CODEC_OPUS: return "A_OPUS";
      case KR_AUDIO_CODEC_VORBIS: return "A_VORBIS";
      case KR_AUDIO_CODEC_AAC: return "A_AAC/MPEG4/LC/SBR";
      default: break;
    }
  }
  return "No Codec";
}

static int gen_track_uid(int track) {
  uint64_t t;
  uint64_t r;
  uint64_t rval;
  t = time(NULL) * track;
  r = rand();
  r = r << 32;
  r += rand();
  rval = t ^ r;
  return rval;
}

static void pack_codec_header(kr_mkv_muxer *m, mkv_track *track) {
  int i;
  size_t codec_data_sz;
  codec_data_sz = 0;
  for (i = 0; i < track->codec_header.len; i++) {
    codec_data_sz += track->codec_header.data[i].sz;
  }
  if (codec_data_sz > 0) {
    kr_ebml_pack_element(&m->ebml, MKV_CODECDATA);
    kr_ebml_pack_data_size(&m->ebml, codec_data_sz);
    for (i = 0; i < track->codec_header.len; i++) {
      kr_ebml_pack(&m->ebml, track->codec_header.data[i].buf,
       track->codec_header.data[i].sz);
    }
  }
}

static int pack_video_track(kr_mkv_muxer *m, mkv_track *track) {
  uint8_t *track_info;
  uint8_t *video_info;
  kr_ebml2_start_element(&m->ebml, MKV_TRACK, &track_info);
  kr_ebml_pack_uint8(&m->ebml, MKV_TRACKNUMBER, track->num);
  kr_ebml_pack_uint64(&m->ebml, MKV_TRACK_UID, track->uid);
  kr_ebml_pack_string(&m->ebml, MKV_CODECID,
   kr_codec_to_mkv_codec(track->codec));
  /*kr_ebml_pack_uint8(&m->ebml, MKV_FLAG_LACING, 1);*/
  kr_ebml_pack_uint8(&m->ebml, MKV_TRACKTYPE, MKV_TRACK_VIDEO);
  kr_ebml2_start_element(&m->ebml, MKV_VIDEO, &video_info);
  kr_ebml_pack_uint16(&m->ebml, MKV_WIDTH, track->info.video.width);
  kr_ebml_pack_uint16(&m->ebml, MKV_HEIGHT, track->info.video.height);
  kr_ebml2_finish_element(&m->ebml, video_info);
  pack_codec_header(m, track);
  kr_ebml2_finish_element(&m->ebml, track_info);
  return 0;
}

static int pack_audio_track(kr_mkv_muxer *m, mkv_track *track) {
  uint8_t *track_info;
  uint8_t *audio_info;
  kr_ebml2_start_element(&m->ebml, MKV_TRACK, &track_info);
  kr_ebml_pack_uint8(&m->ebml, MKV_TRACKNUMBER, track->num);
  kr_ebml_pack_uint64(&m->ebml, MKV_TRACK_UID, track->uid);
  kr_ebml_pack_string(&m->ebml, MKV_CODECID,
   kr_codec_to_mkv_codec(track->codec));
  kr_ebml_pack_uint8(&m->ebml, MKV_TRACKTYPE, 2);
  kr_ebml2_start_element(&m->ebml, MKV_AUDIO, &audio_info);
  kr_ebml_pack_uint8(&m->ebml, MKV_CHANNELS, track->info.audio.channels);
  kr_ebml_pack_float(&m->ebml, MKV_SAMPLERATE,
   track->info.audio.sample_rate);
  if (track->info.audio.bit_depth > 0) {
    kr_ebml_pack_uint8(&m->ebml, MKV_BITDEPTH, track->info.audio.bit_depth);
  }
  kr_ebml2_finish_element(&m->ebml, audio_info);
  pack_codec_header(m, track);
  kr_ebml2_finish_element(&m->ebml, track_info);
  return 0;
}

static int pack_tracks(kr_mkv_muxer *m) {
  int i;
  int ret;
  uint8_t *tracks_info;
  mkv_track *track;
  kr_ebml2_start_element(&m->ebml, MKV_TRACKS, &tracks_info);
  for (i = 0; i < m->ntracks; i++) {
    track = &m->tracks[i];
    if (track->info.type == KR_TRACK_CODED_AUDIO) {
      ret = pack_audio_track(m, track);
    } else if (track->info.type == KR_TRACK_CODED_VIDEO) {
      ret = pack_video_track(m, track);
    } else {
      return -1;
    }
    if (ret) return -1;
  }
  kr_ebml2_finish_element(&m->ebml, tracks_info);
  return 0; 
}

static int pack_segment(kr_mkv_muxer *m, char *title) {
  uint8_t *segment_info;
  if (!title) title = "Krad Radio Live";
  m->tc_scale = 1000000;
  kr_ebml2_start_element(&m->ebml, MKV_SEGMENT, &m->segment);
  kr_ebml2_start_element(&m->ebml, MKV_SEGMENT_INFO, &segment_info);
  kr_ebml_pack_string(&m->ebml, MKV_SEGMENT_TITLE, title);
  kr_ebml_pack_uint64(&m->ebml, MKV_SEGMENT_TIMECODESCALE, m->tc_scale);
  kr_ebml_pack_string(&m->ebml, MKV_SEGMENT_MUXINGAPP, KR_MKV_VERSION);
  kr_ebml_pack_string(&m->ebml, MKV_SEGMENT_WRITINGAPP, KR_VERSION_STR_FULL);
  kr_ebml2_finish_element(&m->ebml, segment_info);
  return 0;
}

static int pack_header(kr_mkv_muxer *m, char *title, int type) {
  char *doctype;
  doctype = "matroska";
  if (type == KR_CONTAINER_WEBM) doctype = "webm";
  kr_ebml2_set_buffer(&m->ebml, m->hdr, sizeof(m->hdr));
  kr_ebml_pack_header(&m->ebml, doctype, 2, 2);
  pack_segment(m, title);
  pack_tracks(m);
  m->hdr_len = m->ebml.pos;
  return 0;
}

static int mkv_cluster(kr_mkv_muxer *m, int64_t timecode) {
  if (m->cluster != NULL) {
    kr_ebml2_finish_element(&m->ebml, m->cluster);
    m->cluster = NULL;
  }
  m->cluster_tc = timecode;
  kr_ebml2_start_element(&m->ebml, MKV_CLUSTER, &m->cluster);
  kr_ebml_pack_int64(&m->ebml, MKV_CLUSTER_TIMECODE, m->cluster_tc);
  return 0;
}

static ssize_t mkv_add_video(kr_mkv_muxer *m, mkv_track *track,
  kr_mkv_muxer_data *data) {
  uint32_t block_length;
  unsigned char track_number;
  unsigned short block_timecode;
  unsigned char flags;
  int64_t timecode;
  flags = 0;
  timecode = data->tc;
  block_timecode = 0;
  block_length = data->pkt_len + 4;
  block_length |= 0x10000000;
  track_number = track->num;
  track_number |= 0x80;
  if (track->frames) {
    timecode = round(1000000000 * track->frames / track->info.video.fps_num *
     track->info.video.fps_den / m->tc_scale);
  }
  if (data->keyframe) {
    flags |= 0x80;
    if (track->frames) {
      mkv_cluster(m, timecode);
      data->sync = 1;
    }
  }
  track->frames++;
  block_timecode = timecode - m->cluster_tc;
  printk("MKV: Muxing video track %d frame: %zu sz: %zu timecode: %zu block: %zu "
   "cluster: %zu", track->num, track->frames, data->pkt_len, timecode,
   block_timecode, m->cluster_tc);
  if (timecode > m->segment_tc) {
    m->segment_tc = timecode;
    m->segment_duration = m->segment_tc;
  }
  kr_ebml_pack_element(&m->ebml, MKV_SIMPLEBLOCK);
  kr_ebml2_revpack4(&m->ebml, &block_length);
  kr_ebml_pack(&m->ebml, &track_number, 1);
  kr_ebml2_revpack2(&m->ebml, &block_timecode);
  kr_ebml_pack(&m->ebml, &flags, 1);
  return 0;
}

static int64_t audio_track_timecode(kr_mkv_muxer *m, mkv_track *t) {
  int64_t timecode;
  int64_t timecode_scale;
  int64_t sample_rate;
  int64_t nframes;
  timecode_scale = m->tc_scale;
  nframes = t->frames;
  sample_rate = t->info.audio.sample_rate;
  timecode = round(1000000000 * nframes / sample_rate / timecode_scale);
  return timecode;
}

typedef struct {
  uint8_t track;
  uint32_t sz;
  int16_t timecode;
  uint8_t flags;
} mkv_block;

static int pack_block(kr_ebml *e, mkv_block *b) {
  kr_ebml_pack_element(e, MKV_SIMPLEBLOCK);
  kr_ebml2_revpack4(e, &b->sz);
  kr_ebml_pack(e, &b->track, 1);
  kr_ebml2_revpack2(e, &b->timecode);
  kr_ebml_pack(e, &b->flags, 1);
  return 0;
}

static int add_audio_frames(kr_mkv_muxer *m, mkv_track *track, int nframes) {
  track->frames += nframes;
  track->timecode = audio_track_timecode(m, track);
  return 0;
}

static ssize_t mkv_add_audio(kr_mkv_muxer *m, mkv_track *track,
 kr_mkv_muxer_data *data) {
  mkv_block b;
  add_audio_frames(m, track, data->frames);
  if (m->audio_only && (track->timecode - m->cluster_tc >= 1000)) {
    mkv_cluster(m, track->timecode);
    data->sync = 1;
  } else if (track->timecode - m->cluster_tc >= 10000) {
    mkv_cluster(m, track->timecode);
    data->sync = 1;
  }
  b.track = track->num;
  b.track |= 0x80;
  b.sz = data->pkt_len;
  b.sz += 4;
  b.sz |= 0x10000000;
  b.timecode = track->timecode - m->cluster_tc;
  printk("Audio track %d timecode: %zu block: %zu cluster: %zu", track->num,
   track->timecode, b.timecode, m->cluster_tc);
  b.flags = 0;
  b.flags |= 0x80;
  pack_block(&m->ebml, &b);
  if (track->timecode > m->segment_tc) {
    m->segment_tc = track->timecode;
    m->segment_duration = m->segment_tc;
  }
  return 0;
}

ssize_t kr_mkv_mux(kr_mkv_muxer *m, kr_mkv_muxer_data *data) {
  ssize_t ret;
  mkv_track *track;
  if (!m || !data) return -1;
  if (!data->framing || !data->sz || !data->pkt_len) return -2;
  if (data->track < 0 || data->track >= KR_MKV_MUXER_NTRACKS) return -3;
  track = &m->tracks[data->track];
  kr_ebml2_set_buffer(&m->ebml, data->framing, data->sz);
  if (!m->nblocks) {
    mkv_cluster(m, 0);
    data->sync = 1;
  }
  if (track->info.type == KR_TRACK_CODED_AUDIO) {
    ret = mkv_add_audio(m, track, data);
  } else if (track->info.type == KR_TRACK_CODED_VIDEO) {
    ret = mkv_add_video(m, track, data);
  } else ret = -3;
  if (ret < 0) return ret;
  data->len = m->ebml.pos;
  m->nblocks++;
  return data->len;
}

ssize_t kr_mkv_muxer_pack_header(kr_mkv_muxer *m, uint8_t *buf, size_t sz) {
  if (!m || !buf || !sz) return -1;
  if (m->hdr_len > sz) return -2;
  memcpy(buf, m->hdr, m->hdr_len);
  return m->hdr_len;
}

size_t kr_mkv_muxer_sizeof() {
  return sizeof(kr_mkv_muxer);
}

kr_mkv_muxer *kr_mkv_muxer_init(kr_mkv_muxer_setup *setup) {
  int i;
  int ret;
  kr_mkv_muxer *m;
  if (!setup || !setup->mem) return NULL;
  if (setup->tracks < 1 || setup->tracks > KR_MKV_MUXER_NTRACKS) return NULL;
  m = (kr_mkv_muxer *)setup->mem;
  memset(m, 0, sizeof(*m));
  m->ntracks = setup->tracks;
  m->audio_only = 1;
  for (i = 0; i < m->ntracks; i++) {
    m->tracks[i].num = i + 1;
    m->tracks[i].uid = gen_track_uid(m->tracks[i].num + 1);
    m->tracks[i].codec = setup->info[i].codec;
    m->tracks[i].codec_header = setup->header[i];
    m->tracks[i].info = setup->info[i];
    if (m->tracks[i].info.type == KR_TRACK_CODED_VIDEO) m->audio_only = 0;
  }
  ret = pack_header(m, setup->title, setup->type);
  if (ret) return NULL;
  return m;
}
