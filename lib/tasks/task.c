#include <krad/time/stamp.h>
#include "task.h"
#include "pool.h"
#include "internal.h"

struct kr_task {
  uuid_t id;
  void *data;
  void *user;
  kr_task_pool *pool;
  struct timespec create_time;
  struct timespec start_time;
  struct timespec stop_time;
  kr_event_record last;
  kr_task_info info;
};

static int run_event(kr_task *task, kr_task_event_type event_type);

#include "internal.c"

static void task_transition(kr_task *task, kr_task_event_type event_type) {
  kr_task_state last_state;
  kr_task_state state;
  last_state = task->info.status.state;
  state = transitions[event_type];
  task->info.status.state = state;
  printk("%s :: %s -> %s", kr_task_state_to_str(last_state),
   kr_task_event_type_to_str(event_type), kr_task_state_to_str(state));
  if (event_type == KR_TASK_CREATE) {
    kr_timestamp_now(&task->info.status.create_time, &task->create_time);
  }
  if (event_type == KR_TASK_START) {
    kr_timestamp_now(&task->info.status.start_time, &task->start_time);
  }
  if (stop_events[event_type]) {
    kr_timestamp_now(&task->info.status.stop_time, &task->stop_time);
  }
}

static int run_event(kr_task *task, kr_task_event_type event_type) {
  kr_task_event event;
  event.task = task;
  event.type = event_type;
  task_transition(task, event_type);
  return kr_task_pool_run_event(task->pool, &event);
}

size_t kr_task_sizeof() {
  return sizeof(kr_task);
}

kr_task_info *kr_task_get_info(kr_task *task) {
  if (!task) return NULL;
  return &task->info;
}

int kr_task_init(kr_task *task, kr_task_pool *pool, kr_task_setup *setup) {
  int ret;
  kr_task_params_info *info;
  if (!task || !setup) return -1;
  memset(task, 0, sizeof(*task));
  info = setup->info;
  task->user = setup->user;
  task->pool = pool;
  uuid_generate(task->id);
  uuid_unparse_lower(task->id, task->info.id);
  task->info.type = info->type;
  task->info.params = *info;
  ret = media_tasks[task->info.type].create(task);
  if (ret != 0) {
    printke("Task: failed to create");
    return ret;
  }
  ret = run_event(task, KR_TASK_CREATE);
  return 0;
}

int kr_task_start(kr_task *task) {
  int ret;
  if (!task) return -1;
  if (!startable_states[task->info.status.state]) {
    printke("Task: Attempted to start from a nonstartable state");
    return -1;
  }
  ret = media_tasks[task->info.type].start(task);
  if (ret != 0) {
    printke("Task: failed to start");
    return ret;
  }
  run_event(task, KR_TASK_START);
  return 0;
}

int kr_task_stop(kr_task *task) {
  int ret;
  if (!task) return -1;
  if (!stoppable_states[task->info.status.state]) {
    printke("Task: Attempted to stop from a nonstoppable state");
    return -1;
  }
  ret = media_tasks[task->info.type].stop(task);
  if (ret != 0) {
    printke("Task: failed to stop");
    return ret;
  }
  run_event(task, KR_TASK_CANCEL);
  return 0;
}

int kr_task_clear(kr_task *task) {
  int ret;
  if (!task) return -1;
  ret = media_tasks[task->info.type].destroy(task);
  if (ret != 0) {
    printke("Task: failed task-specific destroy");
  }
  run_event(task, KR_TASK_DESTROY);
  memset(task, 0, sizeof(*task));
  return 0;
}

/*
int kr_task_ctl(kr_task *task) {
  // FIXME add patch/diff argument and make use of it
  return 0;
}
*/
