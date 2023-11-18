#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/tasks/internal.h>
#include "livestreamer.h"

typedef struct {
  kr_program livestreamer;
  char *cmd[7];
  char livestreamer_cmd[16];
  char which[16];
  char force_option[16];
  char save_option[16];
  char save_file[2048];
  char url[2048];
} livestreamer;

static int start_livestreamer(kr_task *task);
static int stop_livestreamer(kr_task *task);

static int livestreamer_event(kr_event *event) {
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
        printke("livestreamer event: readv got error: '%s'", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("livestreamer event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("livestreamer output: %d chars, '%s'", bytes_read, output);
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("livestreamer event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("livestreamer event: EPOLLHUP event on fd: %d", event->fd);
  }
  if (done == 1) {
    stop_livestreamer(task);
  }
  return 0;
}

static int start_livestreamer(kr_task *task) {
  livestreamer *l;
  l = (livestreamer *)task->data;
  l->livestreamer.handler = livestreamer_event;
  task_add_program(task, &l->livestreamer);
  return 0;
}

static int stop_livestreamer(kr_task *task) {
  livestreamer *l;
  l = (livestreamer *)task->data;
  task_del_program(task, &l->livestreamer);
  return 0;
}

int kr_livestreamer_create(kr_task *task) {
  livestreamer *l;
  l = kr_allocz(1, sizeof(*l));
  strcpy(l->livestreamer_cmd, "livestreamer");
  strcpy(l->save_option, "-o");
  strcpy(l->which, "best");
  strcpy(l->force_option, "-f");
  strcpy(l->url, task->info.livestreamer.url);
  strcpy(l->save_file, task->info.livestreamer.save_file);
  l->cmd[0] = l->livestreamer_cmd;
  l->cmd[1] = l->force_option;
  l->cmd[2] = l->save_option;
  l->cmd[3] = l->save_file;
  l->cmd[4] = l->url;
  l->cmd[5] = l->which;
  l->cmd[6] = NULL;
  l->livestreamer.cmd = l->cmd;
  task->data = l;
  return 0;
}

int kr_livestreamer_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_livestreamer_start(kr_task *task) {
  start_livestreamer(task);
  return 0;
}

int kr_livestreamer_stop(kr_task *task) {
  livestreamer *l;
  l = (livestreamer *)task->data;
  if (program_running(&l->livestreamer)) {
    stop_livestreamer(task);
  }
  return 0;
}

int kr_livestreamer_destroy(kr_task *task) {
  livestreamer *l;
  kr_livestreamer_stop(task);
  l = (livestreamer *)task->data;
  free(l);
  return 0;
}
