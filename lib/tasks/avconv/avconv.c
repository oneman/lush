#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/tasks/internal.h>
#include "avconv.h"

typedef struct {
  kr_program avconv;
  char *cmd[5];
  char avconv_cmd[128];
  char avconv_arg[128];
  char in[2048];
  char out[2048];
} avconv;

static int start_avconv(kr_task *task);
static int stop_avconv(kr_task *task);

static int avconv_event(kr_event *event) {
  /* temp kludge */
  char output[KR_TASK_OUTPUT_MAX_LEN];
  memset(output, 0, sizeof(output));
  /* end temp kludge */
  struct iovec iov[1];
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int err;
  kr_task *task;
  task = (kr_task *)event->user;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = output;
  iov[0].iov_len = KR_TASK_OUTPUT_MAX_LEN;
  if (event->events & EPOLLIN) {
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("avconv event: readv got error '%s'", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("avconv event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("avconv output: %d chars, '%s'", bytes_read, output);
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("avconv event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("avconv event: EPOLLHUP event on fd: %d", event->fd);
  }
  if (done == 1) {
    stop_avconv(task);
  }
  return 0;
}

static int start_avconv(kr_task *task) {
  avconv *a;
  a = (avconv *)task->data;
  a->avconv.handler = avconv_event;
  task_add_program(task, &a->avconv);
  return 0;
}

static int stop_avconv(kr_task *task) {
  avconv *a;
  a = (avconv *)task->data;
  task_del_program(task, &a->avconv);
  return 0;
}

int kr_avconv_create(kr_task *task) {
  avconv *a;
  a = kr_allocz(1, sizeof(*a));
  sprintf(a->avconv_cmd, "avconv");
  sprintf(a->avconv_arg, "-i");
  sprintf(a->in, "%s", task->info.avconv.input);
  sprintf(a->out, "%s", task->info.avconv.output);
  a->cmd[0] = a->avconv_cmd;
  a->cmd[1] = a->avconv_arg;
  a->cmd[2] = a->in;
  a->cmd[3] = a->out;
  a->cmd[4] = NULL;
  a->avconv.cmd = a->cmd;
  task->data = a;
  return 0;
}

int kr_avconv_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_avconv_start(kr_task *task) {
  start_avconv(task);
  return 0;
}

int kr_avconv_stop(kr_task *task) {
  avconv *a;
  a = (avconv *)task->data;
  if (program_running(&a->avconv)) {
    stop_avconv(task);
  }
  return 0;
}

int kr_avconv_destroy(kr_task *task) {
  avconv *a;
  kr_avconv_stop(task);
  a = (avconv *)task->data;
  free(a);
  return 0;
}
