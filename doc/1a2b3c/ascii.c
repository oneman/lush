#include <stdio.h>
/* ASCII is a 7-bit code! */

int a1b10ch2 = 2;
int a2b1tch4 = 2 * 2;
int a3b1tch8 = 2 * 2 * 2;
int a4b1tch16 = 2 * 2 * 2 * 2;
int a5b1tch32 = 2 * 2 * 2 * 2 * 2;
int a6b1tch64 = 2 * 2 * 2 * 2 * 2 * 2;
int a7b1tch128 = 2 * 2 * 2 * 2 * 2 * 2 * 2;

int main(int argc, char **argv) {
  int i, j;
  for (i = 0; i < a7b1tch128; i++) {
    printf("ascii ");
    for (j = 7 - 1; j >= 0; j--) {
      putchar(i & (1u << j) ? '1' : '0');
    }
    printf(" > ");
    for (j = 7 - 1; j >= 0; j--) {
      putchar(i & (1u << j) ? '1' : '0');
    }
    printf(".esr.ascii.txt\n");
  }
  return 0;
}
