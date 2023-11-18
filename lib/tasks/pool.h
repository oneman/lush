#if !defined(_tasks_pool_H)
#define _tasks_pool_H (1)

typedef struct kr_task_pool kr_task_pool;
typedef struct kr_task_setup kr_task_setup;

#include <krad/ops/media/media.h>
#include <krad/ops/media/pool.h>
#include <krad/tasks/task.h>
#include <krad/system/cmdr.h>

typedef struct {
  kr_cmdr *cmdr;
  int ntasks;
  kr_loop *loop;
  kr_task_event_cb *event_cb;
  kr_media_pool *media_pool;
} kr_task_pool_setup;

struct kr_task_setup {
  kr_task_params_info *info;
  void *user;
};

int kr_task_pool_open_program(kr_task_pool *pool, kr_program *program);
int kr_task_pool_run_event(kr_task_pool *pool, kr_task_event *event);
kr_loop *kr_task_pool_get_loop(kr_task_pool *pool);
int kr_task_pool_destroy_media(kr_task_pool *pool, kr_media *media);
kr_media *kr_task_pool_create_media(kr_task_pool *pool, kr_task *task);

kr_task *kr_task_find(kr_task_pool *tasks, uuid_t *id);
kr_task *kr_task_pool_iterate(kr_task_pool *tasks, int *iter);
int kr_task_pool_active(kr_task_pool *tasks);
int kr_task_destroy(kr_task_pool *tasks, kr_task *task);
kr_task *kr_task_create(kr_task_pool *pool, kr_task_setup *setup);
int kr_task_pool_destroy(kr_task_pool *tasks);
kr_task_pool *kr_task_pool_create(kr_task_pool_setup *setup);

#endif
