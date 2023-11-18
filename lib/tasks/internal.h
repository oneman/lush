#if !defined(_tasks_internal_H)
#define _tasks_internal_H (1)

#include <stdio.h>
#include <uuid/uuid.h>

#include "task.h"

#include <krad/app/patch.h>
#include <krad/loop/loop.h>
#include <krad/system/priority.h>
#include <krad/tasks/types.h>
#include <krad/ops/media/media.h>
#include <krad/system/program.h>

#define KR_TASK_OUTPUT_MAX_LEN 8192

typedef struct {
  int (*create)(kr_task *);
  int (*ctl)(kr_task *, kr_patchset *);
  int (*start)(kr_task *);
  int (*stop)(kr_task *);
  int (*destroy)(kr_task *);
} kr_task_spec;

struct kr_event_record {
  kr_task_event_type event;
  struct timespec etime;
};

/*
  int task_progress(kr_task *task);
  int task_info_update(kr_task *task, kr_task_type_info *info);
  int task_fail(kr_task *task, kr_fail_info *fail);
  int task_success(kr_task *task);
*/

kr_loop *task_get_loop(kr_task *);
int task_del_program(kr_task *task, kr_program *program);
int task_add_program(kr_task *task, kr_program *program);
int task_raise_event(kr_task *task, kr_task_event_type event_type);

#endif
