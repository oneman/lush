#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/json/parser.h>
#include <krad/tasks/internal.h>
#include "xmms.h"

#define XMMS_BUF_MAX_SZ 512

typedef struct {
  char argv[8][80];
  char env[32];
  char *dummyhead[8];
  kr_program xmmscli;
  uint8_t buffer[XMMS_BUF_MAX_SZ];
  int buffer_len;
} xmms;

static int stop_xmms(kr_task *task) {
  xmms *xc;
  xc = (xmms *)task->data;
  task_del_program(task, &xc->xmmscli);
  return 0;
}

static int xmms_event(kr_event *event) {
  struct iovec iov[1];
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int err;
  xmms *xc;
  kr_task *task;
  task = (kr_task *)event->user;
  xc = (xmms *)task->data;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = xc->buffer;
  iov[0].iov_len = XMMS_BUF_MAX_SZ;
  if (event->events & EPOLLIN) {
    printk("POLLIN");
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("xmms event: readv got %s", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("xmms event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("xmms output: %d chars, '%s'", bytes_read, xc->buffer);
    xc->buffer_len += bytes_read;
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("xmms event: EPOLLERR event on fd");
    task_raise_event(task, KR_TASK_FAILURE);
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("xmms event: EPOLLHUP event on fd: %d", event->fd);
    task_raise_event(task, KR_TASK_SUCCESS);
  }
  if (done == 1) {
    stop_xmms(task);
  }
  return 0;
}

int kr_xmms_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_xmms_create(kr_task *task) {
  xmms *xc;
  //char *filename;
  char *env;
  xc = kr_allocz(1, sizeof(*xc));
  env = task->info.params.xmms.env;
  //filename = task->info.params.xmms.filename;
  snprintf(xc->argv[0], sizeof(xc->argv[0]), "xmms2");
  snprintf(xc->argv[1], sizeof(xc->argv[1]), "add");
  snprintf(xc->argv[2], sizeof(xc->argv[2]), "-n");
  //snprintf(xc->argv[3], sizeof(xc->argv[3]), "%s", filename);
  printk("[%s] %s %s %s %s", env, xc->argv[0], xc->argv[1], xc->argv[2],
   xc->argv[3]);
  xc->dummyhead[0] = xc->argv[0];
  xc->dummyhead[1] = xc->argv[1];
  xc->dummyhead[2] = xc->argv[2];
  xc->dummyhead[3] = xc->argv[3];
  xc->dummyhead[4] = NULL;
  xc->xmmscli.priority = KR_PRIORITY_NORMAL;
  snprintf(xc->xmmscli.env, sizeof(xc->xmmscli.env), "%s", env);
  xc->xmmscli.cmd = xc->dummyhead;
  task->data = xc;
  //xc->media = task_create_media(task);
  //if (xc->media == NULL) return -2;
  //info = kr_media_get_info(xc->media);
  //printk("new media %s", info->id);
  //xc->buffer = kr_allocz(XMMS_BUF_MAX_SZ, sizeof(char));
  //xc->buffer_sz = 0;
  return 0;
}

int kr_xmms_start(kr_task *task) {
  xmms *xc;
  if (task == NULL) return -1;
  xc = (xmms *)task->data;
  xc->xmmscli.handler = xmms_event;
  task_add_program(task, &xc->xmmscli);
  return 0;
}

int kr_xmms_stop(kr_task *task) {
  xmms *xc;
  xc = (xmms *)task->data;
  if (task == NULL) return -1;
  if (program_running(&xc->xmmscli)) {
    stop_xmms(task);
  }
  //if (xc->media) task_destroy_media(task, xc->media);
  return 0;
}

int kr_xmms_destroy(kr_task *task) {
  xmms *xc;
  kr_xmms_stop(task);
  xc = (xmms *)task->data;
  //free(xc->buffer);
  free(xc);
  return 0;
}
