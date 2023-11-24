#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <krad/radio/app.h>

int main(int argc, char *argv[]) {
  char *callsign;
  if (argc == 2) {
    callsign = argv[1];
  } else {
    /*fprintf(stderr, "Usage: %s [callsign]\n", argv[0]);
    exit(1);
    if ((getuid() == 0) || (geteuid() == 0)) {
    fprintf(stderr, "Should not be run as root!\n");
    exit(1);*/
    callsign = "demo";
  }
  /*if (!kr_sysname_valid(callsign)) exit(1);*/
  krad_radio(callsign);
  return -1;
}
