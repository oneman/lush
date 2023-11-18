#if !defined(_tasks_task_H)
#define _tasks_task_H (1)

#include <uuid/uuid.h>
#include "pool.h"

typedef struct kr_task kr_task;
typedef struct kr_task_event kr_task_event;
typedef struct kr_event_record kr_event_record;

struct kr_task_event {
  kr_task *task;
  kr_patchset patchset;
  kr_task_event_type type;
};

typedef int (kr_task_event_cb)(kr_task_event *);

size_t kr_task_sizeof();
kr_task_info *kr_task_get_info(kr_task *task);
int kr_task_init(kr_task *task, kr_task_pool *pool, kr_task_setup *setup);
int kr_task_start(kr_task *task);
int kr_task_stop(kr_task *task);
int kr_task_clear(kr_task *task);
/*int kr_task_ctl(kr_task *task);*/

#endif
