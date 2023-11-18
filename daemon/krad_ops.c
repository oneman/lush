#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <krad/ops/app.h>

int main(int argc, char *argv[]) {
  char *callsign;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [callsign]\n", argv[0]);
    exit(1);
  }
  if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);
  }
  callsign = argv[1];
  if (!kr_sysname_valid(callsign)) exit(1);
  krad_ops(callsign);
  return -1;
}
