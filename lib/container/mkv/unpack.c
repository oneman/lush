#include <krad/app/debug.h>
#include <krad/ebml/ebml.h>
#include <krad/ebml/ids.h>
#include "ids.h"
#include "unpack.h"

struct kr_mkv_demuxer {
  uint8_t mem[32];
  kr_ebml *e;
  uint64_t track_tc[KR_MKV_DEMUXER_NTRACKS];
  uint64_t cluster_tc;
  uint64_t tc_scale;
  double duration;
  kr_mkv_header header;
};

static kr_codec mkv_codec_to_kr_codec(char *str, uint64_t sz) {
  char type;
  char *id;
  kr_codec codec;
  memset(&codec, 0, sizeof(codec));
  if (sz < 3) return codec;
  type = str[0];
  id = str + 2;
  sz -= 2;
  if (type == 'A') {
    codec.type = KR_CODEC_AUDIO;
  } else if (type == 'V') {
    codec.type = KR_CODEC_VIDEO;
  } else {
    printke("Unknown codec: %.*s", sz, id);
    return codec;
  }
  for (;;) {
    if (sz == 5 && (strncmp(id, "VORBIS", 5) == 0)) {
      codec.audio = KR_AUDIO_CODEC_VORBIS;
      break;
    }
    if (sz == 4 && (strncmp(id, "FLAC", 4) == 0)) {
      codec.audio = KR_AUDIO_CODEC_FLAC;
      break;
    }
    if (sz == 4 && (strncmp(id, "OPUS", 4) == 0)) {
      codec.audio = KR_AUDIO_CODEC_OPUS;
      break;
    }
    if (sz == 3 && (strncmp(id, "VP8", 3) == 0)) {
      codec.video = KR_VIDEO_CODEC_VPX;
      break;
    }
    if (sz == 3 && (strncmp(id, "VP9", 3) == 0)) {
      codec.video = KR_VIDEO_CODEC_VPX;
      break;
    }
    if (sz == 6 && (strncmp(id, "THEORA", 6) == 0)) {
      codec.video = KR_VIDEO_CODEC_THEORA;
      break;
    }
    if (sz == 13 && (strncmp(id, "MPEG4/ISO/AVC", 13) == 0)) {
      codec.video = KR_VIDEO_CODEC_H264;
      break;
    }
    break;
  }
  return codec;
}

static ssize_t check_ebml_header(kr_mkv_demuxer *d) {
  char doctype[64];
  uint32_t version;
  uint32_t read_version;
  if (0 > kr_ebml2_unpack_header(d->e, doctype, 64, &version, &read_version)) {
    printke("Could not read EBML header");
    return -1;
  }
  if ((strncmp(doctype, "webm", 4) != 0)
   && (strncmp(doctype, "matroska", 8) != 0)) {
    printke("Invalid Doctype: %s", doctype);
    return -1;
  }
  if (version < 2 || version > 4 || read_version < 2 || read_version > 4) {
    printke("Crazy Version Number: %u - %u", version, read_version);
    return -1;
  }
  printk("Got EBML doctype: %s Version %u Read Version %u", doctype, version,
   read_version);
  return 0;
}

static int parse_segment_info(kr_mkv_demuxer *mkv, uint64_t max_pos) {
  int ret;
  uint32_t id;
  uint64_t size;
  for (;;) {
  if (mkv->e->pos == max_pos) {
      /*printk("Got to end of segment info");*/
      break;
    }
    ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
    if (ret < 0) {
      printke("Read error..");
      return -1;
    }
    switch (id) {
      case MKV_SEGMENT_TITLE:
        /*printk("Skipping TITLE: %"PRIu64" bytes", size);*/
        kr_ebml2_advance(mkv->e, size);
        break;
      case MKV_SEGMENT_TIMECODESCALE:
        kr_ebml2_unpack_uint64(mkv->e, &mkv->tc_scale, size);
        break;
      case MKV_SEGMENT_DURATION:
        kr_ebml2_unpack_double(mkv->e, &mkv->duration, size);
        break;
      case MKV_SEGMENT_UID:
      case MKV_SEGMENT_FILENAME:
      case MKV_SEGMENT_PREVUID:
      case MKV_SEGMENT_PREVFILENAME:
      case MKV_SEGMENT_NEXTUID:
      case MKV_SEGMENT_NEXTFILENAME:
      case MKV_SEGMENT_FAMILY:
      case MKV_SEGMENT_CHAPTERTRANSLATE:
      case MKV_SEGMENT_DATEUTC:
      case MKV_SEGMENT_MUXINGAPP:
      case MKV_SEGMENT_WRITINGAPP:
        kr_ebml2_advance(mkv->e, size);
        break;
      default:
        printk("Skipping unknown element in segment info: %zu bytes", size);
        kr_ebml2_advance(mkv->e, size);
        break;
    }
  }
  return 0;
}

static int parse_track(kr_mkv_demuxer *mkv, uint64_t max_pos) {
  int ret;
  uint32_t id;
  uint64_t size;
  kr_track_info track;
  uint8_t number;
  uint8_t type;
  char codec_id[32];
  float samplerate;
  uint32_t num;
  samplerate = 0;
  number = 0;
  type = 0;
  memset(&track, 0, sizeof(kr_track_info));
  memset(codec_id, 0, sizeof(codec_id));
  for (;;) {
    if (mkv->e->pos == max_pos) {
      /*printk("Got to end of track info");*/
      break;
    }
    ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
    if (ret < 0) {
      printke("Read error..");
      return -1;
    }
    switch (id) {
      case MKV_AUDIO:
      case MKV_VIDEO:
        break;
      case MKV_TRACKNUMBER:
        if (number != 0) {
          printke("Got a second track number for the same track.");
          return -1;
        }
        kr_ebml2_unpack_uint8(mkv->e, &number, size);
        break;
      case MKV_TRACKTYPE:
        if (type != 0) {
          printke("Got a second track type for the same track.");
          return -1;
        }
        kr_ebml2_unpack_uint8(mkv->e, &type, size);
        break;
      case MKV_CODECID:
        kr_ebml2_unpack_string(mkv->e, codec_id, size);
        track.codec = mkv_codec_to_kr_codec(codec_id, size);
        if (track.codec.type == 0) {
          printke("Unsupported Codec: %s", codec_id);
          return -1;
        }
        if (track.codec.type == KR_CODEC_VIDEO) {
          printk("Video Codec: %s", codec_id + 2);
        } else if (track.codec.type == KR_CODEC_AUDIO) {
          printk("Audio Codec: %s", codec_id + 2);
        }
        break;
      case MKV_CODECDATA:
        /*ret = track_read_codec_hdr(mkv, &track, size);*/
        kr_ebml2_advance(mkv->e, size);
        ret = 0;
        if (ret < 0) {
          printke("Error reading codec data header");
          return -1;
        }
        break;
      case MKV_WIDTH:
        kr_ebml2_unpack_uint32(mkv->e, &num, size);
        track.video.width = num;
        break;
      case MKV_HEIGHT:
        kr_ebml2_unpack_uint32(mkv->e, &num, size);
        track.video.height = num;
        break;
      case MKV_CHANNELS:
        kr_ebml2_unpack_uint32(mkv->e, &num, size);
        track.audio.channels = num;
        break;
      case MKV_SAMPLERATE:
        kr_ebml2_unpack_float(mkv->e, &samplerate, size);
        track.audio.sample_rate = samplerate;
        break;
      case MKV_BITDEPTH:
        kr_ebml2_unpack_uint32(mkv->e, &num, size);
        track.audio.bit_depth = num;
        break;
      default:
        printk("Skipping unknown element: %"PRIu64" bytes", size);
        kr_ebml2_advance(mkv->e, size);
        break;
    }
  }
  mkv->header.track[mkv->header.ntracks] = track;
  mkv->header.ntracks++;
  return 0;
}

static int parse_tracks(kr_mkv_demuxer *mkv, uint64_t max_pos) {
  int ret;
  uint32_t id;
  uint64_t size;
  for (;;) {
    /*printk("parsing tracks: pos: %zu max: %zu", mkv->e->pos, max_pos);*/
    if (mkv->e->pos == max_pos) {
      /*printk("Got to end of tracks info");*/
      break;
    }
    ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
    if (ret < 0) {
      printke("Read error...");
      return -1;
    }
    if (id == EID_VOID || id == EID_CRC32 || id == MKV_TAGS) {
      kr_ebml2_advance(mkv->e, size);
      continue;
    }
    if (id == MKV_CLUSTER || id == MKV_CUES || id == MKV_ATTACHMENTS
     || id == MKV_CHAPTERS) {
      printke("Parse tracks: got unexpected mkv element at %.*zu", mkv->e->pos);
      return -1;
    }
    if (id != MKV_TRACK) {
      printke("No Track :/");
      return -1;
    }
    if (mkv->header.ntracks == KR_MKV_DEMUXER_NTRACKS) {
      printke("Tooo many tracks!");
      return -1;
    }
    /*printk("Got Track!");*/
    ret = parse_track(mkv, mkv->e->pos + size);
    if (ret < 0) {
      printke("Track parse error...");
      return -1;
    }
  }
  return 0;
}

static int parse_headers(kr_mkv_demuxer *mkv) {
  int ret;
  int seen_tracks_info;
  int seen_segment_info;
  uint32_t id;
  uint64_t size;
  seen_tracks_info = 0;
  seen_segment_info = 0;
  ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
  if (ret < 0) {
    printke("Segment header Read error...");
    return -1;
  }
  if (id != MKV_SEGMENT) {
    printke("No Segment :/");
    return -1;
  }
  /*printk("Got Segment");*/
  while (!seen_segment_info || !seen_tracks_info) {
    ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
    if (ret < 0) {
      printke("Header Read error..");
      return -1;
    }
    switch (id) {
      case MKV_TRACKS:
        //mkv->tracks_info_data = mkv->e->bufstart + mkv->e->pos;
        //mkv->tracks_info_data_size = size;
        if (parse_tracks(mkv, mkv->e->pos + size) < 0) {
          return -1;
        }
        seen_tracks_info = 1;
        break;
      case MKV_SEGMENT_INFO:
        //mkv->segment_info_data = mkv->e->bufstart + mkv->e->pos;
        //mkv->segment_info_data_size = size;
        /*printk("Got Segment Info: %"PRIu64" bytes", size);*/
        if (parse_segment_info(mkv, mkv->e->pos + size) < 0) {
          return -1;
        }
        seen_segment_info = 1;
        break;
      default:
        printk("Skipping unknown element: %"PRIu64" bytes", size);
        kr_ebml2_advance(mkv->e, size);
        break;
    }
  }
  return 0;
}

/*static int track_read_codec_hdr(kr_mkv_demuxer *mkv, kr_track_info *track,
 uint64_t size) {
  int ret;
  if (size > KRAD_MKV_CODEC_HDR_MAX_SANE_SZ) {
    printke("Got codec data size %"PRIu64". To big!", size);
    return -1;
  }
  if (size == 0) {
    printke("Got codec data size of zero!");
    return -1;
  }
  //FIXME FREE
  printk("Got codec data size %"PRIu64"", size);
  track->codec_data_size = size;
  track->codec_data = kr_alloc(track->codec_data_size);
  ret = kr_ebml2_unpack_data (mkv->e, track->codec_data, track->codec_data_size);
  if (ret != 0) {
    printke("Got error unpacking codec data!");
    return -1;
  }
  if ((track->codec == OPUS) || (track->codec == FLAC)) {
    track->headers = 1;
    track->header_len[0] = track->codec_data_size;
    track->header[0] = track->codec_data;
  }
  if ((track->codec == VORBIS) || (track->codec == THEORA)) {
    uint8_t byte;
    uint32_t bytes_read;
    uint32_t value;
    uint32_t maxlen;
    uint32_t current_header;
    current_header = 0;
    bytes_read = 0;
    value = 0;
    maxlen = MIN (10, track->codec_data_size);
    byte = track->codec_data[bytes_read];
    bytes_read += 1;
    if (byte != 2) {
      printke("Unknown number of Xiph laced headers");
      return -1;
    }
    while ((current_header < 3) && (bytes_read < maxlen)) {
      value = 0;
      while ((current_header < 2) && (bytes_read < maxlen)) {
        byte = track->codec_data[bytes_read];
        bytes_read += 1;
        value += byte;
        if (byte != 255) {
          track->header_len[current_header] = value;
          printk("Xiph lace value %u is %u", current_header,
           track->header_len[current_header]);
          current_header++;
          break;
        } else {
          value += 255;
        }
      }
      if (current_header == 2) {
        track->header_len[current_header] = track->codec_data_size -
                                            (bytes_read +
                                            track->header_len[0] +
                                            track->header_len[1]);
        printk("Xiph lace value %u is %u", current_header,
         track->header_len[current_header]);
        break;
      }
    }
    if ((track->header_len[0] == 0) ||
        (track->header_len[1] == 0) ||
        (track->header_len[2] == 0)) {
      printke("Error reading Xiph lace sizes, a zero");
      return -1;
    }
    if ((track->header_len[0] + track->header_len[1] +
         track->header_len[2] + bytes_read) != track->codec_data_size) {
      printke("Error reading Xiph lace sizes, miss-sized");
      return -1;
    }
    track->header[0] = track->codec_data + bytes_read;
    track->header[1] = track->codec_data + (bytes_read + track->header_len[0]);
    track->header[2] = track->codec_data + (bytes_read +
                                            track->header_len[0] +
                                            track->header_len[1]);
    track->headers = 3;
  }
  return 0;
}*/

static int parse_simpleblock(kr_mkv_demuxer *mkv, uint32_t len,
 uint32_t *track, uint64_t *timecode, uint8_t *flags) {
  int16_t block_timecode;
  uint8_t flags_tmp;
  uint8_t byte;
  block_timecode = 0;
  /*printk("parsing simpelblcok");*/
  kr_ebml2_unpack_data(mkv->e, &byte, 1);
  if (track != NULL) {
    *track = (byte - 0x80);
  }
  kr_ebml2_unpack_int16(mkv->e, &block_timecode, 2);
  mkv->track_tc[*track] = mkv->cluster_tc + block_timecode;
  /*printk("Timecode is: %6.3f", (mkv->cluster_tc + (int64_t)block_timecode) / 1000.0);*/
  /*printk("TC: %zu", mkv->track_tc[*track]);*/
  if (timecode != NULL) {
    *timecode = mkv->track_tc[*track];
  }
  kr_ebml2_unpack_data(mkv->e, &flags_tmp, 1);
  if (flags != NULL) {
    *flags = flags_tmp;
  }
  return len - 4;
}

int read_packet(kr_mkv_demuxer *mkv, uint32_t *track, uint64_t *timecode,
 uint8_t *flags, uint8_t **buffer, size_t *sz) {
  int ret;
  uint32_t id;
  uint64_t size;
  /*printk("mkv read_packet");*/
  for (;;) {
    id = 0;
    size = 0;
    /*printk("loop");*/
    ret = kr_ebml2_unpack_id(mkv->e, &id, &size);
    if (ret < 0) {
      printke("Read packet error.. %d", ret);
      return -1;
    }
    switch (id) {
      /*printk("id ...");*/
      case MKV_CLUSTER:
        break;
      case EID_VOID:
        /*printk("skipping void of %zu", size);*/
        kr_ebml2_advance(mkv->e, size);
      case EID_CRC32:
      case MKV_TAGS:
      case MKV_CUES:
      case MKV_ATTACHMENTS:
      case MKV_CHAPTERS:
        kr_ebml2_advance(mkv->e, size);
        break;
      case MKV_CLUSTER_TIMECODE:
        kr_ebml2_unpack_uint64(mkv->e, &mkv->cluster_tc, size);
        break;
      case MKV_SIMPLEBLOCK:
        size = parse_simpleblock(mkv, size, track, timecode, flags);
        *buffer = mkv->e->buf;
        *sz = size;
        /*printk("done with that simpleblock, its payload is %zu", *sz);*/
        //kr_ebml2_unpack_data(mkv->e, buffer, size - 4);
        kr_ebml2_advance(mkv->e, *sz);
        return *sz;
      default:
        printke("Err! Unknown element: %"PRIu64" bytes", size);
        return -1;
        break;
    }
  }
  return 0;
}

size_t kr_mkv_demuxer_sizeof() {
  return sizeof(kr_mkv_demuxer);
}

kr_mkv_demuxer *kr_mkv_demuxer_init(kr_mkv_demuxer_setup *setup) {
  kr_mkv_demuxer *d;
  if (!setup) return NULL;
  d = (kr_mkv_demuxer *)setup->mem;
  memset(d, 0, sizeof(*d));
  if (sizeof(d->mem) < kr_ebml_sizeof()) return NULL;
  d->e = kr_ebml_init(d->mem);
  return d;
}

ssize_t kr_mkv_start(kr_mkv_demuxer *d, kr_mkv_header *h, uint8_t *buf,
 size_t sz) {
  int ret;
  size_t pos;
  if (!d || !h || !buf || sz < 1) return -1;
  pos = 0;
  kr_ebml2_set_buffer(d->e, buf, sz);
  ret = check_ebml_header(d);
  if (ret) return -2;
  pos = kr_ebml_position(d->e);
  ret = parse_headers(d);
  if (ret) return -3;
  pos = kr_ebml_position(d->e);
  *h = d->header;
  return pos;
}

#define KR_MKV_KEYFRAME 0x80

ssize_t kr_mkv_demux(kr_mkv_demuxer *d, kr_mkv_block *b, uint8_t *buf,
 size_t sz) {
  int ret;
  size_t pos;
  if (!d || !b || !buf || sz < 1) return -1;
  /*printk("demux a block");*/
  kr_ebml2_set_buffer(d->e, buf, sz);
  ret = read_packet(d, &b->track, &b->tc, &b->flags, &b->buf, &b->sz);
  if (b->flags & KR_MKV_KEYFRAME) {
    b->keyframe = 1;
  } else {
    b->keyframe = 0;
  }
  if (ret < 0) return -3;
  pos = kr_ebml_position(d->e);
  return pos;
}
