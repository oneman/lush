#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <krad/json/parser.h>
#include <krad/tasks/internal.h>
#include "avprobe.h"

#define AVP_BUF_MAX_SZ 8192 * 8

typedef struct {
  kr_program avprobe;
  char avprobe_cmd[32];
  char outputf[4];
  char outputf_val[5];
  char verbose[3];
  char verbose_val[2];
  char foption[32];
  char soption[32];
  char *cmd[9];
  char in[2048];
  char *buffer;
  size_t buffer_sz;
  kr_media *media;
} avprobe;

static int avp_init_media_items(kr_task *task, avprobe *avp) {
  int res;
  int i;
  kr_avprobe_media_info *info;
  kr_media_item_info mi_info;
  info = &task->info.avprobe.status.media;
  uuid_unparse_lower(task->id, mi_info.task);
  kr_timestamp_now(&mi_info.create_time, &task->create_time);
  mi_info.type = KR_MEDIA_METADATA;
  sprintf(mi_info.metadata.name, "%s", info->format.filename);
  mi_info.metadata.duration = atof(info->format.duration);
  for (i = 0; i < KR_AVP_MAX_STREAMS; i++) {
    if (info->streams[i].codec_type == KR_VIDEO) {
      mi_info.metadata.width = info->streams[i].width;
      mi_info.metadata.height = info->streams[i].height;
      break;
    }
  }
  res = kr_media_add_item(avp->media, &mi_info);
  if (res != 0) return -2;
  return 0;
}

static int handle_avprobe_info(kr_task *task, char *json, ssize_t sz) {
  int res;
  kr_jps *jps;
  kr_avprobe_media_info info;
  char text[4096];
  memset(&info, 0, sizeof(info));
  kr_jps_alloca(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  res = kr_avprobe_media_info_unpack_json(jps_curr_start(jps), &info);
  if (res != 0) return -1;
  res = kr_avprobe_media_info_to_text(text, &info, sizeof(text));
  if (res <= 0) return -1;
  if (res >= sizeof(text)) return -1;
  printk("\nmedia_info:\n%s\n", text);
  task->info.avprobe.status.media = info;
  return 0;
}

static int stop_avprobe(kr_task *task) {
  avprobe *avp;
  avp = (avprobe *)task->data;
  task_del_program(task, &avp->avprobe);
  return 0;
}

static int avprobe_event(kr_event *event) {
  struct iovec iov[1];
  ssize_t bytes_read;
  ssize_t ret;
  int done;
  int err;
  avprobe *avp;
  kr_task *task;
  task = (kr_task *)event->user;
  avp = (avprobe *)task->data;
  done = 0;
  bytes_read = 0;
  iov[0].iov_base = avp->buffer;
  iov[0].iov_len = AVP_BUF_MAX_SZ;
  if (event->events & EPOLLIN) {
    printk("POLLIN");
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("avprobe event: readv got %s", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("avprobe event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    //printk("avprobe output: %d chars, '%s'", bytes_read, output);
    avp->buffer_sz += bytes_read;
    task_raise_event(task, KR_TASK_PROGRESS);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("avprobe event: EPOLLERR event on fd");
    task_raise_event(task, KR_TASK_FAILURE);
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    //printk("avprobe event: EPOLLHUP event on fd: %d", event->fd);
    handle_avprobe_info(task, avp->buffer, avp->buffer_sz);
    avp_init_media_items(task, avp);
    task_raise_event(task, KR_TASK_SUCCESS);
  }
  if (done == 1) {
    stop_avprobe(task);
  }
  return 0;
}

int kr_avprobe_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_avprobe_create(kr_task *task) {
  avprobe *avp;
  kr_media_info *info;
  avp = kr_allocz(1, sizeof(*avp));
  sprintf(avp->avprobe_cmd, "avprobe");
  sprintf(avp->outputf, "-of");
  sprintf(avp->outputf_val, "json");
  sprintf(avp->verbose, "-v");
  sprintf(avp->verbose_val, "0");
  sprintf(avp->foption, "-show_format");
  sprintf(avp->soption, "-show_streams");
  sprintf(avp->in, "%s", task->info.params.avprobe.filename);
  avp->cmd[0] = avp->avprobe_cmd;
  avp->cmd[1] = avp->outputf;
  avp->cmd[2] = avp->outputf_val;
  avp->cmd[3] = avp->verbose;
  avp->cmd[4] = avp->verbose_val;
  avp->cmd[5] = avp->foption;
  avp->cmd[6] = avp->soption;
  avp->cmd[7] = avp->in;
  avp->cmd[8] = NULL;
  avp->avprobe.cmd = avp->cmd;
  task->data = avp;
  avp->media = task_create_media(task);
  if (avp->media == NULL) return -2;
  info = kr_media_get_info(avp->media);
  printk("new media %s", info->id);
  avp->buffer = kr_allocz(AVP_BUF_MAX_SZ, sizeof(char));
  avp->buffer_sz = 0;
  return 0;
}

int kr_avprobe_start(kr_task *task) {
  avprobe *avp;
  if (task == NULL) return -1;
  avp = (avprobe *)task->data;
  avp->avprobe.handler = avprobe_event;
  task_add_program(task, &avp->avprobe);
  return 0;
}

int kr_avprobe_stop(kr_task *task) {
  avprobe *avp;
  avp = (avprobe *)task->data;
  if (task == NULL) return -1;
  if (program_running(&avp->avprobe)) {
    stop_avprobe(task);
  }
  if (avp->media) task_destroy_media(task, avp->media);
  return 0;
}

int kr_avprobe_destroy(kr_task *task) {
  avprobe *avp;
  kr_avprobe_stop(task);
  avp = (avprobe *)task->data;
  free(avp->buffer);
  free(avp);
  return 0;
}
