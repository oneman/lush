#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define NAIVE "★⚡☣☢☃☠"
#define PROPER L"★⚡☣☢☃☠"

int main(int argc, char *argv[]) {
  size_t syms;
  size_t chars;
  const char *naive_test_string;
  const wchar_t *proper_test_string;
  naive_test_string = NAIVE;
  proper_test_string = PROPER;
  printf("sizeof naive is %zu sizeof proper is %zu\n",
   sizeof(NAIVE), sizeof(PROPER));
  chars = strlen(naive_test_string);
  syms = wcslen(proper_test_string);
  printf("%s is %zu symbols %zu chars\n",
   naive_test_string, syms, chars);
  return 0;
}
