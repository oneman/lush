#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sched.h>

#define __exit_thread_inline(val) \
  asm volatile ("syscall" :: "a" (__NR_exit), "D" (val))

typedef void (thread_func)(void *arg);
typedef struct kr_thread_params kr_thread_params;
typedef struct kr_thread kr_thread;
typedef struct kr_harness kr_harness;

struct kr_thread_params {
  thread_func *fn;
  void *arg;
  size_t stack_sz;
};

struct kr_thread {
  void *stack;
  pid_t tid;
  kr_thread_params params;
};

struct kr_harness {
  kr_thread thread;

};

static void thread_exit() {
  __exit_thread_inline(0);
}

static int run(void *arg) {
  kr_thread *thread;
  thread = (kr_thread *)arg;
  thread->params.fn(thread->params.arg);
  thread_exit();
  return -1;
}

int kr_thread_free(kr_thread *thread) {
  void *save;
  if (thread == NULL) return -1;
  save = thread;
  memset(thread, 0, sizeof(*thread));
  free(save);
  return 0;
}

kr_thread *kr_thread_create(kr_thread_params *params) {
  int ret;
  void *top;
  int flags;
  kr_thread thread;
  if (params == NULL) return NULL;
  flags = 0;
  flags |= CLONE_THREAD | CLONE_VM | CLONE_SIGHAND | CLONE_SYSVSEM;
  flags |= CLONE_FS | CLONE_FILES | CLONE_IO;
  thread.params = *params;
  thread.stack = calloc(1, thread.params.stack_sz + sizeof(kr_thread));
  top = thread.stack + thread.params.stack_sz;
  memcpy(top, &thread, sizeof(kr_thread));
  ret = clone(run, top, flags, top);
  if (ret == -1) {
    free(thread.stack);
    return NULL;
  }
  return (kr_thread *)thread.stack;
}

static void print_id(char *name) {
  pid_t pid;
  pid_t tid;
  pid = getpid();
  tid = syscall(SYS_gettid);
  printf("%s: PID: %d TID: %d\n", name, pid, tid);
}

void hello(void *arg) {
  print_id("Electric");
  usleep(5 * 1000 * 1000);
}

static int test() {
  int i;
  kr_thread *thread[16];
  kr_thread_params params;
  sigset_t mask;
  sigemptyset(&mask);
  sigfillset(&mask);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
    fprintf(stderr, "Could not set signal mask!");
    return -1;
  }
  print_id("main program");
  params.fn = hello;
  params.arg = NULL;
  params.stack_sz = 4096;
  for (i = 0; i < 16; i++) {
    thread[i] = kr_thread_create(&params);
    if (!thread[i]) {
      printf("Thread launch Error: %s\n", strerror(errno));
      return -1;
    }
    usleep(10000);
  }
  usleep(6000000);
  for (i = 0; i < 16; i++) {
    kr_thread_free(thread[i]);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = test();
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
