#include <krad/image/debug.h>
#include <krad/app/name.h>
#include <krad/audio/debug.h>
#include "play.h"

static ssize_t play_fetch_audio(studio_play *p, kr_audio *a) {
  int i;
  int c;
  size_t avail;
  size_t read;
  read = 0;
  if (a->count < 1) return 0;
  c = KR_AUDIO_PLAY_NCHANNELS;
  avail = krad_ringbuffer_read_space(p->audio.samples[c - 1]);
  if (a->count * sizeof(float) > avail) return 0;
  c = a->channels;
  for (i = 0; i < c; i++) {
    read = krad_ringbuffer_read(p->audio.samples[i], (char *)a->samples[i],
     a->count * sizeof(float));
  }
  return read;
}

static int play_store_audio(studio_play *p, kr_audio *a) {
  int i;
  int c;
  c = a->channels;
  if (c > KR_AUDIO_PLAY_NCHANNELS) {
    printke("Studio Play: too many channels for me");
    return -1;
  }
  for (i = 0; i < c; i++) {
    if (krad_ringbuffer_write_space(p->audio.samples[i]) < (a->count * sizeof(float))) {
      printke("Studio Play: not enough space in ringbuffer!");
      // making room
      krad_ringbuffer_read_advance(p->audio.samples[i], a->count * sizeof(float));
    }
    krad_ringbuffer_write(p->audio.samples[i], (char *)a->samples[i],
     a->count * sizeof(float));
  }
  return 0;
}

static int play_destroy_audio(kr_studio_path *path) {
  int i;
  int ret;
  studio_play *p;
  ret = -1;
  p = &path->play;
  switch (p->codec.audio) {
    case KR_AUDIO_CODEC_FLAC:
      ret = kr_flac_decoder_destroy(p->audio.flac);
      break;
    case KR_AUDIO_CODEC_OPUS:
      ret = kr_opus_decoder_destroy(p->audio.opus);
      break;
    case KR_AUDIO_CODEC_VORBIS:
      ret = kr_vorbis_decoder_destroy(p->audio.vorbis);
      break;
    case KR_AUDIO_CODEC_MP3:
      ret = kr_mp3_decoder_destroy(p->audio.mp3);
      break;
    case KR_AUDIO_CODEC_AAC:
      ret = kr_aac_decoder_destroy(p->audio.aac);
      break;
    default: break;
  }
  if (p->audio.mixer_port) {
    ret = kr_mixer_remove(p->audio.mixer_port);
    p->audio.mixer_port = NULL;
  }
  kr_resampler_destroy(p->audio.resampler);
  for (i = 0; i < KR_AUDIO_PLAY_NCHANNELS; i++) {
    krad_ringbuffer_free(p->audio.samples[i]);
  }
  return ret;
}

static int play_destroy_video(kr_studio_path *path) {
  return -1;
}

static int play_mixer_process(int nframes, void *user) {
  int i;
  ssize_t ret;
  studio_play *p;
  kr_audio audio;
  kr_studio_path *path;
  float samples[KR_AUDIO_PLAY_NCHANNELS][8192];
  path = (kr_studio_path *)user;
  p = &path->play;
  printkd("Play: mixer wants %d", nframes);
  memset(samples, 0, sizeof(samples));
  memset(&audio, 0, sizeof(audio));
  if (nframes > (sizeof(samples) / (KR_AUDIO_PLAY_NCHANNELS * sizeof(float)))) {
    printke("Studio Play: Not enough data in buffer");
    return -1;
  }
  audio.rate = 48000;
  audio.count = nframes;
  audio.channels = KR_AUDIO_PLAY_NCHANNELS;
  for (i = 0; i < audio.channels; i++) {
    audio.samples[i] = samples[i];
  }
  ret = play_fetch_audio(p, &audio);
  printkd("Play: I fetched %lld samples", ret/sizeof(float));
  kr_mixer_write(p->audio.mixer_port, &audio);
  return ret;
}

static int play_setup_audio(kr_studio_path *path) {
  int i;
  int ret;
  studio_play *p;
  union {
    kr_vorbis_decoder_setup vorbis;
    kr_opus_decoder_setup opus;
    kr_flac_decoder_setup flac;
    kr_mp3_decoder_setup mp3;
    kr_aac_decoder_setup aac;
  } setup;
  kr_mixer_port_setup mp_setup;
  kr_resampler_setup rsetup;
  ret = -1;
  p = &path->play;
  for (i = 0; i < KR_AUDIO_PLAY_NCHANNELS; i++) {
    // ~5 seconds (at 48000Hz) worth of decoded audio
    p->audio.samples[i] = krad_ringbuffer_create(48000 * 5 * sizeof(float));
    if (!p->audio.samples[i]) {
      printke("Studio Play: could not create ringbuffers");
      return -1;
    }
  }
  kr_mixer_port_setup_init(&mp_setup);
  mp_setup.audio_user = path;
  mp_setup.control_user = path->user;
  mp_setup.process = play_mixer_process;
  mp_setup.info.type = KR_MXR_SOURCE;
  mp_setup.info.channels = KR_AUDIO_PLAY_NCHANNELS;
  p->audio.mixer_port = kr_mixer_port(path->studio->mixer, &mp_setup);
  if (!p->audio.mixer_port) {
    printke("Studio Play: could not create mixer port");
    return -1;
  }
  if (sizeof(p->mem2) < kr_resampler_sizeof()) {
    printke("Studio Play: kr_resampler too big for me");
    return -1;
  }
  memset(&rsetup, 0, sizeof(rsetup));
  rsetup.channels = KR_AUDIO_PLAY_NCHANNELS;
  rsetup.mem = p->mem2;
  p->audio.resampler = kr_resampler_init(&rsetup);
  if (!p->audio.resampler) {
    printke("Studio Play: kr_resampler_init failed");
    return -1;
  }
  memset(&setup, 0, sizeof(setup));
  switch (p->codec.audio) {
    case KR_AUDIO_CODEC_FLAC:
      if (sizeof(p->mem) < kr_flac_decoder_sizeof()) {
        printke("Studio Play: flac decoder too big for me! %zu vs %zu",
          sizeof(p->mem), kr_flac_decoder_sizeof());
        return -2;
      }
      setup.flac.mem = p->mem;
      p->audio.flac = kr_flac_decoder_init(&setup.flac);
      if (!p->audio.flac) {
        printke("Studio Play: flac decoder init failed");
        return -3;
      }
      ret = 0;
      break;
    case KR_AUDIO_CODEC_OPUS:
      if (sizeof(p->mem) < kr_opus_decoder_sizeof()) {
        printke("Studio Play: opus decoder too big for me! %zu vs %zu",
          sizeof(p->mem), kr_opus_decoder_sizeof());
        return -2;
      }
      setup.opus.mem = p->mem;
      setup.opus.info.sample_rate = 48000;
      p->audio.opus = kr_opus_decoder_init(&setup.opus);
      if (!p->audio.opus) {
        printke("Studio Play: opus decoder init failed");
        return -3;
      }
      ret = 0;
      break;
    case KR_AUDIO_CODEC_VORBIS: break;
    case KR_AUDIO_CODEC_MP3:
      if (sizeof(p->mem) < kr_mp3_decoder_sizeof()) {
        printke("Studio Play: mp3 decoder too big for me! %zu vs %zu",
          sizeof(p->mem), kr_mp3_decoder_sizeof());
        return -2;
      }
      setup.mp3.mem = p->mem;
      p->audio.mp3 = kr_mp3_decoder_init(&setup.mp3);
      if (!p->audio.mp3) {
        printke("Studio Play: mp3 decoder init failed");
        return -3;
      }
      ret = 0;
      break;
    case KR_AUDIO_CODEC_AAC:
      if (sizeof(p->mem) < kr_aac_decoder_sizeof()) {
        printke("Studio Play: aac decoder too big for me! %zu vs %zu",
          sizeof(p->mem), kr_aac_decoder_sizeof());
        return -2;
      }
      setup.aac.mem = p->mem;
      p->audio.aac = kr_aac_decoder_init(&setup.aac);
      if (!p->audio.aac) {
        printke("Studio Play: aac decoder init failed");
        return -3;
      }
      ret = 0;
    default:
      break;
  }
  return ret;
}

static int play_setup_video(kr_studio_path *path) {
  printk("Play Video not implemented yet");
  return -1;
}

static int play_audio_resample(studio_play *p, kr_audio *out, kr_audio *in) {
  int ret;
  if (in->rate == 48000) return 1;
  printkd("Studio Play: resampling %u -> 48000", in->rate);
  out->channels = in->channels;
  ret = kr_resampler_process(p->audio.resampler, out, in);
  return ret;
}

static int decode_process_audio(kr_studio_path *path) {
  int i;
  int ret;
  studio_play *p;
  kr_audio out;
  kr_audio resampled;
  kr_audio *raw;
  kr_coded_audio in;
  kr_track_block block;
  float samples[KR_AUDIO_PLAY_NCHANNELS][8192];
  float rsamples[KR_AUDIO_PLAY_NCHANNELS][8192];
  p = &path->play;
  memset(&out, 0, sizeof(out));
  memset(&resampled, 0, sizeof(resampled));
  memset(&in, 0, sizeof(in));
  memset(&block, 0, sizeof(block));
  ret = kr_track_read(p->rd, &block);
  if (ret == 0) return 1;
  if (ret != 1) return -1;
  in = block.coded_audio;
  for (i = 0; i < KR_AUDIO_PLAY_NCHANNELS; i++) {
    out.samples[i] = samples[i];
    resampled.samples[i] = rsamples[i];
  }
  resampled.count = sizeof(rsamples) / (sizeof(float) * KR_AUDIO_PLAY_NCHANNELS);
  raw = &out;
  switch (p->codec.audio) {
    case KR_AUDIO_CODEC_FLAC:
      if (in.len == 0) return 1;
      ret = kr_flac_decode(p->audio.flac, &out, &in);
      if (ret != 0) {
        printke("Studio Play: FLAC decoding failed %d", ret);
        break;
      }
      if (out.count == 0) return 1;
      ret = play_audio_resample(p, &resampled, &out);
      if (ret < 0) printke("Studio Play: resampling failed!");
      if (ret == 0) raw = &resampled;
      else ret = 0;
      break;
    case KR_AUDIO_CODEC_OPUS:
      in.channels = 2;
      ret = kr_opus_decode(p->audio.opus, &out, &in);
      if (ret != 0) {
        printke("Studio Play: OPUS decoding failed %d", ret);
        break;
      }
      if (out.count == 0) return 1;
      break;
    case KR_AUDIO_CODEC_VORBIS: break;
    case KR_AUDIO_CODEC_MP3:
      in.channels = 2;
      ret = kr_mp3_decode(p->audio.mp3, &out, &in);
      if (ret != 0) {
        printke("Studio Play: MP3 decoding failed %d", ret);
        break;
      }
      if (out.count == 0) return 1;
      ret = play_audio_resample(p, &resampled, &out);
      if (ret < 0) printke("Studio Play: resampling failed!");
      if (ret == 0) raw = &resampled;
      else ret = 0;
      break;
    case KR_AUDIO_CODEC_AAC:
      ret = kr_aac_decode(p->audio.aac, &out, &in);
      if (ret != 0) {
        printke("Studio Play: AAC decoding failed %d", ret);
        break;
      }
      if (out.count == 0) return 1;
      ret = play_audio_resample(p, &resampled, &out);
      if (ret < 0) printke("Studio Play: resampling failed!");
      if (ret == 0) raw = &resampled;
      else ret = 0;
      break;
    default: break;
  }
  if (ret == 0) {
    printkd("Studio Play: decoded %d samples", out.count);
    ret = play_store_audio(p, raw);
  }
  return ret;
}

static int decode_process_video(kr_studio_path *path) {
  return -1;
}

static int play_track_event(kr_track_event *e) {
  kr_studio_path *path;
  path = (kr_studio_path *)e->user;
  if (e->type == KR_TRACK_WRITE) {
    if (path->play.codec.type == KR_CODEC_AUDIO) {
      while (decode_process_audio(path) == 0);
    } else if (path->play.codec.type == KR_CODEC_VIDEO) {
      decode_process_video(path);
    }
  } else if (e->type == KR_TRACK_REMOVE) {
    printk("Studio Play: My input track was removed!");
    return kr_studio_remove(path);
  }
  return 0;
}

int studio_play_destroy(kr_studio_path *path) {
  int ret;
  studio_play *p;
  if (!path) return -1;
  ret = -1;
  p = &path->play;
  if (p->codec.type == KR_CODEC_AUDIO) {
    ret = play_destroy_audio(path);
  } else if (p->codec.type == KR_CODEC_VIDEO) {
    ret = play_destroy_video(path);
  }
  kr_track_close(p->rd);
  return ret;
}

int studio_play_create(kr_studio_path *path) {
  int ret;
  studio_play *p;
  kr_play_info *info;
  kr_track_info track_info;
  kr_track_head_setup head_setup;
  if (!path) return -1;
  memset(&head_setup, 0, sizeof(head_setup));
  memset(&track_info, 0, sizeof(track_info));
  p = &path->play;
  info = &path->info.play;
  head_setup.user = path;
  p->in = kr_track_find_by_name(path->studio->tracker,
   info->input.name, info->input.len);
  if (!p->in) return -2;
  kr_track_info_get(p->in, &track_info);
  if (track_info.type != KR_TRACK_CODED_AUDIO
   && track_info.type != KR_TRACK_CODED_VIDEO) {
    printke("Studio Play: invalid input track type!");
    return -3;
  }
  p->codec = track_info.codec;
  if (!p->codec.type) {
    printke("Studio Play: invalid input track codec type");
    return -4;
  }
  if (track_info.type == KR_TRACK_CODED_AUDIO) {
    if (p->codec.type != KR_CODEC_AUDIO) return -5;
    ret = play_setup_audio(path);
    if (ret != 0) {
      printke("Studio Play: play_setup_audio failed %d", ret);
      return -6;
    }
  } else if (track_info.type == KR_TRACK_CODED_VIDEO) {
    if (p->codec.type != KR_CODEC_VIDEO) return -7;
    ret = play_setup_video(path);
    if (ret != 0) {
      printke("Studio Play: play_setup_video failed %d", ret);
      return -8;
    }
  }
  head_setup.type = KR_TRACK_READ_HEAD;
  head_setup.track_ev_cb = play_track_event;
  p->rd = kr_track_open(p->in, &head_setup);
  if (!p->rd) return -9;
  return ret;
}
