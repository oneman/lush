#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <krad/app/debug.h>
#include <krad/app/patch.h>
#include <krad/audio/audio.h>
#include <krad/mixer/mixer.h>
#include "../data.h"

#define NEVENTS 1
#define PORTS_MAX 16

typedef struct mix_test mix_test;

typedef struct {
  char name[64];
  kr_mixer_path *path;
  kr_mixer_path_info info;
  mix_test *task;
} mix_port;

struct mix_test {
  kr_mixer *mix;
  mix_port ports[PORTS_MAX];
  char name[64];
  int pd;
};

/*static mix_port *port_find_by_name(mix_test *task, char *name) {
  int i = 0;
  if (name == NULL) return NULL;
  for (i = 0; i < PORTS_MAX; i++) {
    if (!strcmp(task->ports[i].name, name)) {
      return &task->ports[i];
    }
  }
  return NULL;
}
*/

static mix_port *get_port(mix_test *task) {
  int i;
  for (i = 0; i < PORTS_MAX; i++) {
    if (task->ports[i].path == NULL) {
      task->ports[i].task = task;
      return &task->ports[i];
    }
  }
  return NULL;
}

static void mixer_event(kr_mixer_event *event) {
  int i;
  mix_test *task;
  task = (mix_test *)event->user;
  mix_port *port;
  if (task == NULL) {
    printke("MixerTest: event with null user pointer");
    return;
  }
  switch (event->type) {
    case KR_MIXER_CREATE:
      printk("%s: Mixer path create event", task->name);
      if (event->info.type == KR_MXR_INPUT) {
        port = get_port(task);
        port->path = event->path;
        event->user_path = port;
        printk("attached link");
      }
      break;
    case KR_MIXER_PATCH:
      printk("%s: Mixer path patch event", task->name);
      break;
    case KR_MIXER_DESTROY:
      printk("%s: Mixer path delete event", task->name);
      port = (mix_port *)event->user_path;
      if (event->info.type != KR_MXR_INPUT) {
      }
      for (i = 0; i < PORTS_MAX; i++) {
        if (&task->ports[i] == port) {
          printk("destroy found");
          task->ports[i].path = NULL;
          break;
        }
      }
      break;
    default:
      printke("%s: Bad event from mixer", task->name);
      break;
  }
}

static int mix_run(mix_test *task) {
  char *addr = "/eq/band/8/db";
  float value = 45.00f;
  int ret;
  int len;
  char string[2048];
  kr_patch patch;
  kr_mixer_path_info info;
  memset(&string, 0, sizeof(string));
  printk("size of string is %zu", sizeof(string));
  kr_path *path;
  kr_path_alloca(path);
  len = strlen(addr);
  ret = kr_path_parse(path, addr, len);
  printk("Path parse ret: %d", ret);
  kr_mixer_path_info_init(&info);
  info.channels = 2;
  info.type = KR_MXR_BUS;
  kr_mixer_path_info_to_text(string, &info, sizeof(string));
  printk("Mixer info is:\n %s", string);
  memset(&patch, 0 , sizeof(patch));
  kr_mixer_path_info_patch_path(&patch, path);
  memcpy(patch.value, &value, sizeof(value));
  kr_patch_apply(&info, &patch);
  kr_mixer_path_info_to_text(string, &info, sizeof(string));
  printk("Mixer info is:\n %s", string);
  return 0;
}

static int mix_teardown(mix_test *task) {
  int i;
  for (i = 0; i < PORTS_MAX; i++) {
    if (task->ports[i].path != NULL) {
      printk("Calling remove");
      kr_mixer_remove(task->ports[i].path);
    }
  }
  kr_mixer_destroy(task->mix);
  close(task->pd);
  free(task);
  task = NULL;
  return 0;
}

static int create_masterbus(mix_test *task) {
  int ret;
  kr_mixer_path_info info;
  memset(&info, 0, sizeof(info));
  info.type = KR_MXR_BUS;
  info.channels = 2;
  ret = kr_mixer_bus(task->mix, &info, task);
  if (ret != 0) printke("Error creating link");
  return ret;
}

static mix_test *mix_setup(char *name) {
  mix_test *task;
  kr_mixer_setup setup;
  int err;
  if (name == NULL) return NULL;
  task = calloc(1, sizeof(*task));
  task->pd = epoll_create1(EPOLL_CLOEXEC);
  if (task->pd == -1) {
  err = errno;
    fprintf(stderr, "Error: %s\n", strerror(err));
  }
  snprintf(task->name, sizeof(task->name), "%s", name);
  memset(&setup, 0 , sizeof(setup));
  setup.user = task;
  setup.path_count = 16;
  setup.event_cb = mixer_event;
  task->mix = kr_mixer_create(&setup);
  if (task->mix == NULL) return NULL;
  create_masterbus(task);
  return task;
}

static int test1() {
  int ret;
  mix_test *task;
  task = mix_setup("Mixer Test");
  if (task == NULL) return 1;
  ret = mix_run(task);
  if (ret != 0) {
    printke("Mixer run ret: %d\n", ret);
  }
  ret = mix_teardown(task);
  if (ret != 0) {
    printke("Teardown ret: %d\n", ret);
  }
  return ret;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test1();
  printk("Test1 result: %d\n", ret);
  return ret;
}

