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

#include "krad_jack.h"

typedef struct {
  kr_jack *jack;
} jack_test_ctx;

void jack_event(kr_jack_event_cb_arg *arg) {
  printf("yay event\n");
}

static int test() {
  jack_test_ctx test;
  kr_jack_setup jack_setup;
  memset(&test, 0, sizeof(jack_test_ctx));
  memset(&jack_setup, 0, sizeof(jack_setup));
  sprintf(jack_setup.client_name, "kr_jack_test");
  jack_setup.user = &test;
  jack_setup.event_cb = jack_event;
  test.jack = kr_jack_create(&jack_setup);
  kr_jack_destroy(test.jack);
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
