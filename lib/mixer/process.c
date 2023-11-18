static void path_audio(kr_audio *audio, kr_mixer_path *path);
static void path_metering(kr_mixer_path *path, kr_audio *audio,
 kr_mixer_path_info *last);
static int process_source(kr_mixer_path *src, int nframes);
static int process_mix(kr_mixer_path *path);
static int process_output(kr_mixer_path *path);
static int process_sources(kr_mixer *m);
static int mixer_activate(kr_event *ev);

#define NSRCS 16
#define INDIR 1
#define OUTDIR 2

static void processed(kr_mixer_path *path) {
  if (path->processed != 0) {
    printke("Oh shit double processed!");
  }
  path->processed = 1;
}

static void path_audio(kr_audio *audio, kr_mixer_path *path) {
  int c;
  audio->stc = 0;
  audio->owner = path;
  audio->rate = path->sample_rate;
  audio->channels = path->channels;
  audio->count = path->nframes;
  for (c = 0; c < audio->channels; c++) {
    audio->samples[c] = path->samples[c];
  }
  find_zc(audio);
}

static void path_metering(kr_mixer_path *path, kr_audio *audio,
 kr_mixer_path_info *last) {
  int ret;
  kr_patchset ps;
  memset(&ps, 0, sizeof(ps));
  path->meter_pos += audio->count;
  if (path->meter_pos < path->meter_rate) return;
  path->meter_pos -= path->meter_rate;
  ret = kr_patchset_on(&ps, (void *)&path->info, last, KR_MIXER_PATH_INFO);
  if (ret) printk("patchset_on returns %d", ret);
  if (ps.len) run_event_full(path, KR_MIXER_METER, &ps);
}

static int process_inputs_from(kr_mixer_path *path, int nframes) {
  int i;
  int inputs;
  kr_mixer_path *input;
  void *user[NSRCS];
  kr_audio audio;
  kr_mixer_path_info last;
  mixer_path_state state;
  inputs = kr_graph_in_out_links(path->mixer->graph, &path->g.elem, OUTDIR, user, NSRCS);
  for (i = 0; i < inputs; i++) {
    input = (kr_mixer_path *)user[i];
    state = path_state_get(input);
    if (state != KR_MXP_ACTIVE && state != KR_MXP_REMOVE) continue;
    copy_samples(input->samples, path->samples, path->channels, path->nframes);
    if ((path->info.type == KR_MXR_SOURCE && path->info.channels == KR_MONO)
     && (input->info.channels == KR_STEREO)) {
      clone_channel(input->samples, path->samples, 1, 0, path->nframes);
      input->channels = KR_STEREO;
    } else {
      input->channels = path->channels;
    }
    input->nframes = path->nframes;
    path_audio(&audio, input);
    kr_spin_lock(&input->lock);
    last = input->info;
    kr_afx_process(input->afx, &audio, &audio, input->info.afx);
    kr_spin_unlock(&input->lock);
    path_metering(input, &audio, &last);
    processed(input);
  }
  return inputs;
}

static int process_source(kr_mixer_path *src, int nframes) {
  int inputs;
  kr_mixer_path_info last;
  kr_audio audio;
  if (src->process) src->process(nframes, src->audio_user);
  path_audio(&audio, src);
  kr_spin_lock(&src->lock);
  last = src->info;
  kr_afx_process(src->afx, &audio, &audio, src->info.afx);
  kr_spin_unlock(&src->lock);
  path_metering(src, &audio, &last);
  inputs = process_inputs_from(src, nframes);
  if (path_state_get(src) == KR_MXP_REMOVE && inputs == 0) path_state_set(src, KR_MXP_TERM);
  return inputs;
}

static int input_blended(kr_mixer_path *i) {
  if (path_state_get(i) == KR_MXP_ACTIVE && i->blended == i->channels) return 1;
  return 0;
}

static int sum_and_blend_inputs(kr_mixer_path *path) {
  int i;
  int inputs;
  kr_mixer_path *input;
  void *user[NSRCS];
  mixer_path_state state;
  inputs = kr_graph_in_out_links(path->mixer->graph, &path->g.elem, INDIR, user, NSRCS);
  for (i = 0; i < inputs; i++) {
    input = (kr_mixer_path *)user[i];
    if (input_blended(path)) {
      sum_channels(path->samples, input->samples, path->channels, path->nframes);
      continue;
    }
    state = path_state_get(input);
    if (state == KR_MXP_ACTIVE) {
      input->blended = blend_in(path, input);
    } else if (state == KR_MXP_REMOVE) {
      input->blended = blend_out(path, input);
      if (!input->blended) path_state_set(input, KR_MXP_TERM);
    }
  }
  return inputs;
}

static int process_mix(kr_mixer_path *path) {
  int mixed;
  kr_mixer_path_info last;
  kr_audio audio;
  mixed = 0;
  path_audio(&audio, path);
  kr_audio_zero(&audio);
  mixed += sum_and_blend_inputs(path);
  kr_spin_lock(&path->lock);
  last = path->info;
  kr_afx_process(path->afx, &audio, &audio, path->info.afx);
  kr_spin_unlock(&path->lock);
  path_metering(path, &audio, &last);
  if (path->type == KR_MXR_OUTPUT) {
    limit_samples(path->samples, path->channels, path->nframes);
  } else if (path->type == KR_MXR_BUS) {
    mixed += process_inputs_from(path, path->mixer->period_frames);
  }
  processed(path);
  if (path_state_get(path) == KR_MXP_REMOVE && mixed == 0) path_state_set(path, KR_MXP_TERM);
  return mixed;
}

static int bus_inputs_ready(kr_mixer_path *path) {
  int i;
  int n;
  int r;
  kr_mixer_path *input;
  void *user[NSRCS];
  r = 0;
  n = kr_graph_in_out_links(path->mixer->graph, &path->g.elem, INDIR, user, NSRCS);
  for (i = 0; i < n; i++) {
    input = (kr_mixer_path *)user[i];
    if (input->processed) r++;
  }
  if (r == n) return 1;
  return 0;
}

static int process_bus(kr_mixer_path *path) {
  int ready;
  ready = bus_inputs_ready(path);
  if (!ready) return 1;
  path->nframes = path->mixer->period_frames;
  process_mix(path);
  return 0;
}

static int process_output(kr_mixer_path *output) {
  process_mix(output);
  return 0;
}

static int process_sources(kr_mixer *m) {
  int i;
  kr_mixer_path *path;
  mixer_path_state state;
  i = 0;
  while ((path = kr_pool_iterate_active(m->path_pool, &i))) {
    if (path->info.type != KR_MXR_SOURCE) continue;
    state = path_state_get(path);
    if (state != KR_MXP_ACTIVE && state != KR_MXP_REMOVE) continue;
    process_source(path, m->period_frames);
  }
  return 0;
}

static int process_buses(kr_mixer *m) {
  int i;
  int p;
  kr_mixer_path *path;
  do {
    p = 0;
    i = 0;
    while ((path = kr_pool_iterate_active(m->path_pool, &i))) {
      if (path->info.type != KR_MXR_BUS) continue;
      if (!path->processed) {
        p += process_bus(path);
      }
    }
  } while (p);
  return 0;
}

static void run_callback_outputs(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    /* callbacking on capture ports which then call kr_mixer_read() */
    if (path->port == PORT_CALLBACK && path->info.type == KR_MXR_OUTPUT) {
      path->process(mixer->period_frames, path->audio_user);
    }
  }
}

static void run_notify_sources(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  i = 0;
  /* notify sources know about mem being reusable and timing */
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->port == PORT_NOTIFY && path->info.type == KR_MXR_SOURCE) {
      kr_give_notice(path->fd);
    }
  }
}

static void process_mark(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  mixer_path_state state;
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    state = path_state_get(path);
    if (state == KR_MXP_ACTIVE || state == KR_MXP_REMOVE) {
      path->processed = 0;
    }
  }
}

static void update_state(kr_mixer *mixer, mixer_path_state state) {
  int i;
  kr_mixer_path *path;
  mixer_path_state cur;
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    cur = path_state_get(path);
    if (state == KR_MXP_READY && cur == KR_MXP_READY) {
      path_state_set(path, KR_MXP_ACTIVE);
    } else if (state == KR_MXP_REMOVE && cur == KR_MXP_REMOVE) {
      path_state_set(path, KR_MXP_TERM);
    }
  }
}

static int mixer_update(kr_event *e) {
  kr_mixer *mixer;
  if (!e) return -1;
  mixer = (kr_mixer *)e->user;
  kr_take_notice(mixer->remove_fd);
  update_state(mixer, KR_MXP_REMOVE);
  kr_give_notice(mixer->removed_fd);
  return 0;
}

static void update_timecode(kr_mixer *m) {
  m->info.nframes += m->period_frames;
}

static int mixer_process(kr_mixer *m) {
  if (!m) return -1;
  update_timecode(m);
  update_state(m, KR_MXP_READY);
  process_mark(m);
  process_sources(m);
  process_buses(m);
  run_callback_outputs(m);
  run_notify_sources(m);
  return 0;
}

typedef enum {
  KR_MXR_CLOCK_INTERNAL = 0,
  KR_MXR_CLOCK_EXTERNAL
} kr_mixer_clock_source;

static int mixer_activate(kr_event *ev) {
  static int count;
  kr_mixer *mixer;
  if (!ev) return -1;
  mixer = (kr_mixer *)ev->user;
  /*printk("Mixer: activate! %d", count);*/
  kr_spin_lock(&mixer->clock_lock);
  if (!mixer->driver.clock) mixer_process(mixer);
  kr_spin_unlock(&mixer->clock_lock);
  kr_interval_update(mixer->clock);
  count++;
  return 0;
}

static void mixer_clock_set(kr_mixer *m, int src) {
  kr_spin_lock(&m->clock_lock);
  m->driver.clock = src;
  if (m->driver.clock == KR_MXR_CLOCK_INTERNAL
    && KR_MXR_PERIOD_DEF != m->period_frames) {
    m->period_frames = KR_MXR_PERIOD_DEF;
    printk("Set mixer period size to: %d", m->period_frames);
  }
  kr_spin_unlock(&m->clock_lock);
}

static void driver_port_run(kr_mixer_path *p) {
  kr_mixer *m;
  if (!p) return;
  m = p->mixer;
  m->driver.ports_run++;
  if (p->nframes != m->period_frames) {
    m->period_frames = p->nframes;
    printk("Set mixer period size to: %d", m->period_frames);
  }
  if (m->driver.ports_run == m->driver.ports) {
    mixer_process(m);
    m->driver.ports_run = 0;
  }
}

static void driver_port_del(kr_mixer_path *p) {
  kr_mixer *m;
  if (!p) return;
  m = p->mixer;
  if (p->type == KR_MXR_SOURCE) m->driver.sources--;
  if (p->type == KR_MXR_OUTPUT) m->driver.outputs--;
  m->driver.ports--;
  if (m->driver.ports == 0) mixer_clock_set(m, KR_MXR_CLOCK_INTERNAL);
}

static void driver_port_add(kr_mixer_path *p) {
  kr_mixer *m;
  if (!p) return;
  m = p->mixer;
  if (p->type == KR_MXR_SOURCE) m->driver.sources++;
  if (p->type == KR_MXR_OUTPUT) m->driver.outputs++;
  m->driver.ports++;
  if (m->driver.ports == 1) mixer_clock_set(m, KR_MXR_CLOCK_EXTERNAL);
}
