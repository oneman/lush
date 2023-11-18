#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/json/parser.h>
#include <krad/tasks/internal.h>
#include "ydl.h"

#define YDL_JSON_MAX_SZ 8192 * 8

typedef struct {
  kr_program ydl;
  kr_media *media;
  char args[8][32];
  char *cmd[8];
  char url[512];
  char *json;
  size_t json_sz;
} ydl;

static int ydl_init_media_items(kr_task *task, ydl *y) {
  int i;
  int res;
  kr_ydl_video_info *info;
  kr_media_item_info mi_info;
  info = &task->info.ydl.status.video;
  for (i = 0; i < KR_YDL_MAX_FORMATS; i++) {
    if (info->formats[i].url[0]) {
      memset(&mi_info, 0, sizeof(mi_info));
      uuid_unparse_lower(task->id, mi_info.task);
      kr_timestamp_now(&mi_info.create_time, &task->create_time);
      mi_info.type = KR_MEDIA_URL;
      res = snprintf(mi_info.url.url, sizeof(mi_info.url.url),
       "%s", info->formats[i].url);
      if (res >= sizeof(mi_info.url.url)) return -1;
      res = kr_media_add_item(y->media, &mi_info);
      if (res != 0) return -2;
    }
  }
  return 0;
}

static int handle_ydl_info(kr_task *task, char *json, size_t sz) {
  int res;
  kr_jps *jps;
  kr_ydl_video_info info;
  memset(&info, 0, sizeof(info));
  kr_jps_alloca(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  res = kr_ydl_video_info_unpack_json(jps_curr_start(jps), &info);
  if (res != 0) return -1;
  res = kr_ydl_video_info_to_text(json, &info, YDL_JSON_MAX_SZ);
  if (res <= 0) return -1;
  if (res >= YDL_JSON_MAX_SZ) return -1;
  printk("\nydl_video_info:\n%s\n", json);
  task->info.ydl.status.video = info;
  return 0;
}

static int stop_ydl(kr_task *task) {
  ydl *y;
  y = (ydl *)task->data;
  task_del_program(task, &y->ydl);
  return 0;
}

static int ydl_event(kr_event *event) {
  ydl *y;
  struct iovec iov[1];
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int err;
  kr_task *task;
  task = (kr_task *)event->user;
  y = (ydl *)task->data;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = y->json;
  iov[0].iov_len = YDL_JSON_MAX_SZ - 1;
  if (event->events & EPOLLIN) {
    printk("POLLIN");
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("ydl event: readv got %s", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("ydl event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    y->json_sz += bytes_read;
    //printk("ydl output: %d chars, '%.*s'", bytes_read, bytes_read, output);
    //handle_info(output, bytes_read);
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("ydl event: EPOLLERR event on fd");
    task_raise_event(task, KR_TASK_FAILURE);
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("ydl event: EPOLLHUP event on fd: %d", event->fd);
    handle_ydl_info(task, y->json, y->json_sz);
    ydl_init_media_items(task, y);
    task_raise_event(task, KR_TASK_SUCCESS);
  }
  if (done == 1) {
    stop_ydl(task);
  }
  return 0;
}

int kr_ydl_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_ydl_create(kr_task *task) {
  ydl *y;
  if (task == NULL) return -1;
  y = kr_allocz(1, sizeof(*y));
  sprintf(y->args[0], "youtube-dl");
  sprintf(y->args[1], "-j");
  sprintf(y->url, "%s", task->info.params.ydl.url);
  y->cmd[0] = y->args[0];
  y->cmd[1] = y->args[1];
  y->cmd[2] = y->url;
  y->cmd[3] = NULL;
  y->ydl.cmd = y->cmd;
  task->data = y;
  y->json = kr_allocz(YDL_JSON_MAX_SZ, sizeof(char));
  y->json_sz = 0;
  y->media = task_create_media(task);
  if (y->media == NULL) return -2;
  return 0;
}

int kr_ydl_start(kr_task *task) {
  ydl *y;
  if (task == NULL) return -1;
  y = (ydl *)task->data;
  y->ydl.handler = ydl_event;
  task_add_program(task, &y->ydl);
  return 0;
}

int kr_ydl_stop(kr_task *task) {
  ydl *y;
  y = (ydl *)task->data;
  if (task == NULL) return -1;
  if (program_running(&y->ydl)) {
    stop_ydl(task);
  }
  if (y->media) task_destroy_media(task, y->media);
  return 0;
}

int kr_ydl_destroy(kr_task *task) {
  ydl *y;
  kr_ydl_stop(task);
  y = (ydl *)task->data;
  free(y->json);
  free(y);
  return 0;
}
