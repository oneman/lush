#include <sys/signalfd.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>

#if !defined(_loop_loop_H)
# define _loop_loop_H (1)

#include <krad/system/priority.h>

typedef struct kr_loop kr_loop;
typedef struct kr_loop_setup kr_loop_setup;
typedef struct kr_event kr_event;
typedef struct kr_mainloop_setup kr_mainloop_setup;
typedef int (kr_handler)(kr_event *);
typedef int (kr_startup_handler)(kr_loop *, void *);
typedef int (kr_shutdown_handler)(void *);

struct kr_event {
  int fd;
  uint32_t events;
  void *user;
  kr_handler *handler;
};

typedef enum {
  KR_SUPERLOOP,
  KR_SUBLOOP
} kr_loop_type;

struct kr_loop_setup {
  char name[16];
  kr_priority priority;
  kr_loop *master;
};

struct kr_mainloop_setup {
  void *user;
  kr_startup_handler *startup_handler;
  kr_shutdown_handler *shutdown_handler;
  kr_handler *signal_handler;
};

int kr_loop_del(kr_loop *loop, int fd);
int kr_loop_close(kr_loop *loop, int fd);
int kr_loop_mod(kr_loop *loop, kr_event *event);
int kr_loop_add(kr_loop *loop, kr_event *event);
int kr_loop_add_timeout(kr_loop *loop, int ms, kr_handler *handler, void *user);
int kr_loop_destroy(kr_loop *loop);
kr_loop *kr_loop_create(kr_loop_setup *setup);
int kr_loop_setup_init(kr_loop_setup *setup);
void kr_mainloop(kr_mainloop_setup *setup);

#endif
