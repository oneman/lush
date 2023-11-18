#include "internal.h"

int task_destroy_media(kr_task *task, kr_media *media) {
  return kr_task_pool_destroy_media(task->pool, media);
}

kr_media *task_create_media(kr_task *task) {
  return kr_task_pool_create_media(task->pool, task);
}

kr_loop *task_get_loop(kr_task *task) {
  return kr_task_pool_get_loop(task->pool);
}

int task_del_program(kr_task *task, kr_program *program) {
  int ret;
  kr_loop_del(task_get_loop(task), program->fd);
  ret = kr_program_close(program);
  return ret;
}

int task_add_program(kr_task *task, kr_program *program) {
  int ret;
  kr_event event;
  ret = kr_task_pool_open_program(task->pool, program);
  if (ret == 0) {
    memset(&event, 0, sizeof(event));
    event.fd = program->fd;
    event.handler = program->handler;
    event.user = task;
    event.events = EPOLLIN;
    ret = kr_loop_add(task_get_loop(task), &event);
    if (ret != 0) {
      kr_program_close(program);
      return -1;
    }
  }
  return ret;
}

int task_raise_event(kr_task *task, kr_task_event_type event_type) {
  if (!task_internal_events[event_type]) {
    printke("Task: tried to raise event it shouldn't");
    return -1;
  }
  return run_event(task, event_type);
}
