#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <time.h>
#include <fts.h>
#include <krad/io/file2.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE ((26 * 26) + 26)
#endif
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/uio.h>

#define HIGH 248

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

#include <krad/radio/client.h>

#include "font.c"
#include "doc.c"
#include <cairo/cairo.h>
#include "pix.c"
#include "text.c"

#define yeap return 1;
#define nope return 0;
#define otherwise

int is_leap_year(u64 year) {
  if ((year % 400) == 0) yeap
  if ((year % 100) == 0) nope
  if ((year % 4) == 0) yeap
  otherwise nope
}

#define TO_LOW 0
#define TO_HIGH 248

typedef enum {
  LOW1 = TO_LOW
} bytes_to_low;

typedef enum {
  HIGH1 = TO_HIGH, /* 248 0xF8 */
  HIGH2,           /* 249 0xF9 */
  HIGH3,           /* 250 0xFA */
  HIGH4,           /* 251 0xFB */
  HIGH5,           /* 252 0xFC */
  HIGH6,           /* 253 0xFD */
  HIGH7,           /* 254 0xFE */
  HIGH8            /* 255 0xFF */
} bytes_to_high;

char ascii_cc_str[32][4] = {
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

#define SP_STR "SP"
#define DEL_STR "DEL"

#define ASCII_MARK_MIN (SP + 1)
#define ASCII_MARK_MAX (DEL - 1)
#define ASCII_MARKS = ASCII_MARKS_MAX - ASCII_MARK_MIN

//static const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

u8 is_ascii_control(u8 byte) {
  if (byte == DEL) return 1;
  if ((byte > NUL) && (byte < SP)) return 1;
  return 0;
}

int is_ascii(u8 byte) {
  if ((byte > 0) && (byte < 128)) return 1;
  return 0;
}

u8 is_blank(u8 byte) {
  return is_ascii_blank(byte);
}

size_t blank_len(u8 *buf, size_t sz) {
  size_t i;
  uint8_t byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_blank(byte)) continue;
    break;
  }
  return i;
}

char *ascii_dods = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

#define ASCII_LETTERS 26
#define ASCII_LETTERS_BIG 26
#define ASCII_DIGITS 10
#define ASCII_DODS 32
#define ASCII_CTLS 33
#define NUM_LETTERSORNUMBERS (NUM_LETTERS * NUM_CASES) + NUM_DIGITS

int is_ascii_dod(uint8_t c) {
  int n;
  char dod;
  for (n = 0; n < ASCII_DODS; n++) {
    dod = ascii_dods[n];
    if (c == dod) return 1;
  }
  return 0;
}

int is_dod(uint8_t c) {
  if (is_ascii_dod(c)) return 1;
  return is_unicode_head(c);
}

int is_ascii_number(uint8_t c) {
  if (c == '0') return 1;
  if (c == '1') return 1;
  if (c == '2') return 1;
  if (c == '3') return 1;
  if (c == '4') return 1;
  if (c == '5') return 1;
  if (c == '6') return 1;
  if (c == '7') return 1;
  if (c == '8') return 1;
  if (c == '9') return 1;
  return 0;
}

int is_number(uint8_t c) {
  return is_ascii_number(c);
}

int number_len(uint8_t *buf, size_t sz) {
  size_t i;
  uint8_t byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_number(byte)) continue;
    break;
  }
  return i;
}

uint8_t is_english_alphabet_letter(uint8_t c) {
  if (c == 'A') return 1;
  if (c == 'B') return 1;
  if (c == 'C') return 1;
  if (c == 'D') return 1;
  if (c == 'E') return 1;
  if (c == 'F') return 1;
  if (c == 'G') return 1;
  if (c == 'H') return 1;
  if (c == 'I') return 1;
  if (c == 'J') return 1;
  if (c == 'K') return 1;
  if (c == 'L') return 1;
  if (c == 'M') return 1;
  if (c == 'N') return 1;
  if (c == 'O') return 1;
  if (c == 'P') return 1;
  if (c == 'Q') return 1;
  if (c == 'R') return 1;
  if (c == 'S') return 1;
  if (c == 'T') return 1;
  if (c == 'U') return 1;
  if (c == 'V') return 1;
  if (c == 'W') return 1;
  if (c == 'X') return 1;
  if (c == 'Y') return 1;
  if (c == 'Z') return 1;
  if (c == 'a') return 1;
  if (c == 'b') return 1;
  if (c == 'c') return 1;
  if (c == 'd') return 1;
  if (c == 'e') return 1;
  if (c == 'f') return 1;
  if (c == 'g') return 1;
  if (c == 'h') return 1;
  if (c == 'i') return 1;
  if (c == 'j') return 1;
  if (c == 'k') return 1;
  if (c == 'l') return 1;
  if (c == 'm') return 1;
  if (c == 'n') return 1;
  if (c == 'o') return 1;
  if (c == 'p') return 1;
  if (c == 'q') return 1;
  if (c == 'r') return 1;
  if (c == 's') return 1;
  if (c == 't') return 1;
  if (c == 'u') return 1;
  if (c == 'v') return 1;
  if (c == 'w') return 1;
  if (c == 'x') return 1;
  if (c == 'y') return 1;
  if (c == 'z') return 1;
  return 0;
}

int is_ascii_letter(uint8_t c) {
  return is_english_alphabet_letter(c);
}

int is_a_letter(uint8_t c) {
  return is_ascii_letter(c);
}

int is_letter(uint8_t c) {
  return is_a_letter(c);
}

int is_letter_or_number(uint8_t c) {
  if (is_letter(c)) return 1;
  if (is_number(c)) return 1;
  return 0;
}

int is_alphanumeric(uint8_t c) {
  return is_letter_or_number(c);
}

int is_ascii_space(uint8_t c) {
  if (c == SP) return 1;
  return 0;
}

int is_ascii_line(uint8_t c) {
  if (c == LF) return 1;
  return 0;
}

int alphanumeric_len(uint8_t *buf, size_t sz) {
  size_t i;
  int have_alpha = 0;
  uint8_t byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_letter(byte)) {
      have_alpha++;
      continue;
    }
    if (is_number(byte)) continue;
    return 0;
  }
  if (have_alpha) return 1;
  return 0;
}

#define ℤ int
#define cunt size_t
#define csz size_t
#define byte uint8_t

ℤ ascii_len(byte *dat, cunt sz) {
  byte b;
  ℤ n = 0;
  for (n = 0; n < sz; n++) { b = dat[n];
    if (!is_ascii(b)) break;
  }
  return n;
}

/* An english alphabet letter is 5 bits exactly */
#define VSCII_CHAR_BITS 5
#define VSCII_ASCII_CHAR_BITS 7
#define VSCII_ANSI_CHAR_BITS 5

/* There is 26 english alphabet letters and they are the lowercase NATO
 * alphabet, * otherwise known as the american or latin alphabet or
 * western alphabet. */

#define VSCII_CHARACTER_COUNT 26
#define VSCII_CHARS_NUM VSCII_CHARACTER_COUNT
#define VSCII_NUM_CHARS VSCII_CHARACTER_COUNT
#define VSCII_LETTER_NUM VSCII_CHARS_NUM
#define VSCII_ALPHABET_SZ VSCII_LETTER_NUM

/* An american "word" or "word" or "a single standard normal a.f. no cap */

#define VWORD_LEN_MIN 1
#define VWORD_MOST_LEN_LESSTHAN (26 - 10)
#define VWORD_LEN_MAX 26 * 3
#define VSCII_WORD_LEN_MIN VWORD_LEN_MIN
#define VSCII_WORD_LEN_MAX VWORD_LEN_MAX
#define VSCII_WORD_LENGTH_MINIMUM VWORD_LEN_MIN
#define VSCII_WORD_LENGTH_MAXIMUM VWORD_LEN_MAX

#define VSCII_WORD_TYPE "morphonym"

/* ALL american words are no longer than (26 * 3) 78 letters in length,
 * because we use 80 column terminals here in Virginia */

#define VLINE VWORD_LEN_MAX

/* Most US words that are longer than ceil((phi * 26)) (16) or rather most
 * other US words, are actually caticombinations of US words */

#define VWORD_26_CHARACTER_EXAMPLE "radioimmunoelectrophoresis"

/* "acrylonitrile-butadiene-styrene" is not a US word for example, it is
 * however a dictionary word that is 31 characters long, two of the characters
 * are dashes which is ascii, thus "acrylonitrile-butadiene-styrene" is a 5
 * word US sequence/vector in the US-ascii "context sense" "combomode"
 *
 * 1: acrylonitrile [length 13]
 * 2: dash [length 4]
 * 3: butadiene [length 9]
 * 4: dash [length 4]
 * 5: styrene [length 7]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*  gcc cairo_sandbox.c -o cairo_sandbox `pkg-config --libs --cflags cairo`
*/

#define letterpaper_quarter_inch_ptx 18
#define letterpaper_half_inch_ptx 36
#define letterpaper_inch_ptx 72
#define letterpaper_height_ptx 792
#define letterpaper_width_ptx 612
#define letterpaper_gov_margin 72
#define letterpaper_nonmarginal_width 468 /* perm sym */
#define letterpaper_nonmarginal_height 648 /* perm sym */
/* 648 / 468 = 1.384615 */
#define letterpaper_nonmarginal_quad_ruled_columns 26
#define letterpaper_nonmarginal_quad_ruled_rows 36
/* 36 / 26 = 1.384615 */
#define lpm letterpaper_gov_margin
#define gs_diam 26 * 18
#define gs_rad 13 * 18
#define gs_center lpm + gs_rad

#define crossword_sz "4 and 7/16in wide"

/* content area: 6.5 * 9 = 58.5 in 484704 */
/* letterpaper area: 58.5 + 35 = 93.5 in */
/* letterpaper total quanta = 93.5 * 5184 */
/* margin area: (11 * 2) + (13 * 1) = 35in */
/* margin quanta: 5184 * 35 = 181440 */
/* margin overlapping quanta: 5184 * 4 = 20736 */
/* margin nonoverlapping quanta: 160704 */
/* margin nonoverlapping wide quanta: 67392 */
/* margin nonoverlapping tall quanta: 93312 */
/* margin nonoverlapping ratio: 93312/67392 = 1.384615 */
/* dollar size: 6.2 * 2.6 */

typedef struct {
  int x;
  int y;
} point;

typedef struct {
  point pt[8];
  int n;
} points;

/* screen: control panel, screen */
/* base2 is 1bit */
/* base4 is 2bit */
/* base8 is 3bit */
/* base10 is 4bit */
/* base16 is 4bit */
/* base26 is 5bit */
/* base32 is 5bit */
/* base36 is 6bit */
/* base64 is 6bit */
/* base128 is 7bit */
/* base256 is 8bit */
/* base1k is 9bit */
/* base64k is 16bit */
/* base24m is 24bit */
/* base4b is 32bit */
/* base9qq is 64bit */
/* base1hq is 64bit */
/* binary is base2 */
/* byte is base256 */
/* letter is base26 */
/* ascii is base128 */
/* dpad is base4 */
/* dpad2 is base8 */
/* number is base10 */
/* hex is base16 */
/* a pointer points to a byte series */
/*

0x2020 can be considered as a 0x0a
two space can be considered a newline

type 0x/1x seperation byte: line
type 2x seperation byte: space/dod

type x28 and x29: target encirculation device
type x5B and x5D: target bracketing device
type x7B and x7D: target bracing block signals

type 5C: backward newline
type 2F: path component seperation device

code terminal:
  w  85 char
        5 chars status
        80 chars code
  h  28 line
        26 lines code
        1 line status
        1 line command

code document:
  w  78 char
  h  9999 lines

us letter pixel document:
  w 468
  h 648

desktop terminal screen video:
  s 1
  w 1024
  h 768
  z 85

*/
/*
DODIS:
 HT
 LF
 SP
 FS { }
 GS ( )
    < >
 RS [  ]
 US "  "
  SUS ',./:;|
   OP !#$%&*+-=?@\^_`~
*/
/*

base64 in ascii
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

0 0110000
1 0110001
2 0110010
3 0110011
4 0110100
5 0110101
6 0110110
7 0110111
8 0111000
9 0111001

0x3 0 0110000
0x3 1 0110001
0x3 2 0110010
0x3 3 0110011
0x3 4 0110100
0x3 5 0110101
0x3 6 0110110
0x3 7 0110111
0x3 8 0111000
0x3 9 0111001

0x30 0110000
0x31 0110001
0x32 0110010
0x33 0110011
0x34 0110100
0x35 0110101
0x36 0110110
0x37 0110111
0x38 0111000
0x39 0111001


00 NUL    10 DLE    20 SP   30 0    40 @    50 P    60 `    70 p
01 SOH    11 DC1    21 !    31 1    41 A    51 Q    61 a    71 q
02 STX    12 DC2    22 "    32 2    42 B    52 R    62 b    72 r
03 ETX    13 DC3    23 #    33 3    43 C    53 S    63 c    73 s
04 EOT    14 DC4    24 $    34 4    44 D    54 T    64 d    74 t
05 ENQ    15 NAK    25 %    35 5    45 E    55 U    65 e    75 u
06 ACK    16 SYN    26 &    36 6    46 F    56 V    66 f    76 v
07 BEL    17 ETB    27 '    37 7    47 G    57 W    67 g    77 w
08 BS     18 CAN    28 (    38 8    48 H    58 X    68 h    78 x
09 HT     19 EM     29 )    39 9    49 I    59 Y    69 i    79 y
0A LF     1A SUB    2A *    3A :    4A J    5A Z    6A j    7A z
0B VT     1B ESC    2B +    3B ;    4B K    5B [    6B k    7B {
0C FF     1C FS     2C ,    3C <    4C L    5C \    6C l    7C |
0D CR     1D GS     2D -    3D =    4D M    5D ]    6D m    7D }
0E SO     1E RS     2E .    3E >    4E N    5E ^    6E n    7E ~
0F SI     1F US     2F /    3F ?    4F O    5F _    6F o    7F DEL

*/

#define BUG 1

uint8_t word_len(char *string, size_t len) {
  int i;
  if ((len < VWORD_LEN_MIN) || (len > VWORD_LEN_MAX)) {
    if (len < VWORD_LEN_MIN) {
  if (BUG) {
    fprintf(stderr, "A word is at the least 1 letter.\n");
  }
  return 0;
    } else {
  if (len > VWORD_LEN_MAX) {
    if (BUG) {
      fprintf(stderr, "A word is not more than 78 letters.\n");
    }
    return 0;
  }
    }
  }
  for (i = 0; i < len; i++) {
    if (!(is_letter(string[i]))) {
  if (BUG) {
    fprintf(stderr, "Character %d, %c is not a letter.\n", i,
      string[i]);
    return 0;
  }
    }
  }
  return len;
}

typedef enum {
  NO,
  BLANK,
  DOD,
  NUMBER,
  LETTER
} text_mode;

text_mode get_text_mode(u8 c) {
  if (is_letter(c)) return LETTER;
  else if (is_number(c)) return NUMBER;
  else if (is_blank(c)) return BLANK;
  else if (is_dod(c)) return DOD;
  return NO;
}

size_t text_scan(u8 *buf, size_t sz) {
  text_mode last = BLANK;
  text_mode mode = last;
  int ret;
  /*printf("Got text len: %lu.\n", sz);*/
  return sz;
  for (size_t i = 0; i < sz; i++) {
    u8 c = buf[i];
    mode = get_text_mode(c);
    switch (mode) {
      case BLANK:
        printf("BLANK\n");
        if (c == SP) {
          printf("space");
        }
        if (c == LF) {
          printf("newline");
        }
        if (c == HT) {
          printf("tab");
        }
        break;
      case DOD:
        printf("DOD\n");
        ret = is_unicode_head(c);
        if (ret > 1) {
          i += (ret - 1);
          continue;
        }
        break;
      case NUMBER:
        printf("NUMBER\n");
        break;
      case LETTER:
        printf("LETTER\n");
        break;
      default:
        printf("NOTEXT\n");
        exit(1);
    }
    last = mode;
  }
  return sz;
}

#include "data.c"
#include "files.c"

void superuser() {
  if (setreuid(0, 0) + setregid(0, 0)) {
    fprintf(stderr, "FAIL: need be superuser\n");
    exit(1);
  }
}


/*
char *has(char *bs, int len, char *a, int sz) {
}
*/

void letter_loop() {
  for (char letter = 'a'; letter < '{'; letter++) {
    printf("Letter %c\n", letter);
  }
}

#include <cairo/cairo.h>

char otan(char *buf, size_t sz) {
  if (buf == NULL) exit(1);
  if (sz < 4) exit(1);
  if (sz > 8) exit(1);
  if (sz == 4) {
    if (memcmp("echo", buf, 4) == 0) return 'e';
    if (memcmp("golf", buf, 4) == 0) return 'g';
    if (memcmp("kilo", buf, 4) == 0) return 'k';
    if (memcmp("lima", buf, 4) == 0) return 'l';
    if (memcmp("mike", buf, 4) == 0) return 'm';
    if (memcmp("papa", buf, 4) == 0) return 'p';
    if (memcmp("xray", buf, 4) == 0) return 'x';
    if (memcmp("zulu", buf, 4) == 0) return 'z';
  }
  if (sz == 5) {
    if (memcmp("alpha", buf, 5) == 0) return 'a';
    if (memcmp("bravo", buf, 5) == 0) return 'b';
    if (memcmp("delta", buf, 5) == 0) return 'd';
    if (memcmp("hotel", buf, 5) == 0) return 'h';
    if (memcmp("india", buf, 5) == 0) return 'i';
    if (memcmp("oscar", buf, 5) == 0) return 'o';
    if (memcmp("romeo", buf, 5) == 0) return 'r';
    if (memcmp("tango", buf, 5) == 0) return 't';
  }
  if (sz == 6) {
    if (memcmp("quebec", buf, 6) == 0) return 'q';
    if (memcmp("sierra", buf, 6) == 0) return 's';
    if (memcmp("victor", buf, 6) == 0) return 'v';
    if (memcmp("yankee", buf, 6) == 0) return 'y';
  }
  if (sz == 7) {
    if (memcmp("juliett", buf, 6) == 0) return 'j';
    if (memcmp("charlie", buf, 7) == 0) return 'c';
    if (memcmp("foxtrot", buf, 7) == 0) return 'f';
    if (memcmp("uniform", buf, 7) == 0) return 'u';
    if (memcmp("whiskey", buf, 7) == 0) return 'w';
  }
  if (sz == 8) {
    if (memcmp("november", buf, 8) == 0) return 'n';
  }
  exit(1);
  return '0';
}

void vscii_say_bits() {
  int i, j;
  for (i = 1; i < 3*3*3; i++) {
    printf("%c ", i + 2 * 2 * 2 * 2 * 2 * 3);
    for (j = 5 - 1; j >= 0; j--) {
      int bit = i & (1u << j);
      if (bit == 0) {
        printf("zero");
      }
      if (bit != 0) {
        printf("one");
      }
      /*putchar(i & (1u << j) ? '1' : '0');*/
      if (j > 0) printf(" ");
    }
    printf("\n");
  }
}

u8 word_digit(char *buf, size_t sz) {
  if (buf == NULL) exit(1);
  if (sz < 3) exit(1);
  if (sz > 5) exit(1);
  if (sz == 3) {
    if (memcmp("one", buf, 3) == 0) return 1;
    if (memcmp("two", buf, 3) == 0) return 2;
    if (memcmp("six", buf, 3) == 0) return 6;
  }
  if (sz == 4) {
    if (memcmp("zero", buf, 4) == 0) return 0;
    if (memcmp("four", buf, 4) == 0) return 4;
    if (memcmp("five", buf, 4) == 0) return 5;
    if (memcmp("nine", buf, 4) == 0) return 9;
  }
  if (sz == 5) {
    if (memcmp("three", buf, 5) == 0) return 3;
    if (memcmp("seven", buf, 5) == 0) return 7;
    if (memcmp("eight", buf, 5) == 0) return 8;
  }
  exit(1);
}

char *digit_word(u8 d) {
  if (d > 9) exit(1);
  if (d == 0) { return("zero"); }
  if (d == 1) { return("one"); }
  if (d == 2) { return("two"); }
  if (d == 3) { return("three"); }
  if (d == 4) { return("four"); }
  if (d == 5) { return("five"); }
  if (d == 6) { return("six"); }
  if (d == 7) { return("seven"); }
  if (d == 8) { return("eight"); }
  if (d == 9) { return("nine"); }
  exit(1);
}

char *nato(char letter) {
  if ((letter == 'a') || (letter == 'A')) return "alpha";
  if ((letter == 'b') || (letter == 'B')) return "bravo";
  if ((letter == 'c') || (letter == 'C')) return "charlie";
  if ((letter == 'd') || (letter == 'D')) return "delta";
  if ((letter == 'e') || (letter == 'E')) return "echo";
  if ((letter == 'f') || (letter == 'F')) return "foxtrot";
  if ((letter == 'g') || (letter == 'G')) return "golf";
  if ((letter == 'h') || (letter == 'H')) return "hotel";
  if ((letter == 'i') || (letter == 'I')) return "india";
  if ((letter == 'j') || (letter == 'J')) return "juliett";
  if ((letter == 'k') || (letter == 'K')) return "kilo";
  if ((letter == 'l') || (letter == 'L')) return "lima";
  if ((letter == 'm') || (letter == 'M')) return "mike";
  if ((letter == 'n') || (letter == 'N')) return "november";
  if ((letter == 'o') || (letter == 'O')) return "oscar";
  if ((letter == 'p') || (letter == 'P')) return "papa";
  if ((letter == 'q') || (letter == 'Q')) return "quebec";
  if ((letter == 'r') || (letter == 'R')) return "romeo";
  if ((letter == 's') || (letter == 'S')) return "sierra";
  if ((letter == 't') || (letter == 'T')) return "tango";
  if ((letter == 'u') || (letter == 'U')) return "uniform";
  if ((letter == 'v') || (letter == 'V')) return "victor";
  if ((letter == 'w') || (letter == 'W')) return "whiskey";
  if ((letter == 'x') || (letter == 'X')) return "xray";
  if ((letter == 'y') || (letter == 'Y')) return "yankee";
  if ((letter == 'z') || (letter == 'Z')) return "zulu";
  return "";
}

#define 🔺 continue;

void bfun() {
  /* based named */
  int n;
  for (n = 0; n < 256; n++) {
  printf("\n%*d ", 3, n);
  if (n == 0) { printf("null"); 🔺 }
  if (n == SOH) { printf("incoming"); 🔺 }
  if (n == STX) { printf("transmitting"); 🔺 }
  if (n == ETX) { printf("over"); 🔺 }
  if (n == EOT) { printf("out"); 🔺 }
  if (n == ENQ) { printf("request"); 🔺 }
  if (n == ACK) { printf("response"); 🔺 }
  if (n == BEL) { printf("bullshit"); 🔺 }
  if (n == BS) { printf("backstep"); 🔺 }
  if (n == SO) { printf("alien"); 🔺 }
  if (n == SI) { printf("ascii"); 🔺 }
  if (n == DLE) { printf("secret"); 🔺 }
  if (n == DC1) { printf("aux1"); 🔺 }
  if (n == DC2) { printf("aux2"); 🔺 }
  if (n == DC3) { printf("aux3"); 🔺 }
  if (n == DC4) { printf("aux4"); 🔺 }
  if (n == NAK) { printf("no"); 🔺 }
  if (n == SYN) { printf("signal"); 🔺 }
  if (n == ETB) { printf("nocap"); 🔺 }
  if (n == CAN) { printf("cancel"); 🔺 }
  if (n == EM) { printf("aether"); 🔺 }
  if (n == SUB) { printf("whatever"); 🔺 }
  if (n == ESC) { printf("escape"); 🔺 }
  if (n == FS) { printf("filefile"); 🔺 }
  if (n == GS) { printf("groupgroup"); 🔺 }
  if (n == RS) { printf("recordrecord"); 🔺 }
  if (n == US) { printf("unitunit"); 🔺 }
  if (n == VT) { printf("next"); 🔺 }
  if (n == SP) { printf("space"); 🔺 }
  if (n == LF) { printf("newline"); 🔺 }
  if (n == HT) { printf("tab"); 🔺 }
  if (n == FF) { printf("newpage"); 🔺 }
  if (n == CR) { printf("return"); 🔺 }
  if (n < 33) { printf("%s", ascii_cc_str[n]); 🔺 }
  if (n == 127) { printf("del"); 🔺 }
  if (n == ' ') { printf("space"); 🔺 }
  if (n == '!') { printf("fuck"); 🔺 }
  if (n == '"') { printf("quote"); 🔺 }
  if (n == '#') { printf("hashtag"); 🔺 }
  if (n == '$') { printf("dollarsign"); 🔺 }
  if (n == '%') { printf("percent"); 🔺 }
  if (n == '&') { printf("and"); 🔺 }
  if (n == '\'') { printf("singlequote"); 🔺 }
  if (n == '(') { printf("openparen"); 🔺 }
  if (n == ')') { printf("closeparen"); 🔺 }
  if (n == '*') { printf("star"); 🔺 }
  if (n == '+') { printf("plus"); 🔺 }
  if (n == ',') { printf("comma"); 🔺 }
  if (n == '-') { printf("dash"); 🔺 }
  if (n == '.') { printf("dot"); 🔺 }
  if (n == '/') { printf("slash"); 🔺 }
  if (n == ':') { printf("colon"); 🔺 }
  if (n == ';') { printf("semicolon"); 🔺 }
  if (n == '<') { printf("lesserthan"); 🔺 }
  if (n == '=') { printf("equalsign"); 🔺 }
  if (n == '>') { printf("greaterthan"); 🔺 }
  if (n == '?') { printf("questionmark"); 🔺 }
  if (n == '@') { printf("at"); 🔺 }
  if (n == '[') { printf("openbracket"); 🔺 }
  if (n == '\\') { printf("backslash"); 🔺 }
  if (n == ']') { printf("closebracket"); 🔺 }
  if (n == '^') { printf("uparrow"); 🔺 }
  if (n == '_') { printf("underline"); 🔺 }
  if (n == '`') { printf("backtick"); 🔺 }
  if (n == '{') { printf("openbrace"); 🔺 }
  if (n == '|') { printf("pipe"); 🔺 }
  if (n == '}') { printf("closebrace"); 🔺 }
  if (n == '~') { printf("tilde"); 🔺 }
  if (n == '0') { printf("zero"); 🔺 }
  if (n == '1') { printf("one"); 🔺 }
  if (n == '2') { printf("two"); 🔺 }
  if (n == '3') { printf("three"); 🔺 }
  if (n == '4') { printf("four"); 🔺 }
  if (n == '5') { printf("five"); 🔺 }
  if (n == '6') { printf("six"); 🔺 }
  if (n == '7') { printf("seven"); 🔺 }
  if (n == '8') { printf("eight"); 🔺 }
  if (n == '9') { printf("nine"); 🔺 }
  if (n < 127) { printf("%s", nato(n)); 🔺 }
  if  ((n >= 128) && (n <= 191)) { printf("utf-8 6bit follower"); 🔺 }
  if  ((n >= 192) && (n <= 223)) { printf("utf-8 one of two"); 🔺 }
  if  ((n >= 224) && (n <= 239)) { printf("utf-8 first of three"); 🔺 }
  if ((n >= 240) && (n <= 247)) { printf("utf-8: one of four"); 🔺 }
  if (n >= HIGH) { printf("high"); }
  }
  printf("\n");
}

int is_regular_file(char *path) {
  int err;
  struct stat s;
  err = stat(path, &s);
  if (err) return 0;
  if (S_ISREG(s.st_mode)) return 1;
  return 0;
}

/*

unsigned char *np = "H, He, Li, Be, B, C, N, O, F, Ne, Na, Mg, Al, Si, P, S,
Cl, Ar, K, Ca, Sc, Ti, V, Cr, Mn, Fe, Ni, Co, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe, Cs, Ba,
La, Ce, Pr, Nd, Pm, Sm, Eu, Gd, Tb, Dy, Ho, Er, Tm, Yb, Lu, Hf, Ta, W, Re, Os,
Ir, Pt, Au, Hg, Tl, Pb, Bi, Th, Pa, U, Np, Pu, Am, Cm, Bk, Cf, Es, Fm, Md, No,
Lr, Rf, Db, Sg, Bh, Hs, Mt, Ds, Rg, Cn, Nh, Fl, Mc, Lv, Ts, Og.";

AL, AK, AZ, AR, CA, CO, CT, DE, FL, GA, HI, ID, IL, IN, IA, KS, KY, LA, ME,
MD, MA, MI, MN, MS, MO, MT, NE, NV, NH, NJ, NM, NY, NC, ND, OH, OK, OR, PA,
RI, SC, SD, TN, TX, UT, VT, VA, WA, WV, WI, WY

!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
0123456789

*/
