#include "mux.h"
#include "mux_output.c"

static int mux_block(kr_studio_path *path, int tnum, kr_codec_type type,
 kr_track_block *b);
static int output_mux(kr_studio_path *path);
static int mux_process(kr_studio_path *path);
static int mux_input_track_event(kr_track_event *e);
static int mux_ok(kr_studio_path *path);
static int muxed_track_event(kr_track_event *e);
static int setup_output_track(kr_studio_path *path, int nblocks);

static int mux_block(kr_studio_path *path, int tnum, kr_codec_type type,
 kr_track_block *b) {
  int i;
  ssize_t ret;
  studio_mux *m;
  kr_track_head *wr;
  kr_mux_info *mux_info;
  kr_ogg_muxer_data od;
  kr_mkv_muxer_data md;
  kr_txt_muxer_data td;
  kr_mts_muxer_data tsd;
  kr_occ_data ocd;
  kr_track_block cb;
  ret = 0;
  m = &path->mux;
  mux_info = &path->info.mux;
  wr = m->output.head;
  memset(&od, 0, sizeof(od));
  memset(&md, 0, sizeof(md));
  memset(&td, 0, sizeof(td));
  memset(&cb, 0, sizeof(cb));
  memset(&ocd, 0, sizeof(ocd));
  cb.type = KR_CONTAINER_BLOCK;
  cb.data = cb.buf;
  switch (mux_info->type) {
  case KR_CONTAINER_RAW:
    if (type == KR_CODEC_VIDEO) {
      b->type = KR_CODEC_BLOCK_VIDEO;
      b->data = b->frame.coded.data;
      b->len = b->frame.coded.info.sz;
    } else if (type == KR_CODEC_AUDIO) {
      b->type = KR_CODEC_BLOCK_AUDIO;
      b->data = b->coded_audio.data;
      b->len = b->coded_audio.len;
      b->sync = 1;
    }
    ret = kr_track_write(wr, b);
    break;
  case KR_CONTAINER_FLAC:
    b->type = KR_CODEC_BLOCK_AUDIO;
    b->data = b->coded_audio.data;
    b->len = b->coded_audio.len;
    b->sync = 1;
    ret = kr_track_write(wr, b);
    break;
  case KR_CONTAINER_Y4M:
    b->type = KR_CODEC_BLOCK_VIDEO;
    cb.len = kr_y4m_pack_frame_header(m->y4m, cb.buf, sizeof(cb.buf));
    cb.sync = 1;
    ret = kr_track_write(wr, &cb);
    b->data = b->frame.coded.data;
    b->len = b->frame.coded.info.sz;
    b->sync = 0;
    ret = kr_track_write(wr, b);
    break;
  case KR_CONTAINER_TXT:
    td.track = tnum;
    if (type == KR_CODEC_VIDEO) {
      b->type = KR_CODEC_BLOCK_VIDEO;
      td.in.tc = b->frame.info.tc;
      td.in.sz = b->frame.coded.info.sz;
      td.in.keyframe = b->frame.info.keyframe;
      b->data = b->frame.coded.data;
      b->len = b->frame.coded.info.sz;
    } else if (type == KR_CODEC_AUDIO) {
      b->type = KR_CODEC_BLOCK_AUDIO;
      b->data = b->coded_audio.data;
      b->len = b->coded_audio.len;
      td.in.sz = b->coded_audio.len;
      td.in.tc = b->coded_audio.tc;
      td.in.nframes = b->coded_audio.frames;
    }
    td.out.framing = cb.buf;
    td.out.used = 0;
    td.out.space = sizeof(cb.buf);
    kr_spin_lock(&m->lock);
    ret = kr_txt_mux(m->txt, &td);
    kr_spin_unlock(&m->lock);
    cb.len = td.out.used;
    cb.sync = td.out.sync;
    ret = kr_track_write(wr, &cb);
    break;
  case KR_CONTAINER_WEBM:
  case KR_CONTAINER_MKV:
    md.track = tnum;
    md.framing = cb.buf;
    md.sz = sizeof(cb.buf);
    if (type == KR_CODEC_VIDEO) {
      b->type = KR_CODEC_BLOCK_VIDEO;
      b->data = b->frame.coded.data;
      b->len = b->frame.coded.info.sz;
      /*kr_frame_debug_time(&b->frame, "STUDIO MUX", 10);*/
      md.tc = b->frame.info.tc;
      md.pkt_len = b->frame.coded.info.sz;
      md.keyframe = b->frame.info.keyframe;
    } else if (type == KR_CODEC_AUDIO) {
      b->type = KR_CODEC_BLOCK_AUDIO;
      b->data = b->coded_audio.data;
      b->len = b->coded_audio.len;
      md.tc = b->coded_audio.tc;
      md.pkt_len = b->coded_audio.len;
      md.frames = b->coded_audio.frames;
    }
    kr_spin_lock(&m->lock);
    ret = kr_mkv_mux(m->mkv, &md);
    kr_spin_unlock(&m->lock);
    if (ret < 1) {
      printke("Mux: MKV muxer %d", ret);
      return -2;
    }
    cb.len = md.len;
    cb.sync = md.sync;
    ret = kr_track_write(wr, &cb);
    b->sync = 0;
    ret = kr_track_write(wr, b);
    break;
  case KR_CONTAINER_TS:
    if (type != KR_CODEC_AUDIO) break;
    b->type = KR_CODEC_BLOCK_AUDIO;
    memset(&tsd, 0, sizeof(tsd));
    tsd.track = tnum;
    tsd.input = b->coded_audio.data;
    tsd.sz = b->coded_audio.len;
    kr_spin_lock(&m->lock);
    ret = kr_mts_mux(m->mts, &tsd);
    kr_spin_unlock(&m->lock);
    if (ret != 0) {
      printke("Mux: MTS muxer %d", ret);
      return -4;
    }
    for (i = 0; i < tsd.npkts; i++) {
      cb.sync = 0;
      cb.len = tsd.pkts[i].hdr_sz;
      if (tsd.pkts[i].hdr_sz > sizeof(cb.buf)) {
        printke("Mux: MTS header size too big %d %zu", i, tsd.pkts[i].hdr_sz);
        exit(-1);
      }
      if (i == 0) cb.sync = 1;
      memcpy(cb.buf, tsd.pkts[i].hdr, tsd.pkts[i].hdr_sz);
      if (cb.len > 0) ret = kr_track_write(wr, &cb);
      b->data = tsd.pkts[i].buf;
      b->len = tsd.pkts[i].sz;
      if (b->len > 0) ret = kr_track_write(wr, b);
    }
    break;
  case KR_CONTAINER_OGG:
    if (b->type != KR_CODEC_BLOCK_AUDIO) break;
    od.framing_buf = cb.buf;
    od.sz = sizeof(cb.buf);
    od.track = 0;
    od.in[0].granule_pos = b->coded_audio.tc;
    od.in[0].buf = b->coded_audio.data;
    od.in[0].sz = b->coded_audio.len;
    od.in_len = 1;
    kr_spin_lock(&m->lock);
    ret = kr_ogg_mux(m->ogg, &od);
    kr_spin_unlock(&m->lock);
    if (ret < 1) {
      printke("Mux: Ogg muxer %d", ret);
      return -3;
    }
    for (i = 0; i < od.out_len; i++) {
      if (i % 2 == 0) {
        cb.sync = 1;
        cb.data = od.out[i].buf;
        cb.len = od.out[i].sz;
        ret = kr_track_write(wr, &cb);
      } else {
        b->data = od.out[i].buf;
        b->len = od.out[i].sz;
        ret = kr_track_write(wr, b);
      }
    }
    break;
  case KR_CONTAINER_OCC:
  if (b->type != KR_CODEC_BLOCK_AUDIO) break;
    ret = kr_occ_pack(b->coded_audio.data, b->coded_audio.len, &ocd);
    if (ret != 0) {
      printke("Mux: occ pack failed! %d", ret);
      break;
    }
    cb.sync = 1;
    cb.len = sizeof(ocd.pkts[0].hdr);
    memcpy(cb.buf, ocd.pkts[0].hdr, cb.len);
    b->data = ocd.pkts[0].buf;
    b->len = ocd.pkts[0].len;
    ret = kr_track_write(wr, &cb);
    ret = kr_track_write(wr, b);
    break;
  }
  return ret;
}

static int output_mux(kr_studio_path *mux_path) {
  int i;
  kr_studio *s;
  s = mux_path->studio;
  kr_studio_path *o;
  i = 0;
  while ((o = kr_pool_iterate_active(s->pool, &i))) {
    if (o->info.type != KR_STUDIO_OUTPUT) continue;
    if (!studio_output_of_mux(o, mux_path)) continue;
    studio_output_write(o);
  }
  return 0;
}

static int mux_process(kr_studio_path *path) {
  int i;
  int ret;
  studio_mux *m;
  kr_mux_info *info;
  kr_track_info track_info;
  kr_track_block block;
  if (!path) return -1;
  m = &path->mux;
  info = &path->info.mux;
  if (!info) return -1;
  for (i = 0; i < info->ntracks; i++) {
    memset(&block, 0, sizeof(block));
    ret = kr_track_read(m->head[i], &block);
    if (ret != 1) continue;
    ret = kr_track_info_get(m->track[i], &track_info);
    if (ret != 0) return -1;
    mux_block(path, i, track_info.codec.type, &block);
    output_mux(path);
  }
  /*printk("Muxer: process block #%zu", m->block_num);*/
  return 1;
}

static void mux_destroy_outputs(studio_mux *mux) {
  int i;
  for (i = 0; i < KR_MUXER_MAX_OUTPUTS; i++) {
    if (mux->outputs[i] != NULL) kr_studio_remove(mux->outputs[i]);
  }
}

static int mux_input_track_event(kr_track_event *e) {
  kr_studio_path *path;
  path = (kr_studio_path *)e->user;
  if (e->type == KR_TRACK_READ) {
    printkd("Muxer: input track read event");
  } else if (e->type == KR_TRACK_WRITE) {
    /*printk("Muxer: input track write event");*/
    mux_process(path);
  } else if (e->type == KR_TRACK_REMOVE) {
    printk("Muxer: Track I was reading from was removed!");
    return kr_studio_remove(path);
  }
  return 0;
}

static int kr_raw_mux_ok(kr_codec *c) {
  int ret;
  ret = 0;
  switch (c->audio) {
    case KR_AUDIO_CODEC_MP3:
      ret = 1;
      break;
    case KR_AUDIO_CODEC_AAC:
      ret = 1;
      break;
    case KR_AUDIO_CODEC_OPUS:
      ret = 1;
      break;
    default: break;
  }
  return ret;
}

static int mux_ok(kr_studio_path *path) {
  int ret;
  int i;
  studio_mux *m;
  kr_mux_info *info;
  kr_track_info track_info;
  kr_codec track_codecs[KR_MUXER_MAX_NTRACKS];
  ret = 0;
  m = &path->mux;
  info = &path->info.mux;
  if (info->ntracks == 0) {
    printke("Muxer: Tried to mux with 0 tracks, lame.");
    return 0;
  }
  for (i = 0; i < info->ntracks; i++) {
    if (i == sizeof(info->input[i])) return 0;
    m->track[i] = kr_track_find_by_name(path->studio->tracker,
     info->input[i].name, info->input[i].len);
    if (!m->track[i]) {
      printke("Muxer: failed to find track!");
      return 0;
    }
    ret = kr_track_info_get(m->track[i], &track_info);
    if (ret != 0) {
      printke("Muxer: failed to get track info!");
      return 0;
    }
    track_codecs[i] = track_info.codec;
  }
  ret = 0;
  switch (info->type) {
  case KR_CONTAINER_RAW:
    if (info->ntracks != 1) break;
    ret = kr_raw_mux_ok(track_codecs);
    break;
  case KR_CONTAINER_TS:
    ret = kr_mts_mux_ok(track_codecs, info->ntracks);
    break;
  case KR_CONTAINER_MKV:
    ret = 1; /* MKV Handles All */
    break;
  case KR_CONTAINER_WEBM:
    ret = 1; /* FIXME, well just cross our fingers for now */
    break;
  case KR_CONTAINER_OGG:
    ret = 1; /* FIXME, well just cross our fingers for now */
    break;
  case KR_CONTAINER_FLAC:
    ret = kr_flac_mux_ok(track_codecs, info->ntracks);
    break;
  case KR_CONTAINER_Y4M:
    ret = kr_y4m_mux_ok(track_codecs, info->ntracks);
    break;
  case KR_CONTAINER_TXT:
    if (info->ntracks <= KR_TXT_MUXER_NTRACKS) ret = 1;
    break;
  case KR_CONTAINER_OCC:
    ret = kr_occ_mux_ok(track_codecs, info->ntracks);
    break;
  default:
    ret = 0;
    break;
  }
  if (!ret) printke("Muxer: mux was not okay. %d tracks", info->ntracks);
  return ret;
}

static int muxed_track_event(kr_track_event *e) {
  //kr_studio_path *path;
  //path = (kr_studio_path *)e->user;
  if (e->type == KR_TRACK_READ) {
    printkd("Muxed track read event");
  } else if (e->type == KR_TRACK_WRITE) {
    printkd("Muxed track write event");
  }
  return 0;
}

static int setup_output_track(kr_studio_path *path, int nblocks) {
  int ret;
  kr_name *name;
  studio_mux *m;
  kr_codec_header cheader;
  /*kr_mux_info *info;*/
  kr_track_info track_info;
  kr_track_setup track_setup;
  kr_track_head_setup head_setup;
  memset(&track_info, 0, sizeof(track_info));
  memset(&track_setup, 0, sizeof(track_setup));
  memset(&head_setup, 0, sizeof(head_setup));
  ret = 0;
  m = &path->mux;
  /*info = &path->info.mux;*/
  track_info.mode = KR_TRACK_INFINITE;
  track_info.duration = nblocks;
  track_info.type = KR_TRACK_MUX;
  name = (kr_name *)path->user;
  memcpy(track_info.track.name, name->buf, name->len);
  track_info.track.len = name->len;
  track_setup.user = path;
  track_setup.info = &track_info;
  track_setup.header.data[0].buf = m->header;
  track_setup.header.data[0].sz = m->header_sz;
  track_setup.header.len = 1;
  m->output.track = kr_track_create(path->studio->tracker, &track_setup);
  if (!m->output.track) return -18;
  head_setup.user = path;
  head_setup.type = KR_TRACK_WRITE_HEAD;
  head_setup.track_ev_cb = muxed_track_event;
  /*head_setup.erase_cb = erase_coded;*/
  m->output.head = kr_track_open(m->output.track, &head_setup);
  if (!m->output.head) return -20;

  /* kludge */
  kr_track_codec_header_get(m->head[0], &cheader);
  kr_track_codec_header_set(m->output.head, &cheader);

  printk("Muxer output track created");
  return ret;
}

int studio_mux_add_output(studio_mux *mux, kr_studio_path *out) {
  int i;
  if (!mux || !out) return -1;
  if (mux->noutputs == KR_MUXER_MAX_OUTPUTS) {
    printke("Studio Mux: cannot add more outputs!");
    return -2;
  }
  for (i = 0; i < KR_MUXER_MAX_OUTPUTS; i++) {
    if (mux->outputs[i] == NULL) {
      mux->outputs[i] = out;
      mux->noutputs++;
      return 0;
    }
  }
  return -3;
}

int studio_mux_rm_output(studio_mux *mux, kr_studio_path *out) {
  int i;
  if (!mux || !out) return -1;
  if (mux->noutputs == 0) {
    printke("Studio Mux: no outputs to be removed!");
    return -2;
  }
  for (i = 0; i < KR_MUXER_MAX_OUTPUTS; i++) {
    if (mux->outputs[i] == out) {
      mux->outputs[i] = NULL;
      mux->noutputs--;
      return 0;
    }
  }
  return -3;
}

int studio_mux_destroy(kr_studio_path *path) {
  int i;
  int ret;
  studio_mux *m;
  kr_mux_info *info;
  if (!path) {
    printke("Mux: destroy called on invalid path");
    return -1;
  }
  m = &path->mux;
  info = &path->info.mux;
  kr_spin_lock(&m->lock);
  mux_destroy_outputs(&path->mux);
  for (i = 0; i < info->ntracks; i++) {
    if (!m->head[i]) continue;
    ret = kr_track_close(m->head[i]);
    if (ret) printke("Mux: track %d close ret %d", i, ret);
    m->head[i] = NULL;
  }
  if (m->output.track) {
    if (m->output.head) {
      ret = kr_track_close(m->output.head);
      if (ret) printke("Mux: output track close ret %d", ret);
      m->output.head = NULL;
    }
    ret = kr_track_remove(m->output.track);
    if (ret) printke("Mux: output track destroy ret %d", ret);
    m->output.track = NULL;
  }
  kr_spin_unlock(&m->lock);
  printk("Mux: Destroyed");
  return 0;
}

int studio_mux_create(kr_studio_path *path) {
  int i;
  int ret;
  studio_mux *m;
  kr_mux_info *info;
  kr_codec_header codec_header;
  union {
    kr_y4m_muxer_setup y4m;
    kr_flac_muxer_setup flac;
    kr_ogg_muxer_setup ogg;
    kr_mkv_muxer_setup mkv;
    kr_txt_muxer_setup txt;
    kr_mts_muxer_setup mts;
  } setup;
  kr_track_head_setup head_setup;
  memset(&codec_header, 0, sizeof(codec_header));
  memset(&setup, 0, sizeof(setup));
  memset(&head_setup, 0, sizeof(head_setup));
  if (!path) return -1;
  ret = 0;
  m = &path->mux;
  info = &path->info.mux;
  kr_spin_init(&m->lock);
  if (!mux_ok(path)) return -1;
  for (i = 0; i < info->ntracks; i++) {
    head_setup.user = path;
    head_setup.type = KR_TRACK_READ_HEAD;
    head_setup.track_ev_cb = mux_input_track_event;
    m->head[i] = kr_track_open(m->track[i], &head_setup);
    if (!m->head[i]) {
      printke("Muxer: track read head fail!!");
      return -1;
    }
  }
  switch (info->type) {
  case KR_CONTAINER_TS:
    if (sizeof(m->mem) < kr_mts_muxer_sizeof()) {
      printke("MTS is bigger than we thunk!");
      exit(1);
    }
    if (info->ntracks != 1) {
      ret = -1;
      break;
    }
    setup.mts.tracks = info->ntracks;
    setup.mts.mem = m->mem;
    kr_track_info_get(m->track[0], &setup.mts.track_info[0]);
    m->mts = kr_mts_muxer_init(&setup.mts);
    if (!m->mts) {
      printk("Muxer: mts_muxer_init fail");
      ret = -1;
      break;
    }
    m->header_sz = kr_mts_muxer_pack_header(m->mts, m->header,
     sizeof(m->header));
    printk("mts header size is %zu", m->header_sz);
    break;
  case KR_CONTAINER_RAW:
    break;
  case KR_CONTAINER_TXT:
    if (sizeof(m->mem) < kr_txt_muxer_sizeof()) {
      printke("MUXTEXT is bigger than we thunk!");
      exit(1);
    }
    setup.txt.mem = m->mem;
    setup.txt.comment_len = snprintf(setup.txt.comment,
     sizeof(setup.txt.comment), "Muxed with %s", KR_VERSION_STR_FULL);
    setup.txt.tracks = info->ntracks;
    for (i = 0; i < info->ntracks; i++) {
      kr_track_info_get(m->track[i], &setup.txt.track[i].info);
      ret = kr_track_codec_header_get(m->head[i], &setup.txt.track[i].header);
    }
    m->txt = kr_txt_muxer_init(&setup.txt);
    if (m->txt) {
      printkd("Muxer: MUXTEXT yeah!");
    } else {
      printke("Muxer: MUXTEXT faily :(");
      ret = -1;
      break;
    }
    m->header_sz = kr_txt_muxer_pack_header(m->txt, m->header,
     sizeof(m->header));
    printkd("Muxer: Header is %zu bytes", m->header_sz);
    break;
  case KR_CONTAINER_WEBM:
  case KR_CONTAINER_MKV:
    if (sizeof(m->mem) < kr_mkv_muxer_sizeof()) {
      printke("MKV is bigger than we thunk!");
      exit(1);
    }
    setup.mkv.mem = m->mem;
    setup.mkv.type = info->type;
    setup.mkv.tracks = info->ntracks;
    for (i = 0; i < info->ntracks; i++) {
      kr_track_info_get(m->track[i], &setup.mkv.info[i]);
      ret = kr_track_codec_header_get(m->head[i], &setup.mkv.header[i]);
    }
    m->mkv = kr_mkv_muxer_init(&setup.mkv);
    if (m->mkv) {
      printkd("Muxer: MKV yeah!");
    } else {
      printkd("Muxer: MKV faily :(");
      ret = -1;
      break;
    }
    m->header_sz = kr_mkv_muxer_pack_header(m->mkv, m->header,
     sizeof(m->header));
    printkd("header size is %zu", m->header_sz);
    break;
  case KR_CONTAINER_OGG:
    if (sizeof(m->mem) < kr_ogg_muxer_sizeof()) {
      printke("Ogg is bigger than we thunk!");
      exit(1);
    }
    ret = kr_track_codec_header_get(m->head[0], &codec_header);
    if (codec_header.len < 1) {
      printke("Muxer: ret %d wrong num headers: %d", ret, codec_header.len);
      ret = -1;
      break;
    }
    setup.ogg.tracks = 1;
    setup.ogg.mem = m->mem;
    setup.ogg.header[0] = codec_header;
    printkd("header 0 sz %d", setup.ogg.header[0].data[0].sz);
    printkd("header 1 sz %d", setup.ogg.header[0].data[1].sz);
    printkd("header 2 sz %d", setup.ogg.header[0].data[2].sz);
    m->ogg = kr_ogg_muxer_init(&setup.ogg);
    if (m->ogg) {
      printkd("Muxer: Ogg yeah!");
    } else {
      printkd("Muxer: Ogg faily :(");
      ret = -1;
      break;
    }
    m->header_sz = kr_ogg_muxer_read_header(m->ogg, m->header,
     sizeof(m->header));
    printkd("header size is %zu", m->header_sz);
    break;
  case KR_CONTAINER_FLAC:
    if (sizeof(m->mem) < kr_flac_muxer_sizeof()) {
      printke("Flac is bigger than we thunk!");
      exit(1);
    }
    ret = kr_track_codec_header_get(m->head[0], &codec_header);
    if (codec_header.len != 1) {
      printke("Muxer: awee wrong number of headers.. %d", ret);
      break;
    }
    setup.flac.header = codec_header.data[0].buf;
    setup.flac.header_sz = codec_header.data[0].sz;
    setup.flac.mem = m->mem;
    printk("Muxer: FLAC codec header is %zu bytes", codec_header.data[0].sz);
    m->flac = kr_flac_muxer_init(&setup.flac);
    if (m->flac) {
      printkd("Muxer: YAY FLAC!!");
    } else {
      printkd("Muxer: YAY oh noe :(");
    }
    m->header_sz = kr_flac_pack_header(m->flac, m->header, sizeof(m->header));
    printk("Muxer: FLAC muxer header is %zu bytes", m->header_sz);
    break;
  case KR_CONTAINER_Y4M:
    ret = kr_track_codec_header_get(m->head[0], &codec_header);
    if (codec_header.len != 1) {
      printke("Muxer: awee wrong number of headers.. %d", ret);
      break;
    }
    setup.y4m.header = codec_header.data[0].buf;
    setup.y4m.header_sz = codec_header.data[0].sz;
    setup.y4m.mem = m->mem;
    printkd("Muxer: Y4M codec header is %zu bytes", codec_header.data[0].sz);
    m->y4m = kr_y4m_muxer_init(&setup.y4m);
    if (m->y4m) {
      printkd("Muxer: YAY y4m!!");
    } else {
      printkd("Muxer: YAY oh noe :(");
    }
    m->header_sz = kr_y4m_pack_header(m->y4m, m->header, sizeof(m->header));
    printkd("Muxer: Y4M muxer header is %zu bytes", m->header_sz);
    break;
  case KR_CONTAINER_OCC:
    printk("OCC init: nothing to do yet :)");
    break;
  default:
    printke("Muxer: Unknown type of muxing!!");
    ret = -1;
    break;
  }
  if (ret) {
    for (i = 0; i < info->ntracks; i++) {
      if (m->head[i]) kr_track_close(m->head[i]);
    }
  } else {
    ret = setup_output_track(path, DEFAULT_ENCODE_NBLOCKS);
  }
  return ret;
}
