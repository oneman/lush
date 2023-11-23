#include <stdio.h>

char c0c1_str[32][4] = {
  "NUL",
  "SOH","STX",
  "ETX","EOT",
  "ENQ","ACK",
  "BEL","BS",
  "HT","LF","VT","FF","CR",
  "SO","SI",
  "DLE",
  "DC1","DC2","DC3","DC4",
  "NAK","SYN",
  "ETB","CAN",
  "EM",
  "SUB","ESC",
  "FS","GS","RS","US"};

typedef enum {
  NUL = 0,
  SOH,STX,
  ETX,EOT,
  ENQ,ACK,
  BEL,BS,
  HT,LF,VT,FF,CR,
  SO,SI,
  DLE,
  DC1,DC2,DC3,DC4,
  NAK,SYN,
  ETB,CAN,
  EM,
  SUB,ESC,
  FS,GS,RS,US
} c0c1_t;

#define C0C1_START 0
#define C0C1_LAST US

#define ASCII_SP 32
#define ASCII_SPACE ASCII_SP
#define ASCII_SP_STR "SP"

#define ASCII_DEL 127
#define ASCII_DEL_STR "DEL"

int main(int argc, char **argv) {
  int i, j;
  for (i = 0; i <= ASCII_DEL; i++) {
    for (j = 7 - 1; j >= 0; j--) {
      putchar(i & (1u << j) ? '1' : '0');
    }
    putchar(ASCII_SP);
    if ((i >= C0C1_START) && (i <= C0C1_LAST)) {
      printf("%s", c0c1_str[i]);
    }
    if (i == ASCII_SP) {
      printf(ASCII_SP_STR);
    }
    if ((i > ASCII_SP) && (i < ASCII_DEL)) {
      putchar(i);
    }
    if (i == ASCII_DEL) {
      printf(ASCII_DEL_STR);
    }
    printf("\n");
  }
  return 0;
}
