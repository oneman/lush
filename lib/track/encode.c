#include "encode.h"
#include <krad/image/debug.h>
#include <krad/app/name.h>
#include <krad/audio/debug.h>

#define DEFAULT_ENCODE_NBLOCKS 1024

static int erase_coded(void *user, void *block_user);
static int write_coded_frame(kr_track_head *head, kr_frame *frame);
static int write_coded_audio(kr_track_head *head, kr_coded_audio *a);
static int read_raw_frame(kr_track_head *head, kr_frame *frame);
static int read_raw_audio(kr_track_head *head, kr_audio *audio);
static int get_codec_frame_size(kr_studio_path *path);
static int get_raw_audio(kr_studio_path *p, kr_audio *a);
static void store_raw_audio(kr_studio_path *p, kr_audio *a);
static int encode_process(kr_studio_path *path);
static int encoded_track_event(kr_track_event *e);
static int input_track_event(kr_track_event *e);

static int erase_coded(void *user, void *block_user) {
  int ret;
  kr_studio_path *path;
  if (!user) return -1;
  path = (kr_studio_path *)user;
  ret = kr_pool_release(path->encode.coded, block_user);
  if (ret != 0) {
    printke("Encode pool release ret %d", ret);
    return ret;
  }
  return 0;
}

static int write_coded_frame(kr_track_head *head, kr_frame *frame) {
  kr_track_block block;
  memset(&block, 0, sizeof(block));
  block.user = frame->coded.data;
  block.frame = *frame;
  block.type = KR_CODEC_BLOCK_VIDEO;
  if (frame->info.keyframe) block.sync = 1;
  return kr_track_write(head, &block);
}

static int write_coded_audio(kr_track_head *head, kr_coded_audio *a) {
  kr_track_block block;
  memset(&block, 0, sizeof(block));
  block.user = a->data;
  block.coded_audio = *a;
  block.type = KR_CODEC_BLOCK_AUDIO;
  //kr_coded_audio_debug(a);
  return kr_track_write(head, &block);
}

static int read_raw_frame(kr_track_head *head, kr_frame *frame) {
  int ret;
  kr_track_block block;
  ret = kr_track_read(head, &block);
  *frame = block.frame;
  return ret;
}

static int read_raw_audio(kr_track_head *head, kr_audio *audio) {
  int ret;
  kr_track_block block;
  ret = kr_track_read(head, &block);
  if (ret == 1) *audio = block.audio;
  //if (ret == 1) kr_audio_debug(audio);
  return ret;
}

static int get_codec_frame_size(kr_studio_path *path) {
  kr_encode_info *info;
  info = &path->info.encode;
  switch (info->audio.codec) {
  case KR_AUDIO_CODEC_FLAC:
    return info->audio.flac.frame_size;
  case KR_AUDIO_CODEC_VORBIS:
    return 1024; /* w/e ? :P */
  case KR_AUDIO_CODEC_OPUS:
    return info->audio.opus.frame_size;
  case KR_AUDIO_CODEC_MP3:
    return 1152; /* just guessing */
  case KR_AUDIO_CODEC_AAC:
    return 2048;
  }
  return 0;
}

static int get_raw_audio(kr_studio_path *p, kr_audio *a) {
  int i;
  int c;
  size_t avail;
  studio_encode *e;
  size_t read;
  read = 0;
  e = &p->encode;
  a->count = get_codec_frame_size(p);
  if (a->count < 1) return 0;
  c = KR_ENCODE_NCHANNELS;
  for (i = 0; i < c; i++) {
    a->samples[i] = e->samples[i];
  }
  avail = krad_ringbuffer_read_space(e->audio[c - 1]);
  if (a->count * sizeof(float) > avail) {
    return 0;
  }
  c = a->channels;
  for (i = 0; i < c; i++) {
    read = krad_ringbuffer_read(e->audio[i], (char *)a->samples[i],
     a->count * sizeof(float));
  }
  return read;
}

static void store_raw_audio(kr_studio_path *p, kr_audio *a) {
  int i;
  int c;
  studio_encode *e;
  e = &p->encode;
  c = a->channels;
  for (i = 0; i < c; i++) {
    if (krad_ringbuffer_write_space(e->audio[i]) < (a->count * sizeof(float))) {
      printke("Encoder: store raw audio: not enough space in ringbuffer!");
    }
    krad_ringbuffer_write(e->audio[i], (char *)a->samples[i],
     a->count * sizeof(float));
  }
}

static int encode_process(kr_studio_path *path) {
  int ret;
  studio_encode *e;
  kr_image_setup image_setup;
  kr_encode_info *info;
  kr_frame raw;
  kr_frame coded;
  kr_audio raw_audio;
  kr_coded_audio coded_audio;
  int dbgline;
  char dbgtext[64];
  memset(&raw_audio, 0, sizeof(raw_audio));
  memset(&coded_audio, 0, sizeof(coded_audio));
  ret = -1;
  if (!path) return -1;
  e = &path->encode;
  info = &path->info.encode;
  e->frames++;
  if (info->type == KR_ENCODE_VIDEO) {
    ret = read_raw_frame(e->rd, &raw);
    coded.coded.data = kr_pool_slice(e->coded);
    if (!coded.coded.data) return 0;
  } else if (info->type == KR_ENCODE_AUDIO) {
    for (;;) {
      ret = read_raw_audio(e->rd, &raw_audio);
      if (!ret) break;
      store_raw_audio(path, &raw_audio);
    }
  }
  dbgline = 5;
  if (info->type == KR_ENCODE_VIDEO) {
    switch (info->video.codec) {
    case KR_VIDEO_CODEC_DAALA:
      #ifdef KR_USE_DAALA
      image_setup.info.w = info->video.daala.width;
      image_setup.info.h = info->video.daala.height;
      image_setup.info.fmt = info->video.daala.pixel_fmt;
      image_setup.mem = raw.image.px[0];
      image_setup.img = &raw.image;
      kr_image_init2(&image_setup);
      snprintf(dbgtext, sizeof(dbgtext), "DAALA ENC ;%010"PRIu64"", e->frames);
      kr_image_debug_render(&raw.image, dbgline, dbgtext);
      ret = kr_daala_encode(e->daala, &coded, &raw);
      #endif
      break;
    case KR_VIDEO_CODEC_VPX:
      image_setup.info.w = info->video.vpx.width;
      image_setup.info.h = info->video.vpx.height;
      image_setup.info.fmt = info->video.vpx.pixel_fmt;
      image_setup.mem = raw.image.px[0];
      image_setup.img = &raw.image;
      kr_image_init2(&image_setup);
      snprintf(dbgtext, sizeof(dbgtext), "VPX ENC ;%010"PRIu64"", e->frames);
      kr_image_debug_render(&raw.image, dbgline, dbgtext);
      ret = kr_vpx_encode(e->vpx, &coded, &raw);
      printk("coded %zu", coded.coded.info.sz);
      break;
    case KR_VIDEO_CODEC_THEORA:
      image_setup.info.w = info->video.theora.width;
      image_setup.info.h = info->video.theora.height;
      image_setup.info.fmt = info->video.theora.pixel_fmt;
      image_setup.mem = raw.image.px[0];
      image_setup.img = &raw.image;
      kr_image_init2(&image_setup);
      snprintf(dbgtext, sizeof(dbgtext), "THEORA ENC ;%010"PRIu64"", e->frames);
      kr_image_debug_render(&raw.image, dbgline, dbgtext);
      ret = kr_theora_encode(e->theora, &coded, &raw);
      break;
    case KR_VIDEO_CODEC_Y4M:
      image_setup.info.w = info->video.y4m.width;
      image_setup.info.h = info->video.y4m.height;
      image_setup.info.fmt = info->video.y4m.pixel_fmt;
      image_setup.mem = raw.image.px[0];
      image_setup.img = &raw.image;
      kr_image_init2(&image_setup);
      snprintf(dbgtext, sizeof(dbgtext), "Y4M ENC ;%010"PRIu64"", e->frames);
      kr_image_debug_render(&raw.image, dbgline, dbgtext);
      ret = kr_y4m_encode(e->y4m, &coded, &raw);
      break;
    default:
      return -1;
    }
    if (ret) printke("Encode: ret %d", ret);
    if (ret < 0) exit(ret);
    ret = write_coded_frame(e->wr, &coded);
    if (ret) printke("Encode: write_coded ret %d", ret);
  } else if (info->type == KR_ENCODE_AUDIO) {
    while ( get_raw_audio(path, &raw_audio) > 0 ) {
      coded_audio.data = kr_pool_slice(e->coded);
      coded_audio.sz = kr_pool_slice_size(e->coded);
      if (!coded_audio.data) {
        printke("Encoder: Failed to slice coded audio");
        return -1;
      }
      switch (info->audio.codec) {
      case KR_AUDIO_CODEC_FLAC:
        ret = kr_flac_encode(e->flac, &coded_audio, &raw_audio);
        kr_flac_encoder_get_info(e->flac, &info->audio.flac);
        break;
      case KR_AUDIO_CODEC_OPUS:
        ret = kr_opus_encode(e->opus, &coded_audio, &raw_audio);
        kr_opus_encoder_get_info(e->opus, &info->audio.opus);
        break;
      case KR_AUDIO_CODEC_VORBIS:
        ret = kr_vorbis_encode(e->vorbis, &coded_audio, &raw_audio);
        while (ret == 1) {
          ret = write_coded_audio(e->wr, &coded_audio);
          memset(&coded_audio, 0, sizeof(coded_audio));
          coded_audio.sz = kr_pool_slice_size(e->coded);
          coded_audio.data = kr_pool_slice(e->coded);
          if (!coded_audio.data) exit(1);
          ret = kr_vorbis_encode(e->vorbis, &coded_audio, NULL);
        }
        kr_vorbis_encoder_get_info(e->vorbis, &info->audio.vorbis);
        kr_pool_release(e->coded, coded_audio.data);
        return 1;
      case KR_AUDIO_CODEC_MP3:
        ret = kr_mp3_encode(e->mp3, &coded_audio, &raw_audio);
        kr_mp3_encoder_get_info(e->mp3, &info->audio.mp3);
        break;
      case KR_AUDIO_CODEC_AAC:
        ret = kr_aac_encode(e->aac, &coded_audio, &raw_audio);
        kr_aac_encoder_get_info(e->aac, &info->audio.aac);
        break;
        default:
        return -1;
      }
      if (ret) printke("Encode: ret %d", ret);
      if (ret < 0) exit(ret);
      ret = write_coded_audio(e->wr, &coded_audio);
      if (ret) printke("Encode: write_coded ret %d", ret);
    }
  }
  return 0;
}

static int encoded_track_event(kr_track_event *e) {
  if (e->type == KR_TRACK_READ) {
    /*printk("Studio: Encoder output track read event");*/
  }
  if (e->type == KR_TRACK_WRITE) {
    printk("Studio: Encoder output track write event");
  }
  return 0;
}

static int input_track_event(kr_track_event *e) {
  kr_studio_path *path;
  path = (kr_studio_path *)e->user;
  if (e->type == KR_TRACK_READ) {
    /*printk("Studio: Encoder input track read event");*/
  }
  if (e->type == KR_TRACK_WRITE) {
    /*printk("Studio: Encoder input track write event");*/
    if (path->info.encode.type == KR_ENCODE_AUDIO) {
      while (encode_process(path) == 1);
    } else {
      encode_process(path);
    }
  }
  if (e->type == KR_TRACK_REMOVE) {
    printk("Encode: Track I was reading from was removed!");
    return kr_studio_remove(path);
  }
  return 0;
}

int studio_encode_destroy(kr_studio_path *path) {
  int ret;
  int i;
  studio_encode *e;
  kr_encode_info *info;
  if (!path) return -1;
  ret = 0;
  e = &path->encode;
  info = &path->info.encode;
  if (info->type == KR_ENCODE_VIDEO) {
    switch (info->video.codec) {
    case KR_VIDEO_CODEC_DAALA:
      #ifdef KR_USE_DAALA
      ret = kr_daala_encoder_destroy(e->daala);
      #endif
      break;
    case KR_VIDEO_CODEC_VPX:
      ret = kr_vpx_encoder_destroy(e->vpx);
      break;
    case KR_VIDEO_CODEC_THEORA:
      ret = kr_theora_encoder_destroy(e->theora);
      break;
    case KR_VIDEO_CODEC_Y4M: break;
    default: break;
    }
  } else if (info->type == KR_ENCODE_AUDIO) {
    switch (info->audio.codec) {
    case KR_AUDIO_CODEC_FLAC:
      ret = kr_flac_encoder_destroy(e->flac);
      break;
    case KR_AUDIO_CODEC_OPUS:
      ret = kr_opus_encoder_destroy(e->opus);
      break;
    case KR_AUDIO_CODEC_VORBIS:
      ret = kr_vorbis_encoder_destroy(e->vorbis);
      break;
    case KR_AUDIO_CODEC_MP3:
      ret = kr_mp3_encoder_destroy(e->mp3);
      break;
    case KR_AUDIO_CODEC_AAC:
      ret = kr_aac_encoder_destroy(e->aac);
      break;
    default: break;
    }
    for (i = 0; i < KR_ENCODE_NCHANNELS; i++) {
      krad_ringbuffer_free(e->audio[i]);
    }
  }
  kr_track_close(e->rd);
  kr_track_close(e->wr);
  kr_track_remove(e->out);
  kr_pool_destroy(e->coded);
  return ret;
}

int studio_encode_create(kr_studio_path *path) {
  int ret;
  int i;
  studio_encode *e;
  kr_name *name;
  kr_encode_info *info;
  union {
    kr_y4m_encoder_setup y4m;
    kr_h264_encoder_setup h264;
    kr_theora_encoder_setup theora;
    #ifdef KR_USE_DAALA
    kr_daala_encoder_setup daala;
    #endif
    kr_vpx_encoder_setup vpx;
    kr_vorbis_encoder_setup vorbis;
    kr_opus_encoder_setup opus;
    kr_flac_encoder_setup flac;
    kr_mp3_encoder_setup mp3;
    kr_aac_encoder_setup aac;
  } enc_setup;
  kr_track_info track_info;
  kr_track_setup track_setup;
  kr_track_head_setup head_setup;
  kr_pool_setup pool_setup;
  memset(&track_info, 0, sizeof(track_info));
  memset(&track_setup, 0, sizeof(track_setup));
  memset(&head_setup, 0, sizeof(head_setup));
  memset(&pool_setup, 0, sizeof(pool_setup));
  memset(&enc_setup, 0, sizeof(enc_setup));
  e = &path->encode;
  e->frames = 0;
  info = &path->info.encode;
  head_setup.user = path;
  e->in = kr_track_find_by_name(path->studio->tracker,
   info->in.name, info->in.len);
  if (!e->in) return -3;
  if (info->type == KR_ENCODE_VIDEO) {
    track_info.type = KR_TRACK_CODED_VIDEO;
    track_info.codec.type = KR_CODEC_VIDEO;
    track_info.codec.video = info->video.codec;
    switch (info->video.codec) {
    case KR_VIDEO_CODEC_DAALA:
      #ifdef KR_USE_DAALA
      enc_setup.daala.info = info->video.daala;
      enc_setup.daala.mem = e->mem;
      e->daala = kr_daala_encoder_init(&enc_setup.daala);
      if (!e->daala) return -4;
      ret = kr_daala_encoder_get_headers(e->daala, &track_setup.header);
      if (ret) return -33;
      pool_setup.size = info->video.daala.width * info->video.daala.height * 3;
      #endif
      break;
    case KR_VIDEO_CODEC_VPX:
      enc_setup.vpx.info = info->video.vpx;
      enc_setup.vpx.mem = e->mem;
      track_info.video.width = info->video.vpx.width;
      track_info.video.height = info->video.vpx.height;
      track_info.video.fps_num = info->video.vpx.fps_num;
      track_info.video.fps_den = info->video.vpx.fps_den;
      track_info.video.pixel_fmt = info->video.vpx.pixel_fmt;
      e->vpx = kr_vpx_encoder_init(&enc_setup.vpx);
      if (!e->vpx) return -5;
      pool_setup.size = info->video.vpx.width * info->video.vpx.height * 3;
      break;
    case KR_VIDEO_CODEC_THEORA:
      enc_setup.theora.info = info->video.theora;
      enc_setup.theora.mem = e->mem;
      e->theora = kr_theora_encoder_init(&enc_setup.theora);
      if (!e->theora) return -6;
      ret = kr_theora_encoder_get_headers(e->theora, &track_setup.header);
      if (ret) return -55;
      pool_setup.size = info->video.theora.width * info->video.theora.height * 3;
      break;
    case KR_VIDEO_CODEC_Y4M:
      if (kr_y4m_encoder_sizeof() > sizeof(e->mem)) return -7;
      enc_setup.y4m.info = info->video.y4m;
      enc_setup.y4m.mem = e->mem;
      e->y4m = kr_y4m_encoder_init(&enc_setup.y4m);
      if (!e->y4m) return -8;
      ret = kr_y4m_encoder_get_headers(e->y4m, &track_setup.header);
      if (ret) return -66;
      pool_setup.size = info->video.y4m.width * info->video.y4m.height * 3;
      break;
    default:
      return -9;
    }
  } else if (info->type == KR_ENCODE_AUDIO) {
    track_info.type = KR_TRACK_CODED_AUDIO;
    track_info.codec.type = KR_CODEC_AUDIO;
    track_info.codec.audio = info->audio.codec;
    for (i = 0; i < KR_ENCODE_NCHANNELS; i++) {
      e->audio[i] = krad_ringbuffer_create(5 * 4096 * sizeof(float));
    }
    switch (info->audio.codec) {
    case KR_AUDIO_CODEC_FLAC:
      track_info.audio.sample_rate = info->audio.flac.sample_rate;
      track_info.audio.channels = info->audio.flac.channels;
      track_info.audio.bit_depth = info->audio.flac.bit_depth;
      enc_setup.flac.info = info->audio.flac;
      enc_setup.flac.mem = e->mem;
      if (sizeof(e->mem) < kr_flac_encoder_sizeof()) {
        printke("Encoder: Fuuuuuuuuuuuuu not enuf mem! need %zu bytes",
         kr_flac_encoder_sizeof());
        return -111;
      }
      e->flac = kr_flac_encoder_init(&enc_setup.flac);
      if (!e->flac) return -11;
      kr_flac_encoder_get_info(e->flac, &info->audio.flac);
      ret = kr_flac_encoder_get_headers(e->flac, &track_setup.header);
      if (ret) return -1111;
      pool_setup.size = info->audio.flac.channels * 4096 * 4;
      break;
    case KR_AUDIO_CODEC_OPUS:
      track_info.audio.sample_rate = info->audio.opus.sample_rate;
      track_info.audio.channels = info->audio.opus.channels;
      enc_setup.opus.info = info->audio.opus;
      enc_setup.opus.mem = e->mem;
      if (sizeof(e->mem) < kr_opus_encoder_sizeof()) {
        printke("Encoder: Fuuuuuuuuuuuuu not enuf mem! need %zu bytes",
         kr_opus_encoder_sizeof());
        return -444;
      }
      e->opus = kr_opus_encoder_init(&enc_setup.opus);
      if (!e->opus) return -12;
      ret = kr_opus_encoder_get_headers(e->opus, &track_setup.header);
      if (ret) return -1212;
      pool_setup.size = info->audio.opus.channels * 1024;
      break;
    case KR_AUDIO_CODEC_VORBIS:
      enc_setup.vorbis.info = info->audio.vorbis;
      enc_setup.vorbis.mem = e->mem;
      if (sizeof(e->mem) < kr_vorbis_encoder_sizeof()) {
        printke("Encoder: Fuuuuuuuuuuuuu not enuf mem! need %zu bytes",
         kr_vorbis_encoder_sizeof());
        return -555;
      }
      e->vorbis = kr_vorbis_encoder_init(&enc_setup.vorbis);
      if (!e->vorbis) return -13;
      ret = kr_vorbis_encoder_get_headers(e->vorbis, &track_setup.header);
      if (ret) return -1313;
      pool_setup.size = info->audio.vorbis.channels * 1024;
      break;
    case KR_AUDIO_CODEC_MP3:
      track_info.audio.sample_rate = info->audio.mp3.sample_rate;
      track_info.audio.channels = info->audio.mp3.channels;
      track_info.audio.samples = get_codec_frame_size(path);
      enc_setup.mp3.mem = e->mem;
      enc_setup.mp3.info = info->audio.mp3;
      if (sizeof(e->mem) < kr_mp3_encoder_sizeof()) {
        printke("Encoder: Fuuuuuuuuuuuuu not enuf mem! need %zu bytes",
         kr_mp3_encoder_sizeof());
        return -666;
      }
      e->mp3 = kr_mp3_encoder_init(&enc_setup.mp3);
      if (!e->mp3) return -14;
      pool_setup.size = info->audio.mp3.channels * 1024;
      break;
    case KR_AUDIO_CODEC_AAC:
      track_info.audio.sample_rate = info->audio.aac.sample_rate;
      track_info.audio.channels = info->audio.aac.channels;
      track_info.audio.samples = get_codec_frame_size(path);
      enc_setup.aac.mem = e->mem;
      enc_setup.aac.info = info->audio.aac;
      if (sizeof(e->mem) < kr_aac_encoder_sizeof()) {
        printke("Encoder: Fuuuuuuuuuuuuu not enuf mem! need %zu bytes",
         kr_aac_encoder_sizeof());
        return -777;
      }
      e->aac = kr_aac_encoder_init(&enc_setup.aac);
      if (!e->aac) return -15;
      pool_setup.size = info->audio.aac.channels * 1024;
      break;
    default:
      return -16;
    }
  } else {
    return -17;
  }
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = 0;
  track_info.mode = KR_TRACK_INFINITE;
  track_info.duration = DEFAULT_ENCODE_NBLOCKS;
  name = (kr_name *)path->user;
  memcpy(track_info.track.name, name->buf, name->len);
  track_info.track.len = name->len;
  track_setup.user = path;
  track_setup.info = &track_info;
  e->out = kr_track_create(path->studio->tracker, &track_setup);
  if (!e->out) return -18;
  info->out.len = kr_track_name_get(e->out, info->out.name,
   sizeof(info->out.name));
  pool_setup.size += 8;
  pool_setup.slices = DEFAULT_ENCODE_NBLOCKS;
  e->coded = kr_pool_create(&pool_setup);
  if (!e->coded) return -19;
  head_setup.user = path;
  head_setup.type = KR_TRACK_WRITE_HEAD;
  head_setup.track_ev_cb = encoded_track_event;
  head_setup.erase_cb = erase_coded;
  e->wr = kr_track_open(e->out, &head_setup);
  if (!e->wr) return -20;
  head_setup.type = KR_TRACK_READ_HEAD;
  head_setup.track_ev_cb = input_track_event;
  e->rd = kr_track_open(e->in, &head_setup);
  if (!e->rd) return -21;
  return 0;
}
