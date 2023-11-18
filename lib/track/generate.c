#include <krad/mem/mem.h>
#include <krad/image/image.h>
#include "generate.h"

#define KR_AUDIO_GEN_NCHANNELS KR_STEREO
#define KR_AUDIO_GEN_NFRAMES 1024

static int audio_process(int nframes, void *user) {
  int ret;
  int c;
  kr_studio_path *path;
  studio_generator *gen;
  kr_audio audio;
  float samples[KR_AUDIO_GEN_NCHANNELS][KR_AUDIO_GEN_NFRAMES];
  path = (kr_studio_path *)user;
  if (!path) return -1;
  gen = &path->generator;
  if (nframes > KR_AUDIO_GEN_NFRAMES) return -2;
  audio.count = nframes;
  audio.channels = KR_AUDIO_GEN_NCHANNELS;
  audio.rate = KR_MXR_SRATE_DEF;
  for (c = 0; c < KR_AUDIO_GEN_NCHANNELS; c++) {
    audio.samples[c] = samples[c];
  }
  ret = kr_noise_fill(gen->audio.noise, &audio);
  if (ret) return -3;
  kr_mixer_write(gen->audio.source_port, &audio);
  return nframes;
}

static int audio_gen_destroy(kr_studio_path *path) {
  studio_generator *gen;
  int ret;
  printk("Audio Gen: destroy");
  gen = &path->generator;
  ret = -1;
  if (gen->audio.source_port) {
    ret = kr_mixer_remove(gen->audio.source_port);
    gen->audio.source_port = NULL;
  }
  return ret;
}

static int audio_gen_setup(kr_studio_path *path) {
  int ret;
  kr_mixer *mixer;
  studio_generator *gen;
  kr_mixer_port_setup mp_setup;
  printk("Audio Gen: setup");
  ret = 0;
  gen = &path->generator;
  mixer = path->studio->mixer;
  if (kr_noise_sizeof() > sizeof(gen->audio.mem)) return -2;
  gen->audio.noise = (kr_noise *)gen->audio.mem;
  ret = kr_noise_init(gen->audio.noise);
  if (ret) return ret;
  kr_mixer_port_setup_init(&mp_setup);
  mp_setup.audio_user = path;
  mp_setup.control_user = path->user;
  mp_setup.process = audio_process;
  mp_setup.info.type = KR_MXR_SOURCE;
  mp_setup.info.channels = KR_AUDIO_GEN_NCHANNELS;
  gen->audio.source_port = kr_mixer_port(mixer, &mp_setup);
  if (!gen->audio.source_port) {
    printke("Audio Generator: create source mixer port returned NULL");
    ret = -1;
  }
  printk("Audio Gen: setup worked");
  return ret;
}

static void destroy_frames(kr_studio_path *path) {
  int i;
  studio_generator *gen;
  gen = &path->generator;
  for (i = 0; i < KR_VIDEO_GEN_FRAMES; i++) {
    if (gen->video.mem[i]) {
      free(gen->video.mem[i]);
      gen->video.mem[i] = NULL;
    }
  }
}

static int setup_frames(kr_studio_path *path) {
  int ret;
  int i;
  ssize_t sz;
  kr_image_info img_info;
  kr_image_setup img_setup;
  kr_image *img;
  kr_frame *frm;
  studio_generator *gen;
  kr_generate_video_info *info;
  gen = &path->generator;
  info = &path->info.generate.video;
  memset(&img_info, 0, sizeof(img_info));
  memset(&img_setup, 0, sizeof(img_setup));
  img_info.w = info->width;
  img_info.h = info->height;
  img_info.fmt = info->format;
  sz = kr_image_sizeof(&img_info);
  if (sz <= 0) return -1;
  for (i = 0; i < KR_VIDEO_GEN_FRAMES; i++) {
    frm = &gen->video.frame[i];
    memset(frm, 0, sizeof(*frm));
  }
  for (i = 0; i < KR_VIDEO_GEN_FRAMES; i++) {
    gen->video.mem[i] = kr_alloc(sz);
  }
  for (i = 0; i < KR_VIDEO_GEN_FRAMES; i++) {
    img = &gen->video.frame[i].image;
    img_setup.mem = gen->video.mem[i];
    img_setup.img = img;
    img_setup.info = img_info;
    ret = kr_image_init2(&img_setup);
    if (ret != 0) return -2;
  }
  for (i = 0; i < KR_VIDEO_GEN_FRAMES; i++) {
    img = &gen->video.frame[i].image;
    if (info->type == KR_GENERATE_COLOR) {
      ret = kr_image_fill(img, info->color.r, info->color.g, info->color.b);
      if (ret != 0) return -3;
    } else if (info->type == KR_GENERATE_NO_SIGNAL) {
      ret = kr_image_no_signal(img);
      if (ret != 0) return -4;
    }
  }
  return 0;
}

static int video_gen_destroy(kr_studio_path *path) {
  studio_generator *gen;
  int ret;
  ret = 0;
  gen = &path->generator;
  ret = kr_compositor_remove(gen->video.source_port);
  destroy_frames(path);
  return ret;
}

static int video_gen_setup(kr_studio_path *path) {
  int ret;
  kr_compositor *comp;
  studio_generator *gen;
  kr_compositor_port_setup cp_setup;
  ret = 0;
  gen = &path->generator;
  comp = path->studio->compositor;
  ret = setup_frames(path);
  if (ret) return ret;
  memset(&cp_setup, 0, sizeof(cp_setup));
  cp_setup.info.type = KR_COM_SOURCE;
  kr_compositor_source_info_init(&cp_setup.info.source);
  cp_setup.frame_user = path;
  cp_setup.control_user = path->user;
  gen->video.source_port = kr_compositor_port(comp, &cp_setup);
  if (!gen->video.source_port) {
    printke("Video Generator: create source compositor port returned NULL");
    ret = -1;
  }
  ret = kr_compositor_write(gen->video.source_port, &gen->video.frame[0]);
  if (ret == 1) return 0;
  return -1;
}

int studio_generator_destroy(kr_studio_path *path) {
  int ret;
  ret = -1;
  if (path->info.generate.type == KR_GENERATE_VIDEO) {
    ret = video_gen_destroy(path);
  } else if (path->info.generate.type == KR_GENERATE_AUDIO) {
    ret = audio_gen_destroy(path);
  }
  return ret;
}

int studio_generator_create(kr_studio_path *path) {
  int ret;
  if (path->info.generate.type == KR_GENERATE_VIDEO) {
    ret = video_gen_setup(path);
  } else {
    ret = audio_gen_setup(path);
  }
  return ret;
}
