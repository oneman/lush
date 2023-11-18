#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/tasks/internal.h>
#include "transmission_cli.h"

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
  int nfound;
  int done;
  int err;
  kr_task *task;
  task = (kr_task *)event->user;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = output;
  iov[0].iov_len = KR_TASK_OUTPUT_MAX_LEN;
  if (event->events & EPOLLIN && !(event->events & EPOLLHUP)) {
    printk("GOT EPOLLIN without EPOLLHUP");
  }
  if (event->events & EPOLLIN) {
    printk("POLLIN");
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("transmission_cli event: readv got %s", strerror(err));
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
    printk("%d chars, '%s'", bytes_read, output);
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
  transmission_cli *a;
  a = (transmission_cli *)task->data;
  a->transmission_cli.handler = transmission_cli_event;
  task_add_program(task, &a->transmission_cli);
  return 0;
}

static int stop_transmission_cli(kr_task *task) {
  transmission_cli *a;
  a = (transmission_cli *)task->data;
  task_del_program(task, &a->transmission_cli);
  return 0;
}

int kr_transmission_cli_create(kr_task *task) {
  transmission_cli *c;
  c = kr_allocz(1, sizeof(*c));
  sprintf(c->transmission_cli_cmd, "transmission_cli");
  sprintf(c->file, "%s", task->info.transmission_cli.file);
  c->cmd[0] = c->transmission_cli_cmd;
  c->cmd[1] = c->file;
  c->cmd[2] = NULL;
  c->transmission_cli.cmd = c->cmd;
  task->data = c;
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
  transmission_cli *a;
  a = (transmission_cli *)task->data;
  if (program_running(&a->transmission_cli)) {
    stop_transmission_cli(task);
  }
  return 0;
}

int kr_transmission_cli_destroy(kr_task *task) {
  transmission_cli *a;
  kr_transmission_cli_stop(task);
  a = (transmission_cli *)task->data;
  free(a);
  return 0;
}
