#if !defined(_system_cmdr_H)
#define _system_cmdr_H (1)

#include "program.h"

typedef struct kr_cmdr kr_cmdr;
typedef void (kr_cmdr_startup_cb)(void *user);

typedef struct {
  kr_cmdr_startup_cb *startup_cb;
  void *user;
} kr_cmdr_setup;

int program_running(kr_program *program);

int kr_program_end_input(kr_program *program);
int kr_program_close(kr_program *program);
int kr_program_open(kr_cmdr *cmdr, kr_program *program,
 kr_program_setup *setup);
int kr_cmdr_destroy(kr_cmdr *cmdr);
kr_cmdr *kr_cmdr_create(kr_cmdr_setup *setup);

#endif
