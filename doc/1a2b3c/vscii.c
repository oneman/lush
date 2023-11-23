#include <stdio.h>
/* VSCII ascii c file 208 generali qtest 26 */
int main(int argc, char **argv) {
  int i, j;
  for (i = 1; i < 3*3*3; i++) {
    printf("%c ", i + 2 * 2 * 2 * 2 * 2 * 3);
    for (j = 5 - 1; j >= 0; j--) {
      putchar(i & (1u << j) ? '1' : '0');
    }
    printf("\n");
  }
  return 0;
}
