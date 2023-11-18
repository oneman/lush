#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/app/debug.h>
#include <krad/loop/loop.h>
#include <krad/tasks/task.h>
#include <krad/tasks/transmission_cli/transmission_cli.h>

#include <krad/mem/mem.h>

typedef struct {
  kr_program transmission_cli;
  char *cmd[3];
  char transmission_cli_cmd[128];
  char file[2048];
} transmission_cli;

static int start_transmission_cli(kr_task *task);
static int stop_transmission_cli(kr_task *task);

static int transmission_cli_event(kr_event *event) {
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
        printke("transmission_cli event: readv got error '%s'", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("transmission_cli event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("transmission_cli output : %d chars, '%s'", bytes_read, output);
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("transmission_cli event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("transmission_cli event: EPOLLHUP event on fd: %d", event->fd);
  }
  if (done == 1) {
    stop_transmission_cli(task);
  }
  return 0;
}

static int start_transmission_cli(kr_task *task) {
  transmission_cli *t;
  t = (transmission_cli *)task->data;
  t->transmission_cli.handler = transmission_cli_event;
  task_add_program(task, &t->transmission_cli);
  task_raise_event(task, KR_TASK_PROGRESS);
  return 0;
}

static int stop_transmission_cli(kr_task *task) {
  transmission_cli *t;
  t = (transmission_cli *)task->data;
  task_del_program(task, &t->transmission_cli);
  return 0;
}

int kr_transmission_cli_create(kr_task *task) {
  transmission_cli *t;
  t = kr_allocz(1, sizeof(*t));
  sprintf(t->transmission_cli_cmd, "transmission-cli");
  sprintf(t->file, "%s", task->info.transmission_cli.file);
  t->cmd[0] = t->transmission_cli_cmd;
  t->cmd[1] = t->file;
  t->cmd[2] = NULL;
  t->transmission_cli.cmd = t->cmd;
  task->data = t;
  return 0;
}

int kr_transmission_cli_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_transmission_cli_start(kr_task *task) {
  start_transmission_cli(task);
  return 0;
}

int kr_transmission_cli_stop(kr_task *task) {
  transmission_cli *t;
  t = (transmission_cli *)task->data;
  if (program_running(&t->transmission_cli)) {
    stop_transmission_cli(task);
  }
  return 0;
}

int kr_transmission_cli_destroy(kr_task *task) {
  transmission_cli *t;
  kr_transmission_cli_stop(task);
  t = (transmission_cli *)task->data;
  free(t);
  return 0;
}
