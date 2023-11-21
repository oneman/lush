#include "stdiov.h"

/* build: gcc -g -Wall -o isvsciiword isvsciiword.c */

int main(int argc, char *argv[]) {
  int len = 0;
  int ret = 0;
  static char word[VWORD_LEN_MAX];

  if ((argc == 0) || (argc > 2)) {
    fprintf(stderr, "argument count can be only 1 or 2\n");
    fprintf(stdout, "n\n");
    return 0;
  }
  if (argc == 1) {
    ret = read(STDIN_FILENO, word, 78);
    if (ret < 1) {
      fprintf(stderr, "read() fail, ret %d\n", ret);
      return 0;
    }
    len = ret;
  } else {
    if (argc == 2) {
      len = strnlen(argv[1], VWORD_LEN_MAX);
      if (len < 1) {
        fprintf(stderr, "a vscii word is atleast 1 character\n");
        return 0;
      }
      if (len > VWORD_LEN_MAX) {
        fprintf(stderr, "%d is too long to be a vscii word.\n", len);
        return 0;
      }
      memcpy(word, argv[1], len);
    }
  }
  ret = isvsciiword(word, len);
  if (ret == len) {
    fprintf(stdout, "yes\n");
    return 1;
  }
  fprintf(stdout, "no\n");
  return 0;
}
