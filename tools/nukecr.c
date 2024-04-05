#include <krad/app/debug.h>
#include <krad/cnake/header.h>

#include <krad/cnake/text.c>

int main(int argc, char *argv[]) {
  unsigned char byte[1];
  for (;;) {
    int ret = read(STDIN_FILENO, byte, 1);
    if (ret < 0) return 1;
    if (ret == 0) return 0;
    if (byte[0] == CR) continue;
    ret = write(STDOUT_FILENO, byte, 1);
    if (ret != 1) exit(1);
  }
  return 1;
}
