#include "demux.h"

static int demux_pool_create(kr_studio_path *path, size_t sz) {
  studio_demux *d;
  kr_pool_setup pool_setup;
  if (!path) return -1;
  d = &path->demux;
  memset(&pool_setup, 0, sizeof(pool_setup));
  pool_setup.size = sz;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = 0;
  pool_setup.slices = DEFAULT_ENCODE_NBLOCKS;
  d->coded = kr_pool_create(&pool_setup);
  if (!d->coded) return -2;
  return 0;
}

static int demux_output_track_erase_cb(void *user, void *block_user) {
  int ret;
  kr_studio_path *path;
  if (!user) return -1;
  path = (kr_studio_path *)user;
  ret = kr_pool_release(path->demux.coded, block_user);
  if (ret != 0) {
    printke("Demux pool release ret %d", ret);
    return ret;
  }
  return 0;
}

static int demux_output_track_event(kr_track_event *e) {
  return 0;
}

static int setup_output_tracks(kr_studio_path *path,
 kr_track_info *track_info, kr_codec_header *header, int n) {
  int i;
  studio_demux *d;
  kr_name *name;
  kr_track_setup track_setup;
  kr_track_head_setup head_setup;
  d = &path->demux;
  name = (kr_name *)path->user;
  if (n > KR_DEMUXER_NTRACKS) return -1;
  for (i = 0; i < n; i++) {
    memset(&track_setup, 0, sizeof(track_setup));
    memset(&head_setup, 0, sizeof(head_setup));
    track_setup.user = path;
    track_info[i].track.len = snprintf(track_info[i].track.name,
      sizeof(track_info[i].track.name), "%.*s%d",
        name->len, name->buf, i);
    track_info[i].mode = KR_TRACK_INFINITE;
    track_info[i].duration = DEFAULT_ENCODE_NBLOCKS;
    track_setup.info = &track_info[i];
    if (header) track_setup.header = header[i];
    d->output.track[i] = kr_track_create(path->studio->tracker, &track_setup);
    if (!d->output.track[i]) return -2;
    head_setup.user = path;
    head_setup.type = KR_TRACK_WRITE_HEAD;
    head_setup.track_ev_cb = demux_output_track_event;
    head_setup.erase_cb = demux_output_track_erase_cb;
    d->output.head[i] = kr_track_open(d->output.track[0], &head_setup);
    if (!d->output.head[i]) return -3;
  }
  printk("Demuxer: %d output tracks created", n);
  return 0;
}

static int demux_read_ogg(studio_demux *d, uint8_t *buf, size_t sz) {
  int ret;
  size_t off;
  size_t len;
  kr_track_block block;
  kr_codec_packet pkt;
  kr_ogg_pkt_info opkt_info;
  kr_track_head *wr;
  ret = -1;
  wr = d->output.head[0];
  memset(&block, 0, sizeof(block));
  kr_ogg_unpacker_resync(d->ogg);
  if ( (d->buffer_len + sz) > sizeof(d->buffer)) {
    printk("Demux: not enough space in buffer");
    return -1;
  }
  memcpy(d->buffer + d->buffer_len, buf, sz);
  d->buffer_len += sz;
  ret = kr_ogg_valid(d->buffer, d->buffer_len);
  if (ret < 0) {
    printkd("not valid stuff, returning");
    return 0;
  }
  off = ret;
  len = d->buffer_len;
  for (;;) {
    pkt.sz = kr_pool_slice_size(d->coded);
    pkt.buf = kr_pool_slice(d->coded);
    if (!pkt.buf) {
      printke("Demux: pool slice failed!");
      return -1;
    }
    kr_ogg_unpacker_output_set(d->ogg, &pkt, 1);
    ret = kr_ogg_unpack(d->ogg, &opkt_info, d->buffer + off, len - off);
    if (ret == 1) {
      printkd("end of buffer");
      kr_pool_release(d->coded, pkt.buf);
      break;
    }
    if (ret < 0) {
      printke("Demux: Ogg unpack failed! %d", ret);
      kr_pool_release(d->coded, pkt.buf);
      return -1;
    }
    if (opkt_info.truncated) {
      memmove(d->buffer, (d->buffer + off) + (len - off) - opkt_info.truncated,
         opkt_info.truncated);
      d->buffer_len = opkt_info.truncated;
      kr_pool_release(d->coded, pkt.buf);
      break;
    }
    printkd("Full pkt of size %zu %llu", pkt.len, opkt_info.granule_pos);
    d->buffer_len = 0;
    block.user = pkt.buf;
    block.coded_audio.tc = opkt_info.granule_pos;
    block.coded_audio.data = pkt.buf;
    block.coded_audio.len = pkt.len;
    block.coded_audio.rate = 48000;
    block.type = KR_CODEC_BLOCK_AUDIO;
    ret = kr_track_write(wr, &block);
    if (ret != 0) {
      printke("Demux: track write fail!");
      kr_pool_release(d->coded, pkt.buf);
      return -1;
    }
  }
  return 0;
}

static int demux_read_mkv(studio_demux *d, uint8_t *buf, size_t sz) {
  int ret;
  size_t read;
  kr_track_block tblock;
  kr_mkv_block mblock;
  kr_track_head *wr;
  if (d->buffer_len + sz > sizeof(d->buffer)) return -1;
  memcpy(d->buffer + d->buffer_len, buf, sz);
  d->buffer_len += sz;
  memset(&tblock, 0, sizeof(tblock));
  memset(&mblock, 0, sizeof(mblock));
  ret = kr_mkv_demux(d->mkv, &mblock, d->buffer, d->buffer_len);
  if (ret < 0) {
    printke("mkv demux ret: %d", ret);
    return -2;
  }
  if (ret == 0) return 0;
  if (ret > d->buffer_len) {
    printk("not whole pkt in buffer %d vs %zu", ret, d->buffer_len);
    return 0;
  }
  read = ret;
  tblock.data = kr_pool_slice(d->coded);
  tblock.user = tblock.data;
  if (!tblock.data) {
    printke("Demux: pool slice failed!");
    return -3;
  }
  memcpy(tblock.data, mblock.buf, mblock.sz);
  if (mblock.keyframe) tblock.sync = 1;
  tblock.len = mblock.sz;
  tblock.coded_audio.tc = mblock.tc;
  tblock.coded_audio.data = tblock.data;
  tblock.coded_audio.len = tblock.len;
  tblock.type = KR_CODEC_BLOCK_AUDIO;
  if (mblock.track >= KR_DEMUXER_NTRACKS) return -4;
  wr = d->output.head[0];
  ret = kr_track_write(wr, &tblock);
  if (ret != 0) {
    printke("Demux: track write fail! %d", ret);
    kr_pool_release(d->coded, tblock.data);
    return -5;
  }
  d->buffer_len -= read;
  memmove(d->buffer, d->buffer + read, d->buffer_len);
  return ret;
}

static int demux_read_ts(studio_demux *d, uint8_t *buf, size_t sz) {
  int ret;
  size_t read;
  kr_track_block tblock;
  kr_mts_packet_info pkt;
  kr_track_head *wr;
  if (d->buffer_len + sz > sizeof(d->buffer)) return -1;
  memcpy(d->buffer + d->buffer_len, buf, sz);
  d->buffer_len += sz;
  kr_mts_resync(d->ts);
  read = 0;
  for (;;) {
    memset(&tblock, 0, sizeof(tblock));
    memset(&pkt, 0, sizeof(pkt));
    tblock.data = kr_pool_slice(d->coded);
    tblock.user = tblock.data;
    if (!tblock.data) {
      printke("Demux: pool slice failed!");
      return -2;
    }
    kr_mts_output_set(d->ts, tblock.data, kr_pool_slice_size(d->coded));
    ret = kr_mts_parse(d->ts, &pkt, d->buffer, d->buffer_len);
    if (ret < 0) {
      printke("Demux: TS unpack failed! %d", ret);
      kr_pool_release(d->coded, tblock.data);
      return -1;
    }
    if (ret == 0) {
      printkd("end of buffer");
      kr_pool_release(d->coded, tblock.data);
      break;
    }
    read += ret;
    tblock.len = pkt.sz;
    tblock.coded_audio.tc = pkt.pts;
    tblock.coded_audio.data = tblock.data;
    tblock.coded_audio.len = tblock.len;
    tblock.type = KR_CODEC_BLOCK_AUDIO;
    wr = d->output.head[0];
    ret = kr_track_write(wr, &tblock);
    if (ret != 0) {
      printke("Demux: track write fail!");
      kr_pool_release(d->coded, tblock.data);
      return -1;
    }
  }
  if (d->buffer_len < read) {
    printke("Demux: something went pretty wrong!");
    return -3;
  }
  d->buffer_len -= read;
  memmove(d->buffer, d->buffer + read, d->buffer_len);
  return ret;
}

static int demux_read_flac(studio_demux *d, uint8_t *buf, size_t len) {
  int i;
  int ret;
  int fail;
  kr_track_block block;
  kr_flac_data data;
  kr_flac_info info;
  kr_track_head *wr;
  fail = 0;
  if (d->buffer_len + len > sizeof(d->buffer)) {
    printke("Demux: buffer full!");
    return -1;
  }
  memcpy(d->buffer + d->buffer_len, buf, len);
  d->buffer_len += len;
  memset(&block, 0, sizeof(block));
  memset(&data, 0, sizeof(data));
  data.in.buf = d->buffer;
  data.in.len = d->buffer_len;
  ret = kr_flac_demux(d->flac, &data);
  if (ret != 0) {
    printke("Demux: flac demux failed %d.", ret);
    return -2;
  }
  for (i = 0; i < data.out.len; i++) {
    block.data = kr_pool_slice(d->coded);
    block.user = block.data;
    if (!block.data) {
      printke("Demux: pool slice failed!");
      return -3;
    }
    block.len = data.out.frames[i].len;
    if (block.len > kr_pool_slice_size(d->coded)) {
      printke("Demux: slice size too small!");
      fail = 1;
      break;
    }
    memcpy(block.data, data.out.frames[i].buf, block.len);
    block.coded_audio.tc = d->frames++;
    block.coded_audio.data = block.data;
    block.coded_audio.len = block.len;
    kr_flac_demuxer_get_info(d->flac, &info);
    block.coded_audio.rate = info.sample_rate;
    block.type = KR_CODEC_BLOCK_AUDIO;
    wr = d->output.head[0];
    ret = kr_track_write(wr, &block);
    if (ret != 0) {
      printke("Demux: track write fail!");
      fail = 1;
      break;
    }
  }
  if (fail) {
    kr_pool_release(d->coded, block.data);
    return -4;
  }
  if (data.out.len > 0) {
    d->buffer_len -= data.read;
    memmove(d->buffer, d->buffer + data.read, d->buffer_len);
  }
  return 0;
}

static int demux_read_occ(studio_demux *d, uint8_t *buf, size_t len) {
  int i;
  int ret;
  int fail;
  kr_track_block block;
  kr_occ_data data;
  kr_track_head *wr;
  fail = 0;
  memset(&block, 0, sizeof(block));
  memset(&data, 0, sizeof(data));
  ret = kr_occ_unpack(buf, len, &data);
  if (ret != 0) {
    printke("Demux: occ unpack failed! %d", ret);
    return -1;
  }
  printkd("Demux: unpacked %d OPUS pkts!", data.npkts);
  for (i = 0; i < data.npkts; i++) {
    block.data = kr_pool_slice(d->coded);
    block.user = block.data;
    block.len = data.pkts[i].len;
    if (block.len > kr_pool_slice_size(d->coded)) {
      printke("Demux: slice size too small!");
      fail = 1;
      break;
    }
    memcpy(block.data, data.pkts[i].buf, block.len);
    block.coded_audio.tc = d->frames++ * 480;
    block.coded_audio.data = block.data;
    block.coded_audio.len = block.len;
    block.coded_audio.rate = 48000;
    block.type = KR_CODEC_BLOCK_AUDIO;
    wr = d->output.head[0];
    ret = kr_track_write(wr, &block);
    if (ret != 0) {
      printke("Demux: track write fail!");
      fail = 1;
      break;
    }
  }
  if (fail) {
    kr_pool_release(d->coded, block.data);
    return -4;
  }
  return 0;
}

static int demux_read_raw(studio_demux *d, uint8_t *buf, size_t len) {
  int ret;
  kr_track_block block;
  kr_track_info track_info;
  if ( (d->buffer_len + len) > sizeof(d->buffer)) {
    printk("Demux: not enough space in buffer");
    return -1;
  }
  if ( (d->buffer_len + len) > kr_pool_slice_size(d->coded) ) {
    printk("Demux: slice size too small! %zu vs %zu",
     d->buffer_len + len, kr_pool_slice_size(d->coded));
    return -2;
  }
  memset(&block, 0, sizeof(block));
  block.len = len;
  block.data = kr_pool_slice(d->coded);
  if (!block.data) {
    printke("Demux: pool slice failed!");
    return -3;
  }
  kr_track_info_get(d->output.track[0], &track_info);
  if (d->buffer_len > 0) {
    memcpy(block.data, d->buffer, d->buffer_len);
    memcpy(block.data + d->buffer_len, buf, len);
    block.len += d->buffer_len;
    d->buffer_len = 0;
  } else {
    memcpy(block.data, buf, len);
  }
  block.type = KR_CODEC_BLOCK_AUDIO;
  block.user = block.data;
  block.coded_audio.data = block.data;
  block.coded_audio.len = block.len;
  block.coded_audio.rate = track_info.audio.sample_rate;
  if (track_info.codec.audio == KR_AUDIO_CODEC_MP3) {
    block.coded_audio.tc = (1152 * d->frames);
  } else if (track_info.codec.audio == KR_AUDIO_CODEC_AAC) {
    block.coded_audio.tc = (2048 * d->frames);
  }
  ret = kr_track_write(d->output.head[0], &block);
  if (ret != 0) {
    printke("Demux: track write fail!");
    kr_pool_release(d->coded, block.data);
  }
  return ret;
}

static int demux_probe_mp3(studio_demux *d, uint8_t *buf, size_t len,
 kr_track_info *info) {
  int ret;
  size_t rd;
  size_t pos;
  kr_mpga_info mp_info;
  rd = 0;
  pos = 0;
  for (;;) {
    ret = kr_mpga_scan(buf + pos, len - pos, &rd);
    if (ret < 0) {
      pos += rd;
      if ((len - pos) > 1) continue;
      printk("Studio Demux: No mpga sync found");
      return -1;
    }
    memcpy(d->buffer, buf + pos + ret, len - pos - ret);
    d->buffer_len = len - pos - ret;
    pos += rd;
    ret = kr_mpga_parse(&mp_info, d->buffer, d->buffer_len);
    if (ret == 0) break;
  }
  if (mp_info.frames == 0) {
    printk("Studio Demux: probably need more buffer");
    return -2;
  }
  if (mp_info.layer != 3 || mp_info.version != 1) {
    printk("Studio Demux: mpga but not mp3!");
    return -3;
  }
  info->audio.sample_rate = mp_info.sample_rate;
  printk("Studio Demux: this looks like mp3 %dHz %dch!",
   mp_info.sample_rate, mp_info.channels);
  return 0;
}

static int demux_probe_aac(studio_demux *d, uint8_t *buf,
 size_t len, kr_track_info *info) {
  int ret;
  size_t rd;
  size_t pos;
  kr_adts_info ad_info;
  rd = 0;
  pos = 0;
  for (;;) {
    ret = kr_mpga_scan(buf + pos, len - pos, &rd);
    if (ret < 0) {
      pos += rd;
      if ((len - pos) > 1) continue;
      printk("Studio Demux: No adts sync found");
      return -1;
    }
    memcpy(d->buffer, buf + pos + ret, len - pos - ret);
    d->buffer_len = len - pos - ret;
    pos += rd;
    ret = kr_adts_parse(&ad_info, d->buffer, d->buffer_len);
    if (ret == 0) break;
  }
  if (ad_info.frames == 0) {
    printk("Studio Demux: probably need more buffer");
    return -2;
  }
  info->audio.sample_rate = ad_info.sample_rate;
  printk("Studio Demux: this looks like aac %dHz!", ad_info.sample_rate);
  return 0;
}

static int demux_probe_raw(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  int fail;
  kr_track_info track_info;
  fail = 1;
  path->info.demux.type = KR_CONTAINER_RAW;
  memset(&track_info, 0, sizeof(track_info));
  track_info.type = KR_TRACK_CODED_AUDIO;
  track_info.codec.type = KR_CODEC_AUDIO;
  ret = demux_probe_mp3(&path->demux, buf, len, &track_info);
  if (ret == 0) {
    track_info.codec.audio = KR_AUDIO_CODEC_MP3;
    track_info.audio.samples = 1152;
    fail = 0;
  }
  if (fail) {
    ret = demux_probe_aac(&path->demux, buf, len, &track_info);
    if (ret == 0) {
      track_info.codec.audio = KR_AUDIO_CODEC_AAC;
      track_info.audio.samples = 2048;
      fail = 0;
    }
    if (ret != 0) fail = 1;
  }
  if (fail) {
    printk("Studio Play: couldn't guess raw codec");
    return -1;
  }
  ret = setup_output_tracks(path, &track_info, NULL, 1);
  return ret;
}

static int demux_probe_ogg(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  int i;
  studio_demux *d;
  kr_track_info track_info;
  kr_codec_header header;
  kr_codec_packet pkt;
  kr_ogg_pkt_info opkt_info;
  kr_ogg_unpacker_setup setup;
  d = &path->demux;
  ret = kr_ogg_valid(buf, len);
  if (ret != 0) return ret;
  memset(&setup, 0, sizeof(setup));
  memset(&header, 0, sizeof(header));
  /* here I should know enough about the stream to create output tracks */
  if (sizeof(d->mem) < kr_ogg_unpacker_sizeof()) {
    printke("OGG demuxer is bigger than we thunk!");
    exit(1);
  }
  setup.mem = d->mem;
  d->ogg = kr_ogg_unpacker_init(&setup);
  if (!d->ogg) {
    printke("Ogg demuxer init failed!");
    return -1;
  }
  header.len = 2;
  header.data[0].buf = d->header;
  header.data[1].buf = d->header + (sizeof(d->header) / 2) ;
  header.data[0].sz = sizeof(d->header) / 2;
  header.data[1].sz = sizeof(d->header) / 2;
  for (i = 0; i < header.len; i++) {
    pkt.buf = header.data[i].buf;
    pkt.sz = header.data[i].sz;
    kr_ogg_unpacker_output_set(d->ogg, &pkt, 1);
    ret = kr_ogg_unpack(d->ogg, &opkt_info, buf, len);
    if (ret != 0) return -2;
    header.data[i].sz = pkt.len;
  }
  path->info.demux.type = KR_CONTAINER_OGG;
  /* one audio track only for now */
  memset(&track_info, 0, sizeof(track_info));
  track_info.type = KR_TRACK_CODED_AUDIO;
  track_info.codec = opkt_info.codec;
  ret = setup_output_tracks(path, &track_info, &header, 1);
  return ret;
}

static int demux_probe_mkv(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  kr_mkv_demuxer_setup setup;
  kr_mkv_header header;
  studio_demux *d;
  d = &path->demux;
  if (sizeof(d->mem) < kr_mkv_demuxer_sizeof()) {
    printke("MKV demuxer is bigger than we thunk!");
    exit(1);
  }
  memset(&header, 0, sizeof(header));
  setup.mem = d->mem;
  d->mkv = kr_mkv_demuxer_init(&setup);
  if (!d->mkv) {
    printke("MKV demuxer init failed!");
    return -1;
  }
  ret = kr_mkv_start(d->mkv, &header, buf, len);
  if (ret < 1) return -2;
  if (header.ntracks != 1) {
    printke("Demux: MKV wrong number of tracks %d, only 1 supported atm",
      header.ntracks);
    return -3;
  }
  d->buffer_len = len - ret;
  if (d->buffer_len > sizeof(d->buffer)) exit(1);
  memcpy(d->buffer, buf + ret, d->buffer_len);
  header.track[0].type = KR_TRACK_CODED_AUDIO;
  ret = setup_output_tracks(path, header.track,
   header.header, header.ntracks);
  path->info.demux.type = KR_CONTAINER_MKV;
  return ret;
}

static int demux_probe_ts(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  kr_mts_setup setup;
  kr_mts_info info;
  kr_track_info track_info;
  kr_mts_packet_info pkt_info;
  studio_demux *d;
  d = &path->demux;
  if (sizeof(d->mem) < kr_mts_sizeof()) {
    printke("MTS demuxer is bigger than we thunk!");
    exit(1);
  }
  setup.mem = d->mem;
  memset(&info, 0, sizeof(info));
  d->ts = kr_mts_init(&setup);
  if (!d->ts) {
    printke("MTS demuxer init failed!");
    return -1;
  }
  ret = kr_mts_parse(d->ts, &pkt_info, buf, len);
  if (ret < 1) return -2;
  kr_mts_info_get(d->ts, &info);
  if (info.nprograms != 1) return -3;
  if (info.programs[0].nstreams != 1) {
    printke("Demux: TS wrong number of tracks %d, only 1 supported atm",
      info.programs[0].nstreams);
    return -4;
  }
  d->buffer_len = len;
  memcpy(d->buffer, buf, len);
  memset(&track_info, 0, sizeof(track_info));
  track_info.type = KR_TRACK_CODED_AUDIO;
  track_info.codec.type = KR_CODEC_AUDIO;
  track_info.codec.audio = KR_AUDIO_CODEC_AAC;
  ret = setup_output_tracks(path, &track_info,
   NULL, 1);
  path->info.demux.type = KR_CONTAINER_TS;
  return ret;
}

static int demux_probe_flac(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  kr_flac_demuxer_setup setup;
  kr_track_info track_info;
  kr_codec_header header;
  studio_demux *d;
  d = &path->demux;
  if (sizeof(d->mem) < kr_flac_demuxer_sizeof()) {
    printke("FLAC demuxer is bigger than we thunk!");
    exit(1);
  }
  setup.mem = d->mem;
  setup.buf = buf;
  setup.len = len;
  d->flac = kr_flac_demuxer_init(&setup);
  if (!d->flac) return -1;
  ret = kr_flac_unpack_header(d->flac, d->header, sizeof(d->header));
  if (ret <= 0) {
    printke("Demux FLAC: Not enough space in buffer, hdr too big?!");
    return -2;
  }
  memset(&header, 0, sizeof(header));
  header.len = 1;
  header.data[0].buf = d->header;
  header.data[0].sz = ret;
  memset(&track_info, 0, sizeof(track_info));
  track_info.type = KR_TRACK_CODED_AUDIO;
  track_info.codec.type = KR_CODEC_AUDIO;
  track_info.codec.audio = KR_AUDIO_CODEC_FLAC;
  ret = setup_output_tracks(path, &track_info,
   &header, 1);
  path->info.demux.type = KR_CONTAINER_FLAC;
  d->buffer_len = len - header.data[0].sz;
  memcpy(d->buffer, buf + header.data[0].sz, d->buffer_len);
  return ret;
}

static int demux_probe_occ(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  kr_track_info track_info;
  if (kr_occ_sync_check(buf, len) != 0) return -1;
  memset(&track_info, 0, sizeof(track_info));
  track_info.type = KR_TRACK_CODED_AUDIO;
  track_info.codec.type = KR_CODEC_AUDIO;
  track_info.codec.audio = KR_AUDIO_CODEC_OPUS;
  track_info.audio.samples = 480;
  track_info.audio.sample_rate = 48000;
  track_info.audio.channels = 2;
  ret = setup_output_tracks(path, &track_info, NULL, 1);
  path->info.demux.type = KR_CONTAINER_OCC;
  return ret;
}

static int demux_probe(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  studio_demux *d;
  d = &path->demux;
  printk("Demux: probing OGG");
  ret = demux_probe_ogg(path, buf, len);
  if (ret) printk("Demux: no it's not ogg");
  else {
    printk("Demux: OGG detected!");
    return 0;
  }
  d->buffer_len = 0;
  printk("Demux: probing MPEG-TS");
  ret = demux_probe_ts(path, buf, len);
  if (ret) printk("Demux: no it's not ts");
  else {
    printk("Demux: MPEG-TS detected!");
    return 0;
  }
  d->buffer_len = 0;
  printk("Demux: probing MKV");
  ret = demux_probe_mkv(path, buf, len);
  if (ret) printk("Demux: no it's not MKV");
  else {
    printk("Demux: MKV detected!");
    return 0;
  }
  d->buffer_len = 0;
  printk("Demux: probing FLAC");
  ret = demux_probe_flac(path, buf, len);
  if (ret) printk("Demux: no it's not MKV");
  else {
    printk("Demux: FLAC detected!");
    return 0;
  }
  d->buffer_len = 0;
  printk("Demux: probing RAW");
  ret = demux_probe_raw(path, buf, len);
  if (ret) printk("Demux: no it's not raw");
  else {
    printk("Demux: RAW detected!");
    return 0;
  }
  d->buffer_len = 0;
  return -1;
}

static int ext_to_container_type(char *ext) {
  if (!ext) return 0;
  if (!strcmp(ext, "mp3")) return KR_CONTAINER_RAW;
  if (!strcmp(ext, "aac")) return KR_CONTAINER_RAW;
  if (!strcmp(ext, "occ")) return KR_CONTAINER_OCC;
  if (!strcmp(ext, "ogg")) return KR_CONTAINER_OGG;
  if (!strcmp(ext, "opus")) return KR_CONTAINER_OGG;
  if (!strcmp(ext, "mkv")) return KR_CONTAINER_MKV;
  if (!strcmp(ext, "mka")) return KR_CONTAINER_MKV;
  if (!strcmp(ext, "webm")) return KR_CONTAINER_WEBM;
  if (!strcmp(ext, "ts")) return KR_CONTAINER_TS;
  if (!strcmp(ext, "flac")) return KR_CONTAINER_FLAC;
  return 0;
}

int studio_demux_probe(kr_studio_path *path, studio_demux_probe_input  *in) {
  int ret;
  if (!path || !in) return -1;
  if (!in->buf || !in->len) return -2;
  printk("Demux Probe");
  ret = -1;
  path->info.demux.type = ext_to_container_type(in->ext);
  switch (path->info.demux.type) {
    case KR_CONTAINER_RAW:
      printk("Demux: Detected RAW container from ext");
      ret = demux_probe_raw(path, in->buf, in->len);
      if (ret != 0) printke("Demux: raw probing failed %d", ret);
      break;
    case KR_CONTAINER_OGG:
      printk("Demux: Detected OGG container from ext");
      ret = demux_probe_ogg(path, in->buf, in->len);
      if (ret != 0) printke("Demux: ogg probing failed %d", ret);
      break;
    case KR_CONTAINER_WEBM:
    case KR_CONTAINER_MKV:
      printk("Demux: Detected MKV container from ext");
      ret = demux_probe_mkv(path, in->buf, in->len);
      if (ret != 0) printke("Demux: mkv probing failed %d", ret);
      break;
    case KR_CONTAINER_TS:
      printk("Demux: Detected MTS container from ext");
      ret = demux_probe_ts(path, in->buf, in->len);
      if (ret != 0) printke("Demux: mts probing failed %d", ret);
      break;
    case KR_CONTAINER_FLAC:
      printk("Demux: Detected FLAC container from ext");
      ret = demux_probe_flac(path, in->buf, in->len);
      if (ret != 0) printke("Demux: flac probing failed %d", ret);
      break;
    case KR_CONTAINER_OCC:
      printk("Demux: Detected OCC container from ext");
      ret = demux_probe_occ(path, in->buf, in->len);
      if (ret != 0) printke("Demux: occ probing failed %d", ret);
      break;
    default:
      printk("Demux: No format detected from ext, trying to guess it by probing");
      ret = demux_probe(path, in->buf, in->len);
      if (ret != 0) printke("Demux: probing failed %d", ret);
      break;
  }
  if (ret == 0) {
    if (path->info.demux.type == KR_CONTAINER_FLAC) {
      ret = demux_pool_create(path, 8192 * 4);
    } else {
      ret = demux_pool_create(path, 2048);
    }
  }
  return ret;
}

int studio_demux_read(kr_studio_path *path, uint8_t *buf, size_t len) {
  int ret;
  studio_demux *d;
  if (!path || !buf || !len) return -1;
  printkd("Demux Read %zu", len);
  ret = -1;
  d = &path->demux;
  switch(path->info.demux.type) {
    case KR_CONTAINER_RAW:
      ret = demux_read_raw(d, buf, len);
      if (ret != 0) printke("Demux: demux read raw failed!");
      break;
    case KR_CONTAINER_OGG:
      ret = demux_read_ogg(d, buf, len);
      if (ret != 0) printke("Demux: demux read ogg failed!");
      break;
    case KR_CONTAINER_MKV:
      ret = demux_read_mkv(d, buf, len);
      if (ret != 0) printke("Demux: demux read mkv failed!");
      break;
    case KR_CONTAINER_TS:
      ret = demux_read_ts(d, buf, len);
      if (ret != 0) printke("Demux: demux read mts failed!");
      break;
    case KR_CONTAINER_FLAC:
      ret = demux_read_flac(d, buf, len);
      if (ret != 0) printke("Demux: demux read flac failed!");
      break;
    case KR_CONTAINER_OCC:
      ret = demux_read_occ(d, buf, len);
      if (ret != 0) printke("Demux: demux read occ failed!");
      break;
    default:
      printke("Demux: demux type not set, did probing happen?");
      break;
  }
  if (ret == 0) ++d->frames;
  return ret;
}

int studio_demux_destroy(kr_studio_path *path) {
  int i;
  int ret;
  studio_demux *d;
  if (!path) {
    printke("Demux: destroy called on invalid path");
    return -1;
  }
  d = &path->demux;
  kr_spin_lock(&d->lock);
  for (i = 0; i < KR_DEMUXER_NTRACKS; i++) {
    if (!d->output.head[i]) continue;
    ret = kr_track_close(d->output.head[i]);
    if (ret) printke("Demux: output track %d close ret %d", i, ret);
    d->output.head[i] = NULL;
    ret = kr_track_remove(d->output.track[i]);
    if (ret) printke("Demux: output track %d destroy ret %d", i, ret);
    d->output.track[i] = NULL;
  }
  kr_pool_destroy(d->coded);
  kr_spin_unlock(&d->lock);
  printk("Demux: Destroyed");
  return 0;
}

int studio_demux_create(kr_studio_path *path) {
  int ret;
  studio_demux *d;
  if (!path) return -1;
  ret = 0;
  d = &path->demux;
  memset(d, 0, sizeof(*d));
  kr_spin_init(&d->lock);
  return ret;
}
