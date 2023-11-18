#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/tasks/internal.h>
#include "quvi.h"

typedef struct {
  kr_program quvi;
  char *cmd[3];
  char quvi_cmd[128];
  char url[2048];
  char output[16384];
  int output_len;
  kr_media *media;
} quvi;

static int start_quvi(kr_task *task);
static int stop_quvi(kr_task *task);

static int quvi_init_media_items(kr_task *task, quvi *q) {
  int i;
  int res;
  kr_quvi_media_info *info;
  kr_media_item_info mi_info;
  info = &task->info.quvi.status.media_info;
  for (i = 0; i < info->versions; i++) {
    memset(&mi_info, 0, sizeof(mi_info));
    uuid_unparse_lower(task->id, mi_info.task);
    kr_timestamp_now(&mi_info.create_time, &task->create_time);
    mi_info.type = KR_MEDIA_URL;
    res = snprintf(mi_info.url.url, sizeof(mi_info.url.url),
     "%s", info->version[i].url);
    if (res >= sizeof(mi_info.url.url)) return -1;
    res = kr_media_add_item(q->media, &mi_info);
    if (res != 0) return -2;
  }
  return 0;
}

static int quvi_event(kr_event *event) {
  struct iovec iov[1];
  quvi *q;
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int err;
  kr_task *task;
  task = (kr_task *)event->user;
  q = task->data;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = q->output + q->output_len;
  iov[0].iov_len = sizeof(q->output) - q->output_len;
  if (event->events & EPOLLIN) {
    if (q->output_len >= sizeof(q->output)) {
      done = 1;
      printke("QUVI: no room left to read program output");
      task_raise_event(task, KR_TASK_FAILURE);
    } else {
      ret = readv(event->fd, iov, 1);
      if (ret == -1) {
        if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
          err = errno;
          printke("QUVI: readv got %s", strerror(err));
          done = 1;
        }
      } else if (ret == 0) {
        printke("QUVI: readv got 0 bytes");
      } else {
        printk("QUVI: readv got %d bytes '%s'", ret, q->output);
        q->output_len += ret;
        bytes_read += ret;
        task_raise_event(task, KR_TASK_PROGRESS);
      }
    }
  }
  if (event->events & EPOLLERR) {
    done = 1;
    printke("quvi event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
  }
  if (done == 1) {
    memset(&task->info.quvi, 0, sizeof(task->info.quvi));
    ret = kr_quvi_info_unpack_json(q->output, &task->info.quvi);
    if (ret == 0) {
      quvi_init_media_items(task, q);
      task_raise_event(task, KR_TASK_SUCCESS);
    } else {
      task_raise_event(task, KR_TASK_FAILURE);
      printke("QUVI: failed to unpack json from kr_quvi output");
    }
    stop_quvi(task);
  }
  return 0;
}

static int start_quvi(kr_task *task) {
  quvi *q;
  q = (quvi *)task->data;
  q->quvi.handler = quvi_event;
  return task_add_program(task, &q->quvi);
}

static int stop_quvi(kr_task *task) {
  quvi *q;
  q = (quvi *)task->data;
  return task_del_program(task, &q->quvi);
}

int kr_quvi_create(kr_task *task) {
  quvi *q;
  if (task == NULL) return -1;
  q = kr_allocz(1, sizeof(*q));
  strcpy(q->quvi_cmd, "kr_quvi");
  strcpy(q->url, task->info.params.quvi.url);
  q->cmd[0] = q->quvi_cmd;
  q->cmd[1] = q->url;
  q->cmd[2] = NULL;
  q->quvi.cmd = q->cmd;
  task->data = q;
  q->media = task_create_media(task);
  if(q->media == NULL) return -2;
  return 0;
}

int kr_quvi_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_quvi_start(kr_task *task) {
  start_quvi(task);
  return 0;
}

int kr_quvi_stop(kr_task *task) {
  quvi *q;
  q = (quvi *)task->data;
  if (program_running(&q->quvi)) {
    stop_quvi(task);
  }
  if (q->media) task_destroy_media(task, q->media);
  return 0;
}

int kr_quvi_destroy(kr_task *task) {
  quvi *q;
  kr_quvi_stop(task);
  q = (quvi *)task->data;
  free(q);
  return 0;
}
