#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <limits.h>
#include <krad/app/debug.h>
#include <krad/loop/loop.h>
#include <krad/mem/pool.h>
#include <krad/mem/mem.h>
#include "jack.h"
#include "error.h"

#define KR_JACK_CHANNELS_MAX 8

typedef struct {
  int ready;
  kr_adapter *adapter;
  kr_adapter_path *adapter_path;
  jack_port_t *ports[KR_JACK_CHANNELS_MAX];
} kr_jack_path;

typedef struct {
  jack_client_t *client;
  kr_pool *path_pool;
} kr_jack;

static void handle_info(const char *msg);
static void handle_error(const char *msg);
static void handle_shutdown(jack_status_t code, const char *reason, void *arg);
static void handle_thread_init(void *arg);
static int handle_buffer_size(jack_nframes_t nframes, void *arg);
static int handle_sample_rate(jack_nframes_t nframes, void *arg);
static int handle_xrun(void *arg);
static int path_process(kr_adapter_path *path);
static int jack_process(kr_adapter *adapter);
static void *process_thread(void *arg);
static int path_unregister(kr_adapter_path *path);
static int path_register(kr_adapter_path *path);
static void register_paths(kr_adapter *adapter);
static void client_close(kr_adapter *adapter);
static int client_connect(kr_adapter *adapter);
static int init_jack(kr_adapter *adapter);

static void handle_info(const char *msg) {
  printk("Jack: info: %s", msg);
}

static void handle_error(const char *msg) {
  printke("Jack: error: %s", msg);
}

static void handle_shutdown(jack_status_t code, const char *reason, void *arg) {
  kr_adapter *adapter;
  kr_jack_info *info;
  adapter = (kr_adapter *)arg;
  info = &adapter->info->jack;
  info->state = KR_JACK_OFFLINE;
  printke("Jack: shutdown reason: %s", reason);
}

static void handle_thread_init(void *arg) {
  printk("Jack: Thread init");
}

static int handle_buffer_size(jack_nframes_t nframes, void *arg) {
  kr_adapter *adapter;
  kr_jack_info *info;
  int prev_period_size;
  adapter = (kr_adapter *)arg;
  info = &adapter->info->jack;
  prev_period_size = info->period_size;
  info->period_size = nframes;
  printk("Jack: %s buffer size change %d -> %d", info->client_name,
   prev_period_size, info->period_size);
  return 0;
}

static int handle_sample_rate(jack_nframes_t nframes, void *arg) {
  kr_adapter *adapter;
  kr_jack_info *info;
  int prev_sample_rate;
  adapter = (kr_adapter *)arg;
  info = &adapter->info->jack;
  prev_sample_rate = info->sample_rate;
  info->sample_rate = nframes;
  printk("Jack: %s sample rate change %d -> %d", info->client_name,
   prev_sample_rate, info->sample_rate);
  return 0;
}

static int handle_xrun(void *arg) {
  kr_adapter *adapter;
  kr_jack_info *info;
  adapter = (kr_adapter *)arg;
  info = &adapter->info->jack;
  info->xruns++;
  printke("Jack: %s xrun number %d!", info->client_name, info->xruns);
  return 0;
}

static int path_process(kr_adapter_path *path) {
  int ret;
  int i;
  kr_audio audio;
  kr_adapter *adapter;
  kr_jack_path *jack_path;
  kr_jack_path_info *info;
  kr_jack_info *jack_info;
  jack_path = (kr_jack_path *)path->handle;
  adapter = jack_path->adapter;
  jack_info = &adapter->info->jack;
  if (path->info->type == KR_JACK_IN) {
    info = &path->info->jack_in;
  } else {
    info = &path->info->jack_out;
  }
  audio.channels = info->channels;
  audio.count = jack_info->period_size;
  audio.rate = jack_info->sample_rate;
  for (i = 0; i < audio.channels; i++) {
    audio.samples[i] = jack_port_get_buffer(jack_path->ports[i], audio.count);
  }
  if (path->info->type == KR_JACK_IN) {
    ret = path->write(path->user, (kr_av *)&audio);
  } else {
    ret = path->read(path->user, (kr_av *)&audio);
  }
  return ret;
}

static int jack_process(kr_adapter *adapter) {
  int i;
  kr_jack *jack;
  kr_jack_path *path;
  jack = (kr_jack *)adapter->handle;
  i = 0;
  while ((path = kr_pool_iterate_active(jack->path_pool, &i))) {
    if (path->ready == 1 && path->adapter_path->info->type == KR_JACK_IN) {
      path_process(path->adapter_path);
    }
  }
  i = 0;
  while ((path = kr_pool_iterate_active(jack->path_pool, &i))) {
    if (path->ready == 1 && path->adapter_path->info->type == KR_JACK_OUT) {
      path_process(path->adapter_path);
    }
  }
  return 0;
}

static void *process_thread(void *arg) {
  kr_adapter *adapter;
  kr_jack *jack;
  kr_jack_info *info;
  jack_nframes_t frames;
  adapter = (kr_adapter *)arg;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  for (;;) {
    if (info->state != KR_JACK_ONLINE) break;
    frames = jack_cycle_wait(jack->client);
    if (info->state != KR_JACK_ONLINE) break;
    if (frames != info->period_size) {
      printke("Jack: unexpected period size!");
      info->period_size = frames;
    } 
    jack_process(adapter);
    info->frames += frames;
    jack_cycle_signal(jack->client, 0);
  }
  jack_cycle_signal(jack->client, -1);
  return 0;
}

static int path_unregister(kr_adapter_path *path) {
  int i;
  int ret;
  kr_adapter *adapter;
  kr_jack *jack;
  kr_jack_path *jack_path;
  kr_jack_path_info *info;
  jack_path = (kr_jack_path *)path->handle;
  adapter = (kr_adapter *)jack_path->adapter;
  jack = (kr_jack *)adapter->handle;
  if (path->info->type == KR_JACK_IN) {
    info = &path->info->jack_in;
  } else {
    info = &path->info->jack_out;
  }
  jack_path->ready = 0;
  for (i = 0; i < info->channels; i++) {
    if (jack_path->ports[i] == NULL) continue;
    ret = jack_port_unregister(jack->client, jack_path->ports[i]);
    if (ret != 0) {
      printke("Jack: could not unregister port");
    }
    jack_path->ports[i] = NULL;
  }
  return 0;
}

static int path_register(kr_adapter_path *path) {
  int i;
  int fail;
  int flags;
  char *suffix;
  char name[256];
  int len;
  kr_adapter *adapter;
  kr_jack *jack;
  kr_jack_path *jack_path;
  kr_jack_path_info *info;
  jack_path = (kr_jack_path *)path->handle;
  adapter = (kr_adapter *)jack_path->adapter;
  jack = (kr_jack *)adapter->handle;
  if (path->info->type == KR_JACK_IN) {
    info = &path->info->jack_in;
    flags = JackPortIsInput;
    suffix = "audio_in";
  } else {
    info = &path->info->jack_out;
    flags = JackPortIsOutput;
    suffix = "audio_out";
  }
  len = jack_client_name_size();
  if (sizeof(name) < len) len = sizeof(name);
  printk("Jack: path register %s (%d chan)", info->name, info->channels);
  fail = 0;
  for (i = 0; i < info->channels; i++) {
    snprintf(name, len, "%s/%s %d", info->name, suffix, i + 1);
    jack_path->ports[i] = jack_port_register(jack->client, name,
     JACK_DEFAULT_AUDIO_TYPE, flags, 0);
    if (jack_path->ports[i] == NULL) {
      printke("JACK: port register: %s", name);
      fail = 1;
      break;
    }
    printk("JACK: port registered: %s", name);
  }
  if (fail) {
    path_unregister(path);
    /* FIXME  mark path state as fail */
    return -1;
  }
  jack_path->ready = 1;
  return 0;
}

static void register_paths(kr_adapter *adapter) {
  int i;
  kr_jack *jack;
  kr_jack_path *path;
  jack = (kr_jack *)adapter->handle;
  i = 0;
  while ((path = kr_pool_iterate_active(jack->path_pool, &i))) {
    path_register(path->adapter_path);
  }
}

static void client_close(kr_adapter *adapter) {
  kr_jack *jack;
  kr_jack_info *info;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  if (jack->client != NULL) {
    jack_client_close(jack->client);
    jack->client = NULL;
    printk("Jack: client closed");
  }
  info->state = KR_JACK_OFFLINE;
}

static int client_connect(kr_adapter *adapter) {
  int ret;
  char *name;
  kr_jack *jack;
  kr_jack_info *info;
  jack_status_t status;
  jack_options_t options;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  options = JackNoStartServer;
  if ((info->server_name[0] != '\0')
   && (memchr(info->server_name + 1, '\0',
   sizeof(info->server_name) - 1) != NULL)) {
    options |= JackServerName;
  }
  jack->client = jack_client_open(info->client_name, options, &status,
   info->server_name);
  if (jack->client == NULL) {
    printke("Jack: jack_client_open() failed, status = 0x%2.0x", status);
    if (status & JackServerFailed) {
      printke("Jack: Unable to connect to JACK server");
    }
    return -1;
  }
  info->state = KR_JACK_ONLINE;
  printk("Jack: client opened");
  if (status & JackNameNotUnique) {
    name = jack_get_client_name(jack->client);
    strncpy(info->client_name, name, sizeof(info->client_name) - 1);
    printke("Jack: unique name `%s' assigned", info->client_name);
  }
  info->sample_rate = jack_get_sample_rate(jack->client);
  info->period_size = jack_get_buffer_size(jack->client);
  jack_on_info_shutdown(jack->client, handle_shutdown, adapter);
  jack_set_thread_init_callback(jack->client, handle_thread_init, adapter);
  jack_set_buffer_size_callback(jack->client, handle_buffer_size, adapter);
  jack_set_sample_rate_callback(jack->client, handle_sample_rate, adapter);
  jack_set_xrun_callback(jack->client, handle_xrun, adapter);
  jack_set_process_thread(jack->client, process_thread, adapter);
  ret = jack_activate(jack->client);
  if (ret != 0) {
    client_close(adapter);
    printke("Jack: activate");
    return -1;
  }
  printk("Jack: client activated");
  return 0;
}

static int init_jack(kr_adapter *adapter) {
  kr_jack *jack;
  jack = (kr_jack *)adapter->handle;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  printk("Jack: Adapter init");
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*jack);
  pool_setup.size = sizeof(kr_jack_path);
  pool_setup.slices = 64; /* FIXME */
  pool = kr_pool_create(&pool_setup);
  if (!pool) return -1;
  jack = kr_pool_overlay_get(pool);
  jack->path_pool = pool;
  adapter->handle = jack;
  return 0;
}

int kr_jack_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  printk("Jack: kr_jack_lctl called");
  return -1;
}

int kr_jack_unlink(kr_adapter_path *path) {
  int ret;
  kr_adapter *adapter;
  kr_jack *jack;
  kr_jack_info *info;
  kr_jack_path *jack_path;
  jack_path = (kr_jack_path *)path->handle;
  adapter = jack_path->adapter;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  printk("Jack: path unlink called");
  if (info->state == KR_JACK_ONLINE) {
    ret = path_unregister(path);
    if (ret) {
      printke("Jack: could not unregister jack path: %d", ret);
    }
  }
  ret = kr_pool_release(jack->path_pool, jack_path);
  if (ret) {
    printke("Jack: failed to release jack path from pool");
    return ret;
  }
  path->handle = NULL;
  printk("Jack: path unlink succeeded");
  return 0;
}

int kr_jack_link(kr_adapter *adapter, kr_adapter_path *path) {
  int ret;
  kr_jack *jack;
  kr_jack_info *info;
  kr_jack_path *jack_path;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  jack_path = kr_pool_slice(jack->path_pool);
  if (jack_path == NULL) return -3;
  path->handle = jack_path;
  jack_path->adapter = adapter; /* FIXME KLUDGE */
  jack_path->adapter_path = path;
  if (info->state != KR_JACK_ONLINE) {
    printke("Jack: not creating ports because jack server is offline");
    return -5;
  }
  ret = path_register(path);
  if (ret) {
    printke("Jack: error registering ports: %d", ret);
    return -7;
  }
  return 0;
}

int kr_jack_ctl(kr_adapter *adapter, kr_patchset *patchset) {
  printk("Jack: kr_jack_ctl called");
  return -1;
}

int kr_jack_close(kr_adapter *adapter) {
  kr_jack *jack;
  kr_jack_info *info;
  jack = (kr_jack *)adapter->handle;
  info = &adapter->info->jack;
  printk("Jack: close started");
  client_close(adapter);
  if (info->xruns == 0) {
    printk("Jack: had no xruns processing %"PRIu64" frames!", info->frames);
  } else {
    printke("Jack: had %u xruns by close :/", info->xruns);
  }
  kr_pool_destroy(jack->path_pool);
  adapter->handle = NULL;
  printk("Jack: close complete");
  return 0;
}

int kr_jack_open(kr_adapter *adapter) {
  int ret;
  int reopening;
  static int jack_log_setup = 0;
  if (jack_log_setup == 0) {
    jack_set_error_function(handle_error);
    jack_set_info_function(handle_info);
    jack_log_setup = 1;
  }
  if (adapter->handle) {
    reopening = 1;
    client_close(adapter);
    printk("Jack: reopening");
  } else {
    reopening = 0;
    ret = init_jack(adapter);
    if (ret != 0) {
      printke("Jack: failed to init");
      return ret;
    }
  }
  ret = client_connect(adapter);
  if (ret != 0) {
    fail (KR_ERR_JACK_SERVER_CONNECT);
  }
  if (reopening == 1) {
    register_paths(adapter);
  }
  printk("Jack: Adapter opened");
  return 0;
}
