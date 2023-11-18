#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>

#define _KR_LAUNCHER_C

#include <krad/system/program.h>
#include <krad/system/fdset.c>
#include <krad/system/fdshare.c>

typedef struct {
  kr_fdset fdset;
  size_t pos;
  char buf[8192];
} launcher;

static void run_program(launcher *l, kr_program_setup *setup, int fd) {
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
  ret = open_program(&program);
  if (ret != 0) exit(1);
  if (0) {
    ret = send_fds(1, (char *)&program.pid, sizeof(pid_t), &program.fd, 1);
    if (ret != 0) exit(1);
  }
}

static int run_commands(launcher *l) {
  int pos;
  int ret;
  int fd;
  kr_program_setup *setup;
  pos = 0;
  while (l->pos - pos >= sizeof(kr_program_setup)) {
    setup = (kr_program_setup *)l->buf + pos;
    ret = kr_fdset_get(&l->fdset, &fd, 1);
    if (ret != 0) exit(1);
    run_program(l, setup, fd);
    pos += sizeof(kr_program_setup);
  }
  memmove(l->buf, l->buf + pos, l->pos - pos);
  l->pos -= pos;
  return 0;  
}

static int read_commands(launcher *l) {
  int ret;
  int fds[128];
  int nfds;
  nfds = 128;
  //ret = read(0, l->buf + l->pos, sizeof(l->buf) - l->pos);
  ret = recv_fds(0, (uint8_t *)l->buf + l->pos, sizeof(l->buf) - l->pos, fds, &nfds);
  if (ret > 0) l->pos += ret;
  if (nfds > 0) kr_fdset_put(&l->fdset, fds, nfds);
  return ret;
}

int main(int argc, char *argv[]) {
  static launcher l;
  int ret;
  sigset_t mask;
  char *code;
  struct pollfd in[1];
  code = getenv("KRAD_LAUNCH_CODE");
  if (!code || strlen(code) != 6) exit(1);
  if (memcmp(code, "145666", 6) != 0) exit(1);
  sigemptyset(&mask);
  sigfillset(&mask);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
    exit(1);
  }
  ret = fcntl(0, F_SETFL, O_NONBLOCK);
  if (ret != 0) exit(1);
  kr_fdset_init(&l.fdset);
  in[0].fd = 0;
  in[0].events = POLLIN;
  for (;;) {
    ret = poll(in, 1, -1);
    if (ret < 1) break;
    if ((in[0].revents & POLLIN) == 0) break;
    ret = read_commands(&l);
    if (ret < 1) break;
    ret = run_commands(&l);
    if (ret != 0) break;
    if ((in[0].revents & POLLHUP) == 1) break;
  }
  kr_fdset_close(&l.fdset);
  return 0;
}
