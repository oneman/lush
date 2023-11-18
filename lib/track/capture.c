#include <krad/app/name.h>
#define DEFAULT_CAPTURE_NBLOCKS 96
#define KR_AUDIO_CAP_NCHANNELS KR_STEREO
#define KR_AUDIO_CAP_NSAMPLES KR_MXR_PERIOD_DEF

static int capture_block_erase(void *user, void *block_user);
static int capture_track_event(kr_track_event *e);
static int write_track_frame(kr_track_head *head, kr_frame *frame);
static int write_track_audio(kr_track_head *head, kr_audio *audio, void *user);
static int audio_capture(int nframes, void *user);
static int frame_ready(kr_av_event *ev);
static int frame_captured(kr_event *e);
static int capture_frame(kr_studio_path *path);
static int image_ready(kr_av_event *ev);
static int image_captured(kr_event *e);
static int capture_image(kr_studio_path *path);
static int image_capture_destroy(kr_studio_path *path);
static int image_capture_create(kr_studio_path *path);
static int video_capture_destroy(kr_studio_path *path);
static int video_capture_create(kr_studio_path *path);
static int audio_capture_destroy(kr_studio_path *path);
static int audio_capture_create(kr_studio_path *path);

static int capture_block_erase(void *user, void *block_user) {
  int ret;
  kr_studio_path *path;
  path = (kr_studio_path *)user;
  ret = kr_pool_release(path->capture.raw_pool, block_user);
  if (ret != 0) {
    printke("Capture pool release ret %d", ret);
    return ret;
  }
  return 0;
}

static int capture_track_event(kr_track_event *e) {
  if (e->type == KR_TRACK_READ) {
    /*printk("Studio: Capture track read event");*/
  }
  if (e->type == KR_TRACK_WRITE) {
    /*printk("Studio: Capture track write event");*/
  }
  return 0;
}

static int write_track_frame(kr_track_head *head, kr_frame *frame) {
  kr_track_block block;
  memset(&block, 0, sizeof(block));
  block.user = frame->image.px[0];
  block.frame = *frame;
  /*kr_frame_debug_time(frame, "STUDIO CAPTURE", 14);*/
  return kr_track_write(head, &block);
}

static int write_track_audio(kr_track_head *head, kr_audio *audio, void *user) {
  kr_track_block block;
  memset(&block, 0, sizeof(block));
  block.user = user;
  block.audio = *audio;
  return kr_track_write(head, &block);
}

static int audio_capture(int nframes, void *user) {
  if (!user || nframes < 1) return -1;
  int ret;
  int c;
  kr_studio_path *path;
  studio_capture *cap;
  kr_audio audio;
  float *samples;
  path = (kr_studio_path *)user;
  if (!path) return -1;
  cap = &path->capture;
  if (nframes > KR_AUDIO_CAP_NSAMPLES) return -2;
  audio.count = nframes;
  audio.channels = KR_AUDIO_CAP_NCHANNELS;
  audio.rate = KR_MXR_SRATE_DEF;
  if (!cap->raw_pool) return -3;
  samples = (float *)kr_pool_slice(cap->raw_pool);
  if (!samples) {
    printke("Capture audio: out of mem shit!");
    return -4;
  }
  for (c = 0; c < KR_AUDIO_CAP_NCHANNELS; c++) {
    audio.samples[c] = samples + (c * audio.count);
  }
  ret = kr_mixer_read(cap->mixer_output, &audio);
  if (ret != 0) {
    printke("Capture audio: mixer read failed %d", ret);
    kr_pool_release(cap->raw_pool, samples);
    return -5;
  }
  write_track_audio(cap->head, &audio, samples);
  path->info.capture.audio.total_frames += audio.count;
  return nframes;
}

static int frame_ready(kr_av_event *ev) {
  int ret;
  kr_studio_path *path;
  studio_capture *cap;
  if (!ev) return -1;
  path = (kr_studio_path *)ev->user;
  cap = &path->capture;
  /*printk("Capture: frame ready");*/
  ret = write_track_frame(cap->head, ev->frame);
  path->info.capture.video.total_frames += 1;
  return ret;
}

static int frame_captured(kr_event *e) {
  kr_studio_path *path;
  studio_capture *cap;
  kr_frame frame;
  int ret;
  path = (kr_studio_path *)e->user;
  cap = &path->capture;
  memset(&frame, 0, sizeof(frame));
  /*printk("Frame capture event!");*/
  ret = kr_compositor_read(cap->comp_output, &frame);
  if (ret != 1) {
    printke("Capture: frame not readable but we were told there would be..");
  }
  capture_frame(path);
  return 0;
}

static int capture_frame(kr_studio_path *path) {
  studio_capture *cap;
  kr_frame frame;
  kr_image_setup image_setup;
  uint8_t *mem;
  int ret;
  cap = &path->capture;
  memset(&frame, 0, sizeof(frame));
  mem = kr_pool_slice(path->capture.raw_pool);
  if (!mem) {
    printke("Capture: ran out of free framepool memory");
    return -1;
  }
  image_setup.info.fmt = path->info.capture.video.format;
  image_setup.info.w = path->info.capture.video.width;
  image_setup.info.h = path->info.capture.video.height;
  image_setup.mem = mem;
  image_setup.img = &frame.image;
  kr_image_init2(&image_setup);
  frame.image.owner = path;
  frame.image.ready = frame_ready;
  frame.type = KR_IMAGE_RAW;
  ret = kr_compositor_write(cap->comp_output, &frame);
  return ret;
}

static int image_ready(kr_av_event *ev) {
  kr_studio_path *path;
  kr_image_stash_item *item;
  kr_stash_image simage;
  if (!ev) return -1;
  path = (kr_studio_path *)ev->user;
  simage.type = KR_IMAGE_RAW;
  simage.raw = ev->frame->image;
  item = kr_image_stash_put(path->studio->stash, &simage);
  if (!item) {
    printke("Image Capture: Stashing failed!");
    return -1;
  }
  printk("Image Capture: Stashed successfully!");
  return 0;
}

static int image_captured(kr_event *e) {
  kr_studio_path *path;
  studio_capture *cap;
  kr_frame frame;
  int ret;
  path = (kr_studio_path *)e->user;
  cap = &path->capture;
  memset(&frame, 0, sizeof(frame));
  ret = kr_compositor_read(cap->comp_output, &frame);
  if (ret != 1) {
    printke("Capture: frame not readable but we were told there would be..");
  }
  return ret;
}

static int capture_image(kr_studio_path *path) {
  int ret;
  studio_capture *cap;
  kr_image_setup image_setup;
  kr_frame frame;
  cap = &path->capture;
  memset(&frame, 0, sizeof(frame));
  frame.image.owner = path;
  frame.image.ready = image_ready;
  frame.type = KR_IMAGE_RAW;
  frame.image.info.w = path->info.capture.image.width;
  frame.image.info.h = path->info.capture.image.height;
  frame.image.info.fmt = path->info.capture.image.format;
  image_setup.info = frame.image.info;
  image_setup.img = &frame.image;
  image_setup.mem = kr_alloc(kr_image_sizeof(&frame.image.info));
  if (!image_setup.mem) {
    printke("Capture Image: image mem alloc");
    return -1;
  }
  ret = kr_image_init2(&image_setup);
  if (ret) {
    printke("Capture Image: image init failed");
    free(image_setup.mem);
    return -1;
  }
  ret = kr_compositor_write(cap->comp_output, &frame);
  if (ret != 1) {
    printke("Capture: frame not writable but why.....");
    return -1;
  }
  return 0;
}

static int image_capture_destroy(kr_studio_path *path) {
  int ret;
  studio_capture *cap;
  ret = 0;
  cap = &path->capture;
  printk("Capture Image: Destroy");
  /* FIXME port fd loop remove ? */
  if (cap->comp_output) ret += kr_compositor_remove(cap->comp_output);
  cap->comp_output = NULL;
  return ret;
}

static int image_capture_create(kr_studio_path *path) {
  int ret;
  int fail;
  studio_capture *cap;
  kr_event event;
  kr_compositor_port_setup cp_setup;
  printk("Capture Image: Creating");
  fail = 0;
  memset(&event, 0, sizeof(event));
  memset(&cp_setup, 0, sizeof(cp_setup));
  cap = &path->capture;
  cp_setup.info.type = KR_COM_OUTPUT;
  kr_compositor_output_info_init(&cp_setup.info.output);
  cp_setup.frame_user = path;
  cp_setup.control_user = path->user;
  cap->comp_output = kr_compositor_port(path->studio->compositor, &cp_setup);
  if (cap->comp_output == NULL) {
    printke("Capture Image: Creating compositor port returned NULL");
    fail = 1;
  }
  if (!fail) {
    event.fd = kr_compositor_port_fd(cap->comp_output);
    event.user = path;
    event.handler = image_captured;
    event.events = EPOLLIN;
    ret = kr_tracker_loop_add(path->studio->tracker, &event);
    if (ret) {
      fail = 1;
      printke("Capture Image: Could not add comp port fd to tracker loop");
    }
  }
  if (!fail) {
    printk("Capture Image: Created");
    ret = capture_image(path);
    if (ret) {
      fail = 1;
      printke("Capture Image: failed to try to capture");
    }
  }
  if (!fail) return 0;
  return -1;
}

static int video_capture_destroy(kr_studio_path *path) {
  int ret;
  studio_capture *cap;
  ret = 0;
  cap = &path->capture;
  printk("Capture Video: Destroy");
  /* FIXME port fd loop remove ? */
  if (cap->comp_output) ret += kr_compositor_remove(cap->comp_output);
  if (cap->head) ret += kr_track_close(cap->head);
  if (cap->track) ret += kr_track_remove(cap->track);
  if (cap->raw_pool) ret += kr_pool_destroy(cap->raw_pool);
  cap->comp_output = NULL;
  cap->head = NULL;
  cap->track = NULL;
  cap->raw_pool = NULL;
  return ret;
}

static int video_capture_create(kr_studio_path *path) {
  int fail;
  int ret;
  fail = 0;
  studio_capture *cap;
  kr_compositor_port_setup cp_setup;
  kr_image_info image_info;
  kr_pool_setup pool_setup;
  kr_track_info track_info;
  kr_track_setup track_setup;
  kr_track_head_setup head_setup;
  kr_event event;
  kr_name *name;
  memset(&cp_setup, 0, sizeof(cp_setup));
  memset(&image_info, 0, sizeof(image_info));
  memset(&pool_setup, 0, sizeof(pool_setup));
  memset(&track_info, 0, sizeof(track_info));
  memset(&track_setup, 0, sizeof(track_setup));
  memset(&head_setup, 0, sizeof(head_setup));
  memset(&event, 0, sizeof(event));
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = 0;
  cap = &path->capture;
  printk("Capture Video: Creating");
  cp_setup.info.type = KR_COM_OUTPUT;
  kr_compositor_output_info_init(&cp_setup.info.output);
  cp_setup.frame_user = path;
  cp_setup.control_user = path->user;
  cap->comp_output = kr_compositor_port(path->studio->compositor, &cp_setup);
  if (!cap->comp_output) {
    printke("Studio: Capture creating compositor port returned NULL");
    fail = 1;
  }
  if (!fail) {
    image_info.w = path->info.capture.video.width;
    image_info.h = path->info.capture.video.height;
    image_info.fmt = path->info.capture.video.format;
    pool_setup.size = kr_image_sizeof(&image_info);
    pool_setup.slices = DEFAULT_CAPTURE_NBLOCKS;
    cap->raw_pool = kr_pool_create(&pool_setup);
    if (!cap->raw_pool) {
      fail = 1;
      printke("Capture Video: Could not create track");
    }
    //if (!fail) kr_pool_debug(cap->raw_pool);
  }
  if (!fail) {
    track_info.type = KR_TRACK_VIDEO;
    track_info.mode = KR_TRACK_INFINITE;
    track_info.duration = DEFAULT_CAPTURE_NBLOCKS;
    track_info.video.width = path->info.capture.video.width;
    track_info.video.height = path->info.capture.video.height;
    track_info.video.fps_num = path->info.capture.video.fps_num;
    track_info.video.fps_den = path->info.capture.video.fps_den;
    track_info.video.pixel_fmt = path->info.capture.video.format;
    name = (kr_name *)path->user;
    memcpy(track_info.track.name, name->buf, name->len);
    track_info.track.len = name->len;
    track_setup.user = path;
    track_setup.info = &track_info;
    cap->track = kr_track_create(path->studio->tracker, &track_setup);
    if (!cap->track) {
      fail = 1;
      printke("Capture Video: Could not create track");
    }
  }
  if (!fail) {
    path->info.capture.video.track.len = kr_track_name_get(cap->track,
     path->info.capture.video.track.name,
     sizeof(path->info.capture.video.track.name));
  }
  if (!fail) {
    head_setup.user = path;
    head_setup.type = KR_TRACK_WRITE_HEAD;
    head_setup.track_ev_cb = capture_track_event;
    head_setup.erase_cb = capture_block_erase;
    cap->head = kr_track_open(cap->track, &head_setup);
    if (!cap->head) {
      fail = 1;
      printke("Capture Video: Could not create track write head");
    }
  }
  if (!fail) {
    event.fd = kr_compositor_port_fd(cap->comp_output);
    event.user = path;
    event.handler = frame_captured;
    event.events = EPOLLIN;
    ret = kr_tracker_loop_add(path->studio->tracker, &event);
    if (ret) {
      fail = 1;
      printke("Capture Video: Could not add comp port fd to tracker loop");
    }
  }
  if (!fail) {
    printk("Capture Video: Created");
    capture_frame(path);
    return 0;
  }
  return -1;
}

static int audio_capture_destroy(kr_studio_path *path) {
  int ret;
  studio_capture *cap;
  ret = 0;
  cap = &path->capture;
  printk("Capture Audio: Destroy");
  /* FIXME fd loop remove ? */
  if (cap->mixer_output) ret += kr_mixer_remove(cap->mixer_output);
  if (cap->head) ret += kr_track_close(cap->head);
  if (cap->track) ret += kr_track_remove(cap->track);
  if (cap->raw_pool) ret += kr_pool_destroy(cap->raw_pool);
  cap->mixer_output = NULL;
  cap->head = NULL;
  cap->track = NULL;
  cap->raw_pool = NULL;
  return ret;
}

static int audio_capture_create(kr_studio_path *path) {
  int fail;
  fail = 0;
  studio_capture *cap;
  kr_mixer_port_setup mp_setup;
  kr_image_info image_info;
  kr_pool_setup pool_setup;
  kr_track_info track_info;
  kr_track_setup track_setup;
  kr_track_head_setup head_setup;
  kr_event event;
  kr_name *name;
  memset(&image_info, 0, sizeof(image_info));
  memset(&pool_setup, 0, sizeof(pool_setup));
  memset(&track_info, 0, sizeof(track_info));
  memset(&track_setup, 0, sizeof(track_setup));
  memset(&head_setup, 0, sizeof(head_setup));
  memset(&event, 0, sizeof(event));
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = 0;
  cap = &path->capture;
  printk("Capture Audio: Creating");
  if (!fail) {
    pool_setup.size = KR_AUDIO_CAP_NSAMPLES * KR_AUDIO_CAP_NCHANNELS
     * sizeof(float);
    pool_setup.slices = DEFAULT_CAPTURE_NBLOCKS;
    cap->raw_pool = kr_pool_create(&pool_setup);
    if (!cap->raw_pool) {
      fail = 1;
      printke("Capture Audio: Could not create track");
    }
    //if (!fail) kr_pool_debug(cap->raw_pool);
  }
  if (!fail) {
    track_info.type = KR_TRACK_AUDIO;
    track_info.mode = KR_TRACK_INFINITE;
    track_info.duration = DEFAULT_CAPTURE_NBLOCKS;
    name = (kr_name *)path->user;
    memcpy(track_info.track.name, name->buf, name->len);
    track_info.track.len = name->len;
    track_setup.user = path;
    track_setup.info = &track_info;
    cap->track = kr_track_create(path->studio->tracker, &track_setup);
    if (!cap->track) {
      fail = 1;
      printke("Capture Audio: Could not create track");
    }
  }
  if (!fail) {
    head_setup.user = path;
    head_setup.type = KR_TRACK_WRITE_HEAD;
    head_setup.track_ev_cb = capture_track_event;
    head_setup.erase_cb = capture_block_erase;
    cap->head = kr_track_open(cap->track, &head_setup);
    if (!cap->head) {
      fail = 1;
      printke("Capture Audio: Could not create track write head");
    }
  }
  if (!fail) {
    kr_mixer_port_setup_init(&mp_setup);
    mp_setup.info.type = KR_MXR_OUTPUT;
    mp_setup.audio_user = path;
    mp_setup.control_user = path->user;
    mp_setup.process = audio_capture;
    cap->mixer_output = kr_mixer_port(path->studio->mixer, &mp_setup);
    if (!cap->mixer_output) {
      fail = 1;
      printke("Capture Audio: Creating mixer port returned NULL");
    }
  }
  if (!fail) {
    printk("Capture Audio: Created");
    return 0;
  }
  return -1;
}

int studio_capture_destroy(kr_studio_path *path) {
  int ret;
  ret = -1;
  switch (path->info.capture.type) {
  case KR_CAPTURE_IMAGE:
    ret = image_capture_destroy(path);
    break;
  case KR_CAPTURE_VIDEO:
    ret = video_capture_destroy(path);
    break;
  case KR_CAPTURE_AUDIO:
    ret = audio_capture_destroy(path);
    break;
  }
  return ret;
}

int studio_capture_create(kr_studio_path *path) {
  int ret;
  ret = -1;
  switch (path->info.capture.type) {
  case KR_CAPTURE_IMAGE:
    ret = image_capture_create(path);
    break;
  case KR_CAPTURE_VIDEO:
    ret = video_capture_create(path);
    break;
  case KR_CAPTURE_AUDIO:
    ret = audio_capture_create(path);
    break;
  }
  if (ret) studio_capture_destroy(path);
  return ret;
}
