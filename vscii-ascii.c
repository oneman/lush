#include <stdio.h>
/* build: gcc -g -Wall -o vscii-ascii vscii-ascii.c */
int main(int argc, char **argv) {
  int i, j;
  for (i = 0; i < 128; i++) {
    for (j = 8 - 1; j >= 0; j--) {
      putchar(i & (1u << j) ? '1' : '0');
    }
    if ((i >= 32) && (i != 127)) {
      printf(" %c ", i);
    }
    printf("\n");
  }
  return 0;
}
