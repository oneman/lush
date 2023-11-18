#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/tasks/internal.h>
#include "curl.h"

typedef struct {
  kr_program curl;
  char *cmd[8];
  char curl_cmd[128];
  char rflag1[16];
  char rflag2[16];
  char oflag1[16];
  char oflag2[2048];
  char url[2048];
} curl;

static int start_curl(kr_task *task);
static int stop_curl(kr_task *task);

static int curl_event(kr_event *event) {
  /* temp kludge */
  char output[KR_TASK_OUTPUT_MAX_LEN];
  memset(output, 0, sizeof(output));
  /* end temp kludge */
  struct iovec iov[1];
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int fail;
  int err;
  int val;
  char *endptr;
  kr_task *task;
  task = (kr_task *)event->user;
  done = 0;
  fail = 0;
  bytes_read = 0;
  iov[0].iov_base = output;
  iov[0].iov_len = KR_TASK_OUTPUT_MAX_LEN;
  if (event->events & EPOLLIN) {
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("curl event: readv got error '%s'", strerror(err));
        done = 1;
        fail = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("curl event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("Curl: %d chars, '%s'", bytes_read, output);
    val = strtol(output, &endptr, 10);
    if ((output != endptr) || (val != 0)) {
      printk("Curl: download %02d%% done", val);
      task->info.curl.percent_complete = val;
      task_raise_event(task, KR_TASK_PROGRESS);
    }
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    fail = 1;
    printke("Curl: event EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
  }
  if (done == 1) {
    if (fail) task_raise_event(task, KR_TASK_FAILURE);
    else task_raise_event(task, KR_TASK_SUCCESS);
    stop_curl(task);
  }
  return 0;
}

static int start_curl(kr_task *task) {
  curl *c;
  c = (curl *)task->data;
  c->curl.handler = curl_event;
  task_add_program(task, &c->curl);
  return 0;
}

static int stop_curl(kr_task *task) {
  curl *c;
  c = (curl *)task->data;
  task_del_program(task, &c->curl);
  return 0;
}

int kr_curl_create(kr_task *task) {
  curl *c;
  c = kr_allocz(1, sizeof(*c));
  strcpy(c->curl_cmd, "curl");
  strcpy(c->rflag1, "--stderr");
  strcpy(c->rflag2, "-");
  strcpy(c->oflag1, "-o");
  strcpy(c->oflag2, task->info.curl.dest);
  strcpy(c->url, task->info.curl.url);
  c->cmd[0] = c->curl_cmd;
  c->cmd[1] = c->rflag1;
  c->cmd[2] = c->rflag2;
  c->cmd[3] = c->oflag1;
  c->cmd[4] = c->oflag2;
  c->cmd[5] = c->url;
  c->cmd[6] = NULL;
  c->curl.cmd = c->cmd;
  c->curl.priority = KR_PRIORITY_NONINTERACTIVE;
  task->data = c;
  return 0;
}

int kr_curl_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_curl_start(kr_task *task) {
  start_curl(task);
  return 0;
}

int kr_curl_stop(kr_task *task) {
  curl *c;
  c = (curl *)task->data;
  if (program_running(&c->curl)) {
    stop_curl(task);
  }
  return 0;
}

int kr_curl_destroy(kr_task *task) {
  curl *c;
  kr_curl_stop(task);
  c = (curl *)task->data;
  free(c);
  return 0;
}
