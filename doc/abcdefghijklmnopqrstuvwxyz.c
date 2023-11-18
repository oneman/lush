const char *abcdefghijklmnopqrstuvwxyz = "abcdefghijklmnopqrstuvwxyz";

/*
7 bits
128 patterns
95 printable
1 space
94 marking
26 letters
10 numbers

26 * 2 + 10 = 62 aphanumbers

26 letters
10 numbers
32 doodadas
94 markers
1 space

filter to letters only
filters to letters + numbers only
filters to printable only

32 doodads w/e

48 = 0
58 = 9
65 = A
89 = Z
96 = a
122 = z


*/

/* "lolbytes '97-'22, greatest hits ever" */


#include <stdio.h>

void wtfb(unsigned char c) {
  if (!((c >= 32) && (c <= 126))) return;

  int n = (int)c;
//  printf("byte %d is %c\n", n, c);
/*
  32-47

  32 is the space

  32 nothings
  32 = space
  15 somethings 33-48
  48 = 0
  10 numbers
  7 somethings 58-64
  26 letters
  6 somethings 91-96
  26 letters
  4 somethings 123-128
34 somethings
*/


  if (n < 48) {
    printf("%d/16 well %d is %c\n", n - 32, n, c);
  }
  if ((n >= 58) && (n < 65)) {
    printf("bot %c is %d\n", c, n);
  }
  if ((n > 64 + 26) && (n < 97)) {
    printf("%d %c\n", n, c);
  }
  if (n > 96 + 26) {
    printf("----%d %c\n", n, c);
  }
}

 int bi(int num) {
     printf("%d bi?\n", num);
     int bits = 1;
     int states = 2;
     while (num > states) {
       bits++;
       states *= 2;
     }
     printf("%d is a %d bit number\n", num, bits);
     return bits;

   }

int main(int c, char **g) {
int lettersoff = 96;

  // 7bit code for letters and stuff
  // 5 bits are needed for 26 letters

  int mask = 0b10000000;
  int bits = 2;

  int startoff = 32;
  int asciicharcount = 96;
  int sections = 3;
  int end = 128;
  int cases = 2;
  int letters = 26;
  int numbers = 10;
  int bit = 0b01;
  int byte = 8;
  int chars = end - startoff;
  int letterchars = cases * letters;
  int otherchars = chars - letterchars - numbers;

  int n = 0;
  int b = 0;
  n = bits;

  bi(7);
  bi(26);
  bi(10);
  bi(4096);
  bi(5000);
  bi(50000);
  bi(500000);
  bi(115000);
  bi(1500000);
  bi(50000000);
  bi(5000000000);
  bi(500000000);

  int wb = 26;
  while (wb < 128) { wb++; wtfb(wb); }

  return 0;
}
