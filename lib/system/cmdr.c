#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <krad/app/debug.h>
#include <krad/loop/loop.h>
#include <krad/system/program.h>
#include <krad/system/fdshare.h>
#include <krad/system/fdset.h>

#include "cmdr.h"

typedef struct {
  kr_loop *loop;
  int fd;
  kr_fdset *fdset;
  size_t pos;
  char buf[8192];
} launcher;

struct kr_cmdr {
  pid_t pid;
  void *user;
  int fd;
};

static int open_program(kr_program *program);
/*static int close_program(kr_program *program);*/

#define KR_PROGRAM_PARENT 0
#define KR_PROGRAM_CHILD 1

static int open_program(kr_program *program) {
  int ret;
  pid_t pid;
  sigset_t mask;
  int std_io[2];
  char *envarr[2];
  envarr[0] = program->env;
  envarr[1] = NULL;
  /* FIXME add stderr pipe
  int std_err[2];
  if (pipe(std_err) != 0) return -1;
  */
  if (!program->cfd) {
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, std_io);
    if (ret) exit(1);
  } else {
    std_io[KR_PROGRAM_CHILD] = program->cfd;
  }
  pid = fork();
  if (pid < 0) {
    return pid;
  } else if (pid == 0) {
    if (!program->cfd) {
      ret = close(std_io[KR_PROGRAM_PARENT]);
      if (ret) exit(1);
    }
    ret = close(0);
    if (ret) exit(1);
    ret = close(1);
    if (ret) exit(1);
    ret = fcntl(std_io[KR_PROGRAM_CHILD], F_DUPFD, 0);
    if (ret != 0) exit(1);
    ret = fcntl(std_io[KR_PROGRAM_CHILD], F_DUPFD, 1);
    if (ret != 1) exit(1);
    ret = close(std_io[KR_PROGRAM_CHILD]);
    if (ret) exit(1);
    if (program->priority != KR_PRIORITY_UNKNOWN
     && program->priority != KR_PRIORITY_NORMAL) {
      kr_priority_set(program->priority);
    }
    setpgid(0, 0);
    sigemptyset(&mask);
    sigfillset(&mask);
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) != 0) {
      exit(1);
    }
    printf("moo: %s\n", program->env);
    printf("roar: %s\n", envarr[0]);
    execvpe(program->cmd[0], program->cmd, envarr);
    exit(1);
  } else {
    program->pid = pid;
    ret = close(std_io[KR_PROGRAM_CHILD]);
    if (ret) exit(1);
    if (!program->cfd) {
      program->fd = std_io[KR_PROGRAM_PARENT];
      fcntl(program->fd, F_SETFL, O_NONBLOCK);
      fcntl(program->fd, F_SETFD, FD_CLOEXEC);
    }
  }
  return 0;
}

/*static int close_program(kr_program *program) {
  int ret;
  int status;
  usleep(3000);
  ret = waitpid(program->pid, &status, WNOHANG);
  if (ret == 0) {
    // FIXME do kill here first.. then nohang loop waitpid on the pg
    killpg(program->pid, SIGTERM);
    printk("close_program sent TERM signal");
    usleep(10000);
    ret = waitpid(program->pid, &status, WNOHANG);
    if (ret == 0) {
      printke("SIGTERM wasn't enough kill -9'ing");
      killpg(program->pid, SIGKILL);
      ret = waitpid(program->pid, &status, 0);
    }
  }
  if (ret == -1) {
    printke("waitpid error %s", strerror(errno));
  }
  close(program->fd);
  memset(program, 0, sizeof(*program));
  if (ret > 0) return 0;
  return -1;
}*/

int program_running(kr_program *program) {
  return program->pid;
}

static void run_program(launcher *x, kr_program_setup *setup, int fd) {
  int ret;
  int i;
  char *cmd[10];
  ret = 0;
  kr_program program;
  program.priority = setup->priority;
  memset(cmd, 0, sizeof(cmd));
  for (i = 0; i < setup->argc; i++) {
    cmd[i] = setup->argv[i];
  }
  program.cmd = cmd;
  program.cfd = fd;
  memcpy(program.env, setup->env, 32);
  ret = open_program(&program);
  if (ret != 0) exit(1);
}

static int run_commands(launcher *x) {
  int pos;
  int ret;
  int fd;
  kr_program_setup *setup;
  pos = 0;
  while (x->pos - pos >= sizeof(kr_program_setup)) {
    setup = (kr_program_setup *)x->buf + pos;
    ret = kr_fdset_get(x->fdset, &fd, 1);
    if (ret != 0) exit(1);
    run_program(x, setup, fd);
    pos += sizeof(kr_program_setup);
  }
  memmove(x->buf, x->buf + pos, x->pos - pos);
  x->pos -= pos;
  return 0;
}

static int read_commands(launcher *x) {
  int ret;
  int fds[128];
  int nfds;
  nfds = 128;
  ret = recv_fds(x->fd, (uint8_t *)x->buf + x->pos,
   sizeof(x->buf) - x->pos, fds, &nfds);
  if (ret > 0) {
    x->pos += ret;
    if (nfds > 0) kr_fdset_put(x->fdset, fds, nfds);
  }
  return ret;
}

static int handle_commands(kr_event *event) {
  int ret;
  launcher *krx;
  krx = (launcher *)event->user;
  if (event->events & EPOLLIN) {
    ret = read_commands(krx);
    if (ret < 1) return -1;
    ret = run_commands(krx);
    if (ret != 0) return -1;
  }
  if (event->events & EPOLLHUP) {
    return -1;
  }
  return 0;
}

static int init_launcher(kr_loop *loop, void *user) {
  kr_event harness;
  static launcher *krx;
  if (!loop) return -1;
  krx = (launcher *)user;
  if (!user) return -1;
  krx->loop = loop;
  krx->fdset = kr_allocz(1, kr_fdset_sizeof());
  kr_fdset_init(krx->fdset);
  harness.user = krx;
  harness.events = EPOLLIN;
  harness.handler = handle_commands;
  harness.fd = krx->fd;
  kr_loop_add(krx->loop, &harness);
  return 0;
}

static int signal_event(kr_event *event) {
  ssize_t s;
  struct signalfd_siginfo fdsi;
  /*launcher *krx;
  krx = (launcher *)event->user;*/
  char *name = "Cmdr";
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", name);
        printk("%s: Shutting down", name);
        return -1;
      default:
        printk("%s: Got Signal %u", name, fdsi.ssi_signo);
    }
  }
  return 0;
}

static int shutdown_launcher(void *user) {
  static launcher *krx;
  if (!user) return -1;
  krx = (launcher *)user;
  kr_fdset_close(krx->fdset);
  free(krx->fdset);
  /* reap programs ? */
  return 0;
}

static void run_program_launcher(int fd) {
  static kr_mainloop_setup loop_setup;
  static launcher krx;
  krx.fd = fd;
  loop_setup.user = &krx;
  loop_setup.signal_handler = signal_event;
  loop_setup.startup_handler = init_launcher;
  loop_setup.shutdown_handler = shutdown_launcher;
  kr_mainloop(&loop_setup);
}

int kr_program_end_input(kr_program *program) {
  int ret;
  if (!program) return -1;
  ret = shutdown(program->fd, SHUT_WR);
  return ret;
}

int kr_program_close(kr_program *program) {
  int ret;
  if (!program) return -1;
  ret = close(program->fd);
  return ret;
}

int kr_program_open(kr_cmdr *cmdr, kr_program *program,
 kr_program_setup *setup) {
  int ret;
  int std_io[2];
  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, std_io);
  if (ret) exit(1);
  ret = send_fds(cmdr->fd, (char *)setup, sizeof(*setup), &std_io[1], 1);
  close(std_io[1]);
  program->fd = std_io[0];
  fcntl(program->fd, F_SETFL, O_NONBLOCK);
  fcntl(program->fd, F_SETFD, FD_CLOEXEC);
  return ret;
}

int kr_cmdr_destroy(kr_cmdr *cmdr) {
  int ret;
  if (!cmdr) return -1;
  ret = close(cmdr->fd);
  return ret;
}

kr_cmdr *kr_cmdr_create(kr_cmdr_setup *setup) {
  static kr_cmdr cmdr;
  static int run = 0;
  pid_t pid;
  int ret;
  int io[2];
  int p;
  int c;
  if (run) return NULL;
  run = 1;
  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, io);
  if (ret) exit(1);
  p = io[0];
  c = io[1];
  pid = fork();
  if (pid < 0) {
    exit(1);
  } else if (pid == 0) {
    setup->startup_cb(setup->user);
    ret = close(p);
    if (ret) exit(1);
    ret = close(0);
    if (ret) exit(1);
    ret = close(1);
    if (ret) exit(1);
    ret = close(2);
    if (ret) exit(1);
    ret = fcntl(c, F_SETFL, O_NONBLOCK);
    if (ret) exit(1);
    ret = fcntl(c, F_SETFD, FD_CLOEXEC);
    if (ret) exit(1);
    ret = prctl(PR_SET_NAME, "kr_cmdr", NULL, NULL, NULL);
    if (ret) exit(1);
    run_program_launcher(c);
    exit(1);
  } else {
    ret = close(c);
    if (ret) exit(1);
    ret = fcntl(p, F_SETFL, O_NONBLOCK);
    if (ret) exit(1);
    cmdr.fd = p;
    cmdr.pid = pid;
    return &cmdr;
  }
  exit(1);
  return NULL;
}
