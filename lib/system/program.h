#if !defined(_system_program_H)
#define _system_program_H (1)

#include <krad/loop/loop.h>
#include "priority.h"
#include "types.h"

typedef struct {
  char env[32];
  char argv[8][80];
  int argc;
  kr_priority priority;
} kr_program_setup;

typedef struct {
  int fd;
  int err_fd;
  int cfd;
  pid_t pid;
  kr_program_setup info;
  char env[32];
  char **cmd;
  kr_priority priority;
  kr_handler *handler;
} kr_program;

#endif
