#include <krad/app/debug.h>
#include <krad/app/notice.h>
#include <krad/loop/loop.h>
#include <krad/time/interval.h>
#include <krad/mem/pool.h>
#include <krad/audio/audio.h>
#include <krad/audio/afx.h>
#include <krad/audio/hardlimiter.h>
#include <krad/graph/graph.h>
#include <krad/app/spinlock.h>
#include <krad/app/notice.h>
#include "mixer.h"
#include "error.h"

typedef struct {
  uint64_t nframes;
  int sample_rate;
} kr_mixer_info;

struct kr_mixer {
  kr_graph *graph;
  kr_pool *path_pool;
  kr_mixer_event_cb *event_cb;
  kr_loop *loop;
  void *user;
  kr_interval *clock;
  struct {
    int sources;
    int outputs;
    int ports;
    int ports_run;
    int clock;
  } driver;
  kr_mixer_info info;
  kr_spinlock clock_lock;
  int period_frames;
  int remove_fd;
  int removed_fd;
  kr_amem(mem, 128)
};

#include "path.c"

int kr_mixer_destroy(kr_mixer *mixer) {
  int i;
  kr_mixer_path *path;
  if (!mixer) return -1;
  printk("Mixer: Destroying");
  i = 0;
  while ((path = kr_pool_iterate_active(mixer->path_pool, &i))) {
    if (path->state != KR_MXP_NIL) {
      kr_mixer_remove(path);
    }
  }
  kr_graph_destroy(mixer->graph);
  kr_interval_destroy(mixer->clock);
  kr_notifier_destroy(mixer->remove_fd);
  kr_notifier_destroy(mixer->removed_fd);
  kr_loop_destroy(mixer->loop);
  printk("Mixer: Processed for %zu frames", mixer->info.nframes);
  kr_pool_destroy(mixer->path_pool);
  printk("Mixer: Destroyed");
  return 0;
}

kr_mixer *kr_mixer_create(kr_mixer_setup *setup) {
  kr_mixer *mixer;
  kr_pool *pool;
  kr_event ev;
  kr_interval_setup interval_setup;
  kr_loop_setup loop_setup;
  kr_pool_setup pool_setup;
  kr_graph_setup graph_setup;
  if (!setup) return NULL;
  if (setup->path_count < 2 || setup->path_count > 2048) return NULL;
  printk("Mixer: Creating");
  printk("Mixer: sizeof kr_mixer %zu", sizeof(kr_mixer));
  printk("Mixer: sizeof kr_mixer mem %zu", member_sizeof(kr_mixer, mem));
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*mixer);
  pool_setup.size = sizeof(kr_mixer_path);
  pool_setup.slices = setup->path_count;
  pool = kr_pool_create(&pool_setup);
  /*fail*/
  mixer = kr_pool_overlay_get(pool);
  memset(mixer, 0, sizeof(kr_mixer));
  mixer->info.sample_rate = KR_MXR_SRATE_DEF;
  mixer->info.nframes = 0;
  mixer->path_pool = pool;
  mixer->user = setup->user;
  kr_spin_init(&mixer->clock_lock);
  mixer->event_cb = setup->event_cb;
  memset(&graph_setup, 0, sizeof(graph_setup));
  graph_setup.use_locking = 1;
  mixer->graph = kr_graph_create(&graph_setup);
  /*fail*/
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_mixer");
  loop_setup.priority = KR_PRIORITY_REALTIME_IO;
  mixer->loop = kr_loop_create(&loop_setup);
  /*fail*/
  mixer->remove_fd = kr_notifier_create();
  /*fail*/
  mixer->removed_fd = kr_notifier_create();
  /*fail*/
  mixer->period_frames = KR_MXR_PERIOD_DEF;
  memset(&interval_setup, 0, sizeof(interval_setup));
  interval_setup.den = KR_MXR_PERIOD_DEF;
  interval_setup.num = KR_MXR_SRATE_DEF;
  if (sizeof(mixer->mem) < kr_interval_sizeof()) exit(1);
  interval_setup.mem = mixer->mem;
  mixer->clock = kr_interval_create(&interval_setup);
  /*fail*/
  ev.fd = kr_interval_get_fd(mixer->clock);
  ev.user = mixer;
  ev.handler = mixer_activate;
  ev.events = EPOLLIN;
  kr_loop_add(mixer->loop, &ev);
  /*fail*/
  ev.fd = mixer->remove_fd;
  ev.user = mixer;
  ev.handler = mixer_update;
  ev.events = EPOLLIN;
  kr_loop_add(mixer->loop, &ev);
  /*fail*/
  printk("Mixer: Created");
  return mixer;
}
