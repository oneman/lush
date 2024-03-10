typedef enum {
  NON_PORT = 0,
  PORT_CALLBACK,
  PORT_NOTIFY,
  PORT_DRIVER
} mixer_port_type;

typedef enum {
  KR_MXP_NIL,
  KR_MXP_READY,
  KR_MXP_ACTIVE,
  KR_MXP_REMOVE,
  KR_MXP_TERM
} mixer_path_state;

typedef struct {
  kr_mixer_path_info *info;
  void *control_user;
  void *audio_user;
  mixer_port_type port;
  kr_mixer_io_cb *process;
  kr_mixer_path *from;
  kr_mixer_path *to;
} mixer_path_setup;

struct kr_mixer_path {
  kr_mixer_path_type type;
  kr_mixer_channels channels;
  float *samples[KR_MXR_MAX_CHANNELS];
  int nframes;
  int sample_rate;
  uint16_t meter_rate;
  uint16_t meter_pos;
  int processed;
  int blended;
  int fused[KR_MXR_MAX_CHANNELS];
  mixer_path_state state;
  void *audio_user;
  void *control_user;
  mixer_port_type port;
  kr_mixer_io_cb *process;
  int fd;
  kr_mixer *mixer;
  kr_afx *afx;
  union {
    kr_graph_elem elem;
    void *ptr;
  } g;
  kr_spinlock lock;
  kr_mixer_path_info info;
};

static mixer_path_state path_state_get(kr_mixer_path *path);
static void path_state_set(kr_mixer_path *path, mixer_path_state state);
static void port_io(kr_mixer_path *path);
static kr_vertex_type path_vertex_type(kr_mixer_path_type type);
static void run_event_full(kr_mixer_path *path, kr_mixer_event_type type,
 kr_patchset *ps);
static void run_event(kr_mixer_path *path, kr_mixer_event_type type);
static void path_release(kr_mixer_path *path);
static int path_info_check(kr_mixer_path_info *info);
static int path_setup_check(mixer_path_setup *setup);
static void path_afx_create(kr_mixer_path *path);
static void path_setup(kr_mixer_path *path, mixer_path_setup *setup);
static kr_mixer_path *path_create(kr_mixer *mixer, mixer_path_setup *setup);

#include "samples.c"
#include "blend.c"
#include "process.c"

#define KR_STATE_MM __ATOMIC_SEQ_CST

static mixer_path_state path_state_get(kr_mixer_path *path) {
  return __atomic_load_n(&path->state, KR_STATE_MM);
}

static void path_state_set(kr_mixer_path *path, mixer_path_state state) {
  __atomic_store_n(&path->state, state, KR_STATE_MM);
}

static void port_io(kr_mixer_path *path) {
  switch (path->port) {
    case NON_PORT:
      printke("Mixer: non-port run???");
      return;
    case PORT_NOTIFY:
      //if (path->fd > -1) kr_give_notice(path->fd);
      return;
    case PORT_DRIVER:
      driver_port_run(path);
      return;
    case PORT_CALLBACK:
      return;
    default:
      printke("Mixer: port run major WTF");
      return;
  }
}

static kr_vertex_type path_vertex_type(kr_mixer_path_type type) {
  switch (type) {
    case KR_MXR_SOURCE:
      return KR_VERTEX_SOURCE;
    case KR_MXR_BUS:
      return KR_VERTEX_BUS;
    case KR_MXR_OUTPUT:
      return KR_VERTEX_OUTPUT;
    default:
      break;
  }
  return -1;
}

static void run_event_full(kr_mixer_path *path, kr_mixer_event_type type,
 kr_patchset *ps) {
  kr_mixer_event event;
  event.user = path->mixer->user;
  event.user_path = path->control_user;
  event.path = path;
  event.type = type;
  if (type == KR_MIXER_PATCH || type == KR_MIXER_METER) {
    event.patchset = ps;
  } else {
    if (type == KR_MIXER_CREATE) event.info = path->info;
    event.patchset = NULL;
  }
  path->mixer->event_cb(&event);
  path->control_user = event.user_path;
}

static void run_event(kr_mixer_path *path, kr_mixer_event_type type) {
  run_event_full(path, type, NULL);
}

static void path_release(kr_mixer_path *path) {
  int c;
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    free(path->samples[c]);
  }
  if (path->afx != NULL) {
    free(path->afx);
    path->afx = NULL;
  }
  kr_graph_remove(path->mixer->graph, &path->g.elem);
  if (path->fd > -1) {
    kr_notifier_destroy(path->fd);
    path->fd = 0;
  }
  path_state_set(path, KR_MXP_NIL);
  if (path->port == PORT_DRIVER) driver_port_del(path);
  kr_pool_atomic_release(path->mixer->path_pool, path);
}

static int path_info_check(kr_mixer_path_info *info) {
  if (info->channels < 1) return -1;
  if (info->channels > KR_MXR_MAX_CHANNELS) return -1;
  if ((info->type != KR_MXR_SOURCE)
   && (info->type != KR_MXR_INPUT)
   && (info->type != KR_MXR_BUS)
   && (info->type != KR_MXR_OUTPUT)) {
    return -4;
  }
  /* FIXME check all the things
   * afx params
   * */
  return 0;
}

static int path_setup_check(mixer_path_setup *setup) {
  if (setup->info->type == KR_MXR_INPUT || setup->info->type == KR_MXR_BUS) {
    if (setup->audio_user != NULL) return -2;
  } else {
    if (!setup->audio_user) return -4;
  }
  if (!setup->control_user) return -5;
  if (0) if (path_info_check(setup->info)) return -6;
  return 0;
}

static void path_afx_create(kr_mixer_path *path) {
  path->afx = kr_allocz(1, kr_afx_sizeof(path->channels));
  path->afx = kr_afx_init(path->afx, path->channels, path->sample_rate,
   path->info.afx);
}

static void path_setup(kr_mixer_path *path, mixer_path_setup *setup) {
  int c;
  kr_spin_init(&path->lock);
  path->sample_rate = 48000;
  path->channels = setup->info->channels;
  path->type = setup->info->type;
  path->blended = 0;
  path->audio_user = setup->audio_user;
  path->control_user = setup->control_user;
  path->process = setup->process;
  path->info = *setup->info;
  if (path->port == PORT_NOTIFY) {
    path->fd = kr_notifier_create();
  } else {
    path->fd = -1;
  }
  for (c = 0; c < KR_MXR_MAX_CHANNELS; c++) {
    path->samples[c] = kr_allocz(1, KR_MXR_PERIOD_MAX * sizeof(float));
    path->fused[c] = 0;
  }
  path->meter_rate = 800;
  path->meter_pos = 0;
  path->processed = 0;
  path_afx_create(path);
  if (path->port == PORT_DRIVER) driver_port_add(path);
  path_state_set(path, KR_MXP_READY);
  run_event(path, KR_MIXER_CREATE);
}

static kr_mixer_path *path_create(kr_mixer *mixer, mixer_path_setup *setup) {
  int ret;
  kr_mixer_path *path;
  kr_vertex_type vertex_type;
  if (!mixer || !setup) return NULL;
  ret = path_setup_check(setup);
  if (ret) {
    printke("mixer mkpath failed setup check: %d", ret);
    return NULL;
  }
  path = kr_pool_atomic_slice(mixer->path_pool);
  if (!path) {
    printke("mixer mkpath could not slice new path");
    return NULL;
  }
  path->mixer = mixer;
  if (setup->info->type == KR_MXR_INPUT) {
    path->g.elem = kr_graph_link(mixer->graph, &setup->to->g.elem,
     &setup->from->g.elem, path);
  } else {
    vertex_type = path_vertex_type(setup->info->type);
    path->g.elem = kr_graph_vertex(mixer->graph, vertex_type, path);
  }
  if (!path->g.elem.type) {
    kr_pool_atomic_release(mixer->path_pool, path);
    return NULL;
  }
  path->port = setup->port;
  path_setup(path, setup);
  return path;
}

int kr_mixer_read(kr_mixer_path *path, kr_audio *audio) {
  if (!path || !audio) return -1;
  if (path->type != KR_MXR_OUTPUT) return -1;
  path->channels = audio->channels;
  path->nframes = audio->count;
  path->sample_rate = audio->rate;
  //process_output(path);
  //copy_samples(audio->samples, path->samples, path->channels, path->nframes);
  audio->stc = path->mixer->info.nframes;
  //port_io(path);
  return 0;
}

int kr_mixer_write(kr_mixer_path *path, kr_audio *audio) {
  if (!path || !audio) return -1;
  if (path->type != KR_MXR_SOURCE) return -1;
  path->channels = audio->channels;
  path->nframes = audio->count;
  path->sample_rate = audio->rate;
  copy_samples(path->samples, audio->samples, path->channels, path->nframes);
  port_io(path);
  return 0;
}

int kr_mixer_path_info_get(kr_mixer_path *path, kr_mixer_path_info *info) {
  if (!path || !info) return -1;
  *info = path->info;
  return 0;
}

int kr_mixer_ctl(kr_mixer_path *path, kr_patchset *patchset) {
  if (!path || !patchset) return -1;
  kr_spin_lock(&path->lock);
  kr_patchset_apply(&path->info, patchset);
  kr_spin_unlock(&path->lock);
  patchset->on = &path->info;
  run_event_full(path, KR_MIXER_PATCH, patchset);
  return 0;
}

static void remove_path(kr_mixer_path *path) {
  kr_mixer *m;
  m = path->mixer;
  path_state_set(path, KR_MXP_REMOVE);
  kr_give_notice(m->remove_fd);
  while (kr_take_notice(m->removed_fd) != 1) usleep(100);
  if (path_state_get(path) != KR_MXP_TERM) {
    printke("Mixer: WTF");
  } else {
    printk("Mixer: path term");
  }
  run_event(path, KR_MIXER_DESTROY);
  path_release(path);
}

static void remove_inputs(kr_mixer_path *path) {
  int i;
  int n;
  kr_mixer_path *input;
  void *user[NSRCS];
  n = kr_graph_in_out_links(path->mixer->graph, &path->g.elem, 0, user, NSRCS);
  for (i = 0; i < n; i++) {
    input = (kr_mixer_path *)user[i];
    if (path_state_get(input) == KR_MXP_ACTIVE) remove_path(input);
  }
}

int kr_mixer_remove(kr_mixer_path *path) {
  if (!path) {
    printke("mixer path remove called with null value");
    return -1;
  }
  if (path_state_get(path) != KR_MXP_ACTIVE) {
    printke("mixer path remove called with inactive path");
    return -1;
  }
  if (path->type != KR_MXR_INPUT) remove_inputs(path);
  remove_path(path);
  return 0;
}

int kr_mixer_port_setup_init(kr_mixer_port_setup *setup) {
  if (!setup) return -1;
  setup->driver = 0;
  setup->control_user = NULL;
  setup->audio_user = NULL;
  setup->process = NULL;
  kr_mixer_path_info_init(&setup->info);
  return 0;
}

kr_mixer_path *kr_mixer_port(kr_mixer *mixer, kr_mixer_port_setup *setup) {
  mixer_path_setup path_setup;
  if (!mixer || !setup) return NULL;
  path_setup.info = &setup->info;
  path_setup.control_user = setup->control_user;
  path_setup.audio_user = setup->audio_user;
  path_setup.process = setup->process;
  if (path_setup.process) {
    path_setup.port = PORT_CALLBACK;
  } else {
    if (setup->driver == 1) {
      path_setup.port = PORT_DRIVER;
    } else {
      path_setup.port = PORT_NOTIFY;
    }
  }
  return path_create(mixer, &path_setup);
}

int kr_mixer_port_fd(kr_mixer_path *path) {
  if (path && path->port != NON_PORT) return path->fd;
  return -1;
}

int kr_mixer_bus(kr_mixer *mixer, kr_mixer_path_info *info, void *user) {
  kr_mixer_path *path;
  mixer_path_setup setup;
  if (!mixer || !info || !user) return -1;
  setup.info = info;
  setup.port = NON_PORT;
  setup.control_user = user;
  setup.audio_user = NULL;
  path = path_create(mixer, &setup);
  if (!path) return -2;
  return 0;
}

int kr_mixer_link(kr_mixer_path *output, kr_mixer_path *from,
 kr_mixer_path_info *info, void *user) {
  kr_mixer *mixer;
  kr_mixer_path *path;
  mixer_path_setup setup;
  if (!output || !from || !info || !user) return -1;
  mixer = output->mixer;
  setup.info = info;
  setup.port = NON_PORT;
  setup.control_user = user;
  setup.audio_user = NULL;
  setup.from = from;
  setup.to = output;
  path = path_create(mixer, &setup);
  if (!path) return -2;
  return 0;
}
