/*int timespec_sub(struct timespec a, struct timespec b,
 struct timespec *result);
int timespec_compare(struct timespec one, struct timespec two);
int init_timeout(kr_task_pool *tasks);
int compute_timeout(kr_task_pool *tasks);
int handle_timeout(kr_event *event);
int disable_timeout(kr_task_pool *tasks);
int update_timeout(kr_task_pool *tasks);

int timespec_sub(struct timespec a, struct timespec b,
 struct timespec *result) {
  result->tv_sec = a.tv_sec - b.tv_sec;
  result->tv_nsec = a.tv_nsec - b.tv_nsec;
  if (result->tv_nsec < 0) {
    --result->tv_sec;
    result->tv_nsec += 1000000000;
  }
  return 0;
}

int timespec_compare(struct timespec one, struct timespec two) {
  if ((one.tv_sec > two.tv_sec) || ((one.tv_sec == two.tv_sec) && (one.tv_nsec
    > two.tv_nsec))) {
    return 1;
  } else if (one.tv_sec == two.tv_sec && one.tv_nsec == two.tv_nsec) {
    return 0;
  } else {
    return -1;
  }
}

int init_timeout(kr_task_pool *tasks) {
  int ret;
  kr_event event;
  tasks->tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (tasks->tfd < 0) {
    printke("Media Ops Server: timerfd_create fail");
    return -1;
  }
  memset(&event, 0, sizeof(event));
  event.fd = tasks->tfd;
  event.handler = handle_timeout;
  event.user = tasks;
  event.events = EPOLLIN;
  ret = kr_loop_add(tasks->loop, &event);
  if (ret != 0) {
    close(tasks->tfd);
    return ret;
  }
  return 0;
}

int compute_timeout(kr_task_pool *tasks) {
  struct timespec oldest;
  int i;
  kr_task *task;
  clock_gettime(CLOCK_MONOTONIC, &oldest);
  tasks->next_timeout = oldest;
  tasks->check_task = NULL;
  i = 0;
  while ((task = kr_pool_iterate_active(tasks->task_pool, &i))) {
    if (task->info.status.state != 0) {
      if (task->info.status.state == KR_TASK_RUNNING) {
        if (timespec_compare(oldest, task->last.etime) == 1) {
          oldest = task->last.etime;
          tasks->next_timeout = oldest;
          tasks->check_task = task;
        }
      }
    }
  }
  return 0;
}

int handle_timeout(kr_event *event) {
  int ret;
  kr_task *task;
  kr_task_pool *tasks;
  printk("Media Ops Server: in handle_timeout");
  tasks = (kr_ops *)event->user;
  if (tasks->check_task == NULL) {
    return -1;
  }
  task = tasks->check_task;
  ret = task_stop(task);
  if (ret != 0) {
    printke("handle timeout: task_stop failed");
    return ret;
  }
  run_event(task, KR_TASK_TIMEOUT);
  return 0;
}

int disable_timeout(kr_task_pool *tasks) {
  int ret;
  struct itimerspec new_value;
  memset(&new_value, 0, sizeof(new_value));
  ret = timerfd_settime(tasks->tfd, 0, &new_value, NULL);
  if (ret != 0) {
    printk("Media Ops Server: couldn't disable timer");
  }
  return ret;
}

int update_timeout(kr_task_pool *tasks) {
  int ret;
  struct timespec ts;
  struct timespec diff;
  struct itimerspec new_value;
  compute_timeout(tasks);
  if (tasks->check_task == NULL) {
    disable_timeout(tasks);
    return 0;
  }
  clock_gettime(CLOCK_MONOTONIC, &ts);
  timespec_sub(tasks->next_timeout, ts, &diff);
  memset(&new_value, 0, sizeof(new_value));
  new_value.it_value.tv_sec = diff.tv_sec + KR_OPS_TIMEOUT_SEC;
  new_value.it_value.tv_nsec = diff.tv_nsec;
  if (new_value.it_value.tv_sec < 0) {
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_nsec = 1000;
  }
  ret = timerfd_settime(tasks->tfd, 0, &new_value, NULL);
  if (ret != 0) {
    printke("Media Ops Server: timerfd settime fail");
    return -5;
  }
  return 0;
}
*/
