#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <krad/tasks/internal.h>

#define TIMEOUT_SEC 1
#define TIMEOUT_NSEC 0

typedef struct {
  int timerfd;
  kr_program stress;
  kr_program pgrep;
} valgrind_stressor;

static void start_stress(kr_task *task);
static void stop_stress(kr_task *task);
static int start_pgrep(kr_task *task);
static int stop_pgrep(kr_task *task);

static int stress_event(kr_event *event) {
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
  if (event->events & EPOLLIN && !(event->events & EPOLLHUP)) {
    printk("GOT EPOLLIN without EPOLLHUP");
  }
  if (event->events & EPOLLIN) {
    printk("POLLIN");
    ret = readv(event->fd, iov, 1);
    if (ret == -1) {
      if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
        err = errno;
        printke("stress event: readv got %s", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("stress event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("%d chars, '%s'", bytes_read, output);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("stress event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("stress event: EPOLLHUP event on fd: %d", event->fd);
  }
  if (done == 1) {
    task = (kr_task *)event->user;
    stop_stress(task);
  }
  return 0;
}

static int pgrep_event(kr_event *event) {
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
  valgrind_stressor *vs;
  task = (kr_task *)event->user;
  vs = (valgrind_stressor *)task->data;
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
        printke("pgrep event: readv got %s", strerror(err));
        done = 1;
      }
    } else if (ret == 0) {
      if (bytes_read == 0) {
        printke("pgrep event: readv got 0 bytes");
      }
    } else {
      bytes_read += ret;
    }
  }
  /*temp kuldge*/
  if (bytes_read > 0) {
    printk("%d chars, '%s'", bytes_read, output);
  }
  /*end temp kludge*/
  if (event->events & EPOLLERR) {
    done = 1;
    printke("pgrep event: EPOLLERR event on fd");
  }
  if (event->events & EPOLLHUP) {
    done = 1;
    printk("pgrep event: EPOLLHUP event on fd: %d", event->fd);
  }
  if (event->events & EPOLLIN) {
    nfound = atoi(output);
    printk("nfound :%d", nfound);
    if (nfound == 0) {
      printk("pgrep: NO valgrind processes");
      if (program_running(&vs->stress)) {
        task = (kr_task *)event->user;
        stop_stress(task);
      }
    } else if (nfound > 0) {
      printk("pgrep: FOUND valgrind processes");
      if (!program_running(&vs->stress)) {
        task = (kr_task *)event->user;
        start_stress(task);
      }
    }
  }
  if (done == 1) {
    task = (kr_task *)event->user;
    stop_pgrep(task);
  }
  return 0;
}

static int timer_event(kr_event *event) {
  uint64_t nexp;
  int ret;
  int tfd;
  kr_task *task;
  task = (kr_task *) event->user;
  tfd = event->fd;
  nexp = 0;
  ret = read(tfd, &nexp, sizeof(nexp));
  if (ret == -1) {
    printk("timer event: failed to read timerfd");
  }
  start_pgrep(task);
  return 0;
}

static void start_stress(kr_task *task) {
  char *stress_cmd[] = { "stress", "--cpu", "4", "--vm", "4", NULL };
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  memset(&vs->stress, 0, sizeof(vs->stress));
  vs->stress.cmd = stress_cmd;
  vs->stress.handler = stress_event;
  task_add_program(task, &vs->stress);
  printk("START_STRESS DONE");
}

static void stop_stress(kr_task *task) {
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  task_del_program(task, &vs->stress);
  printk("STOP_STRESS DONE");
}

static int start_pgrep(kr_task *task) {
  char *pgrep_cmd[] = { "pgrep", "--full", "--count", "^valgrind .*", NULL };
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  memset(&vs->pgrep, 0, sizeof(vs->pgrep));
  vs->pgrep.cmd = pgrep_cmd;
  vs->pgrep.handler = pgrep_event;
  task_add_program(task, &vs->pgrep);
  task_raise_event(task, KR_TASK_PROGRESS);
  return 0;
}

static int stop_pgrep(kr_task *task) {
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  task_del_program(task, &vs->pgrep);
  return 0;
}

static int start_timer(kr_task *task) {
  int ret;
  kr_event event;
  valgrind_stressor *vs;
  struct itimerspec new_value;
  int fd;
  fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (fd < 0) {
    printke("Valgrind Stressor: timerfd_create fail");
    return -1;
  }
  memset(&event, 0, sizeof(event));
  event.fd = fd;
  event.handler = timer_event;
  event.user = task;
  event.events = EPOLLIN;
  ret = kr_loop_add(task_get_loop(task), &event);
  memset(&new_value, 0, sizeof(new_value));
  new_value.it_value.tv_sec = TIMEOUT_SEC;
  new_value.it_value.tv_nsec = TIMEOUT_NSEC;
  new_value.it_interval.tv_sec = TIMEOUT_SEC;
  new_value.it_interval.tv_nsec = TIMEOUT_NSEC;
  ret = timerfd_settime(fd, 0, &new_value, NULL);
  if (ret != 0) {
    printke("valgrind_stressor: timerfd settime fail");
    return -5;
  }
  vs = (valgrind_stressor *)task->data;
  vs->timerfd = fd;
  return 0;
}

int stop_timer(kr_task *task) {
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  kr_loop_del(task_get_loop(task), vs->timerfd);
  close(vs->timerfd);
  vs->timerfd = -1;
  return 0;
}

int kr_valgrind_stressor_create(kr_task *task) {
  valgrind_stressor *vs;
  vs = kr_allocz(1, sizeof(*vs));
  vs->timerfd = -1;
  task->data = vs;
  return 0;
}

int kr_valgrind_stressor_ctl(kr_task *task, kr_patchset *patchset) {
  return 0;
}

int kr_valgrind_stressor_start(kr_task *task) {
  start_timer(task);
  return 0;
}

int kr_valgrind_stressor_stop(kr_task *task) {
  valgrind_stressor *vs;
  vs = (valgrind_stressor *)task->data;
  if (vs->timerfd > -1) {
    stop_timer(task);
  }
  if (program_running(&vs->pgrep)) {
    stop_pgrep(task);
  }
  if (program_running(&vs->stress)) {
    stop_stress(task);
  }
  return 0;
}

int kr_valgrind_stressor_destroy(kr_task *task) {
  kr_valgrind_stressor_stop(task);
  free(task->data);
  return 0;
}
