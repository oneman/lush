#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE ((26 * 26) + 26)
#endif

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <locale.h>
/* build: gcc -g -Wall -o vscii_locale_test vscii_locale_test.c */

int main(int argc, char **argv) {
  int a = 0;
  int b = 0;
  unsigned char c = 0;
  locale_t cur_locale = duplocale(LC_GLOBAL_LOCALE);
  for (c = 0; c != 255; c++) {
    a = isalpha(c);
    if (a) {
      printf("%d = isalpha(%c);\n", a, c);
      b++;
    } else {
      a = isalpha_l(c, cur_locale);
      if (a) {
        printf("%d = isalpha_l(%c, LC_GLOBAL_LOCALE);\n", a, c);
        b++;
      }
    }
  }
  if (b == 52) {
    printf("52 ints that return nonzero from isalpha(), seems legit\n");
    return 0;
  }
  printf("somekind of insane failure of reality has happened\n");
  return b;
}
