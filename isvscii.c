#include "stdiov.h"

/* build: gcc -g -Wall -o isvscii isvscii.c */

int main(int argc, char **argv) {
  int ret = 0;
  uint8_t c = 0;
  if ((argc == 0) || (argc > 2)) {
    fprintf(stdout, "n\n");
    return 0;
  }
  if (argc == 1) {
    ret = read(1, &c, 1);
  if (ret != 1) {
      fprintf(stdout, "\n");
      return 0;
    }
  }
  if (argc == 2) {
    c = argv[1][0];
  }
  ret = isvscii(c);
  if (ret == 1) {
    fprintf(stdout, "y\n");
    return 1;
  }
  fprintf(stdout, "n\n");
  return 0;
}
