#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/timerfd.h>

#include <krad/mem/pool.h>
#include <krad/loop/loop.h>
#include <krad/system/priority.h>
#include <krad/tasks/task.h>
#include <krad/tasks/types.h>

#include <krad/tasks/valgrind_stressor/valgrind_stressor.h>
#include <krad/tasks/quvi/quvi.h>
#include <krad/tasks/avconv/avconv.h>
#include <krad/tasks/xmms/xmms.h>
#include <krad/tasks/curl/curl.h>
#include <krad/tasks/transmission_cli/transmission_cli.h>
#include <krad/tasks/livestreamer/livestreamer.h>
#include <krad/tasks/avprobe/avprobe.h>
#include <krad/tasks/ydl/ydl.h>

#include "pool.h"
#include "internal.h"
#include <krad/tasks/init.c>
#include <krad/tasks/task.c>
#include <krad/tasks/valgrind_stressor/valgrind_stressor.c>
#include <krad/tasks/quvi/quvi.c>
#include <krad/tasks/avconv/avconv.c>
#include <krad/tasks/xmms/xmms.c>
#include <krad/tasks/curl/curl.c>
#include <krad/tasks/transmission_cli/transmission_cli.c>
#include <krad/tasks/livestreamer/livestreamer.c>
#include <krad/tasks/avprobe/avprobe.c>
#include <krad/tasks/ydl/ydl.c>

struct kr_task_pool {
  kr_pool *pool;
  kr_loop *loop;
  kr_media_pool *media_pool;
  kr_task_event_cb *event_cb;
  kr_cmdr *cmdr;
  struct timespec next_timeout;
  kr_task *check_task;
  int tfd;
};

#include <krad/tasks/timeout.c>

int kr_task_pool_open_program(kr_task_pool *pool, kr_program *program) {
  int i;
  int ret;
  kr_program_setup setup;
  i = 0;
  setup.priority = program->priority;
  while (program->cmd[i]) {
    strcpy(setup.argv[i], program->cmd[i]);
    i++;
  }
  setup.argc = i;
  memcpy(setup.env, program->env, 32);
  ret = kr_program_open(pool->cmdr, program, &setup);
  return ret;
}

int kr_task_pool_destroy_media(kr_task_pool *pool, kr_media *media) {
  return kr_media_destroy(pool->media_pool, media);
}

kr_media *kr_task_pool_create_media(kr_task_pool *pool, kr_task *task) {
  kr_media_setup setup;
  setup.user = task;
  return kr_media_create(pool->media_pool, &setup);
}

kr_loop *kr_task_pool_get_loop(kr_task_pool *pool) {
  if (!pool) return NULL;
  return pool->loop;
}

int kr_task_pool_run_event(kr_task_pool *pool, kr_task_event *event) {
  if (!pool) return -1;
  return pool->event_cb(event);
}

kr_task *kr_task_find(kr_task_pool *tasks, uuid_t *id) {
  int i;
  int ret;
  kr_task *task;
  i = 0;
  if (kr_pool_active(tasks->pool) > 0) {
    while ((task = kr_pool_iterate_active(tasks->pool, &i))) {
      ret = uuid_compare(task->id, *id);
      if (ret == 0) return task;
    }
  }
  return NULL;
}

kr_task *kr_task_pool_iterate(kr_task_pool *tasks, int *iter) {
  kr_task *task;
  task = kr_pool_iterate_active(tasks->pool, iter);
  return task;
}

int kr_task_pool_active(kr_task_pool *tasks) {
  return kr_pool_active(tasks->pool);
}

int kr_task_destroy(kr_task_pool *tasks, kr_task *task) {
  int ret;
  ret = kr_task_clear(task);
  ret = kr_pool_release(tasks->pool, task);
  return ret;
}

kr_task *kr_task_create(kr_task_pool *tasks, kr_task_setup *setup) {
  int ret;
  kr_task *task;
  task = kr_pool_slice(tasks->pool);
  if (task == NULL) {
    printke("Ops: cannot create new task.");
    return NULL;
  }
  memset(task, 0, sizeof(*task));
  ret = kr_task_init(task, tasks, setup);
  if (ret == 0) return task;
  kr_pool_release(tasks->pool, task);
  return NULL;
}

int kr_task_pool_destroy(kr_task_pool *tasks) {
  if (!tasks) return -1;
  int i;
  kr_task *task;
  //disable_timeout(server);
  //close(tasks->tfd);
  i = 0;
  while ((task = kr_pool_iterate_active(tasks->pool, &i))) {
    if (task->info.status.state == KR_TASK_RUNNING) {
      kr_task_stop(task);
    }
  }
  i = 0;
  while ((task = kr_pool_iterate_active(tasks->pool, &i))) {
    if (task->info.status.state != 0) {
      kr_task_destroy(tasks, task);
    }
  }
  kr_pool_destroy(tasks->pool);
  return 0;
}

kr_task_pool *kr_task_pool_create(kr_task_pool_setup *setup) {
  kr_task_pool *tasks;
  kr_pool *pool;
  kr_pool_setup pool_setup;
  if (!setup) return NULL;
  pool_setup.shared = 0;
  pool_setup.overlay = NULL;
  pool_setup.overlay_sz = sizeof(*tasks);
  pool_setup.size = kr_task_sizeof();
  pool_setup.slices = setup->ntasks;
  pool = kr_pool_create(&pool_setup);
  tasks = kr_pool_overlay_get(pool);
  memset(tasks, 0, sizeof(*tasks));
  tasks->pool = pool;
  tasks->loop = setup->loop;
  tasks->cmdr = setup->cmdr;
  tasks->event_cb = setup->event_cb;
  tasks->media_pool = setup->media_pool;
  //init_timeout(tasks);
  return tasks;
}
