#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
int main() {
  int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY, O_NDELAY);
  if (fd < 0)
  {
    perror("open");
    return -1;
  }
  struct termios tty;
  if (tcgetattr(fd, &tty) != 0)
  {
    perror("tcgetattr");
    return -1;
  }
  // set baud rate to 115200
  cfsetospeed(&tty, B115200);
  cfsetispeed(&tty, B115200);
  // apply settings to serial port
  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    perror("tcsetattr");
    return -1;
  }

  char byte;
  while (read(fd, &byte, 1)) putchar(byte);

  close(fd);
  return 0;
}
