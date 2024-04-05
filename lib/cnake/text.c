#ifndef C_TEXT
#define C_TEXT
#endif

/* 8 byte size/offset/length */
#ifndef u64
typedef unsigned long int u64;
#endif
/* 4 byte thing probably a pixel like argb */
#ifndef u32
typedef unsigned int u32;
#endif

/* 1 byte size/offset/length */
#ifndef u8
typedef unsigned char u8;
#endif

/* If the return value == the sz passed in, this buffer is valid utf8 */
/* to be specific the sz passed in and the value returned are the number
 * of bytes not the number of utf8 encoded unicode characters */
u64 text_len(u8 *buf, u64 sz);

/* if return nonzero, the length of the valid word */
/* this wil never be > 78, a word is atleast and no more than a sequence of
 * letters with no more than 2 vowels or 3 consonants in a row and y/w are
 * wildcards (glides)
for reference ASCII:
!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
0123456789
* */

#define WORD_LEN_MIN 1
#define WORD_LEN_MAX 78

u64 word_len(u8 *buf, u64 sz);

int is_aline(u8 c);
int is_aspace(u8 c);
int is_aletter(u8 c);
int is_adigit(u8 c);
int is_uadod(u8 c);
int is_adodad(u8 c);
int is_udohead(u8 c);

int is_avowel(u8 c);
int is_aglide(u8 c);
int is_aconsonant(u8 c);

/*
 * A proper normal sentence is a sequence of alphabetic words no
 * longer than 78, most often less than 26. The final word must
 * be a period, and maybe decorated with exclimation or question
 * marking. All non-alphabetic marks such as emoji, emoticons,
 * dashes, arrows unicode characters or doodles must be translated
 * into alphabetic representations in exploded view.
 *
 * document chain?

 *
 */

const u8 *elements = "H, He, Li, Be, B, C, N, O, F, Ne, Na, Mg, Al, Si, P, S,\
Cl, Ar, K, Ca, Sc, Ti, V, Cr, Mn, Fe, Ni, Co, Cu, Zn, Ga, Ge, As, Se, Br, Kr,\
Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe, Cs, Ba,\
La, Ce, Pr, Nd, Pm, Sm, Eu, Gd, Tb, Dy, Ho, Er, Tm, Yb, Lu, Hf, Ta, W, Re, Os,\
Ir, Pt, Au, Hg, Tl, Pb, Bi, Th, Pa, U, Np, Pu, Am, Cm, Bk, Cf, Es, Fm, Md, No,\
Lr, Rf, Db, Sg, Bh, Hs, Mt, Ds, Rg, Cn, Nh, Fl, Mc, Lv, Ts, Og.";

const u8 *states = "AL, AK, AZ, AR, CA, CO, CT, DE, FL, GA, HI, ID, IL, IN,\
IA, KS, KY, LA, ME,MD, MA, MI, MN, MS, MO, MT, NE, NV, NH, NJ, NM, NY, NC, ND,\
OH, OK, OR, PA, RI, SC, SD, TN, TX, UT, VT, VA, WA, WV, WI, WY";

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
} CC;

#define SP (US + 1)
#define DEL 127

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

#define UNICODES 1112064
#define PHY_C 299792458

#define yeap return 1;
#define nope return 0;
#define otherwise

int is_leap_year(u64 year) {
  if ((year % 400) == 0) yeap
  if ((year % 100) == 0) nope
  if ((year % 4) == 0) yeap
  otherwise nope
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define fubar exit(1);

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

typedef struct {
  size_t sz;
  u64 spaces;
  u64 letters;
  u64 numbers;
  u64 dodads;
  u64 uppercase_letters;
  u64 vowels;
  u64 consonants;
  u64 sylables;
  u64 words;
  u64 n[128];
} text_nfo;

void anal_text(u8 *buf, size_t sz) {
  if (!buf) fubar;
  if (sz < 1) fubar;
  /* Assumptions: We have one line of valid ascii text */
  text_nfo nfo;
  nfo.sz = sz;
  int n = 0;
  for (n = 0; n < sz; n++) {
    char c = buf[n];
    if (c == ' ') {
      nfo.spaces++;
      continue;
    }
    if (isalpha(c)) {
      nfo.letters++;
    } else if (is_adigit(c)) {
      nfo.numbers++;
    }
  }
  printf("Length: %lu\n", nfo.sz);
  printf("Numbers: %lu\n", nfo.numbers);
  printf("Letters: %lu\n", nfo.letters);
  printf("Spaces: %lu\n", nfo.spaces);
  printf("Length: %lu characters\n", nfo.sz);
}

void anal_test() {
  u8 *customer_statement = "\
Let's do that thing with, as an inline example, this silly sentence, the one\
your reading right here starting with Let's and ending soon after here with\
some immediate regrets.";
  anal_text(customer_statement, strlen(customer_statement));
}

/*
13312 - 19893 CJK Ideographs Extension A
19968 - 40869 CJK Ideographs
44032 - 55203 Hangul Syllables
55296 - 56191 Non-Private Use High Surrogates
56192 - 56319 Private Use High Surrogates
56320 - 57343 Low Surrogates
57344 - 63743 The Private Use Area
983040 - 1048573 Private Use
1048576 - 1114109 Private Use*/

u8 is_ablank(u8 byte) {
  if (byte == SP) return 1;
  if (byte == LF) return 1;
  if (byte == CR) return 1;
  if (byte == VT) return 1;
  if (byte == HT) return 1;
  if (byte == FF) return 1;
  return 0;
}

u8 is_ascii_char(u8 byte) {
  if ((byte > SP) && (byte < DEL)) {
    return 1;
  }
  return 0;
}

int is_udohead(u8 byte) {
  if (byte <= 191) return 0;
  if (byte >= 248) return 0;
  if (byte <= 223) return 2;
  if (byte <= 239) return 3;
  if (byte <= 247) return 4;
  return 0;
}

int is_unicode_tail(u8 byte) {
  if ((byte >= 128) && (byte <= 191)) return 1;
  return 0;
}

int is_unicode_neckbeard(u8 head, u8 neck) {
  if (head == 224) {
    if ((neck >= 160) && (neck <= 191)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 237) {
    if ((neck >= 128) && (neck <= 159)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 240) {
    if ((neck >= 144) && (neck <= 191)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 244) {
    if ((neck >= 128) && (neck <= 143)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  return is_unicode_tail(neck);
}

u64 text_len(u8 *buf, u64 sz) {
  u8 byte;
  size_t i;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_aletter(byte)) continue;
    if (is_adigit(byte)) continue;
    if (is_adodad(byte)) continue;
    if (is_ablank(byte)) continue;
    int u = is_udohead(byte);
    if (u < 2) break;
    if ((i + u) > sz) break;
    if (u == 2) {
      if (is_unicode_tail(buf[i + 1])) {
        i += 1;
        continue;
      }
      break;
    } else if (u == 3) {
      if ((is_unicode_neckbeard(byte, buf[i + 1]))
       && (is_unicode_tail(buf[i + 2]))) {
        i += 2;
        continue;
      }
      break;
    } else if (u == 4) {
      if ((is_unicode_neckbeard(byte, buf[i + 1]))
       && (is_unicode_tail(buf[i + 2]))
       && (is_unicode_tail(buf[i + 3]))) {
        i += 3;
        continue;
      }
      break;
    }
  }
  return i;
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

#define ASCII_MARK_MIN (SP + 1)
#define ASCII_MARK_MAX (DEL - 1)
#define ASCII_MARKS = ASCII_MARKS_MAX - ASCII_MARK_MIN

//static const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

u8 is_actl(u8 byte) {
  if (byte == DEL) return 1;
  if ((byte > NUL) && (byte < SP)) return 1;
  return 0;
}

int is_ascii(u8 byte) {
  if ((byte > 0) && (byte < 128)) return 1;
  return 0;
}

u64 blank_len(u8 *buf, u64 sz) {
  u64 i;
  u8 byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_ablank(byte)) continue;
    break;
  }
  return i;
}

char *ascii_dodads = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

#define ASCII_LETTERS 26
#define ASCII_LETTERS_BIG 26
#define ASCII_DIGITS 10
#define ASCII_DODS 32
#define ASCII_CTLS 33
#define NUM_LETTERSORNUMBERS (NUM_LETTERS * NUM_CASES) + NUM_DIGITS

int is_adodad(u8 c) {
  int n;
  char dod;
  for (n = 0; n < ASCII_DODS; n++) {
    dod = ascii_dodads[n];
    if (c == dod) return 1;
  }
  return 0;
}

int is_uadod(u8 c) {
  if (is_adodad(c)) return 1;
  return is_udohead(c);
}

int is_adigit(u8 c) {
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

int number_len(u8 *buf, u64 sz) {
  u64 i;
  u8 byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_adigit(byte)) continue;
    break;
  }
  return i;
}

int is_aletter(u8 c) {
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

int is_avowel(u8 c) {
  if ((c == 'a') || (c == 'A')) return 1;
  if ((c == 'e') || (c == 'E')) return 1;
  if ((c == 'i') || (c == 'I')) return 1;
  if ((c == 'o') || (c == 'O')) return 1;
  if ((c == 'u') || (c == 'U')) return 1;
  return 0;
}

int is_aglide(u8 c) {
  if ((c == 'w') || (c == 'W')) return 1;
  if ((c == 'y') || (c == 'Y')) return 1;
  return 0;
}

int is_aconsonant(u8 c) {
  if ((is_aletter(c)) && (!is_avowel(c))) return 1;
  return 0;
}

int is_aletter_or_anumber(u8 c) {
  if (is_aletter(c)) return 1;
  if (is_adigit(c)) return 1;
  return 0;
}

int is_aalphanumeric(u8 c) {
  return is_aletter_or_anumber(c);
}

int is_aspace(u8 c) {
  if (c == SP) return 1;
  return 0;
}

int is_aline(u8 c) {
  if (c == LF) return 1;
  return 0;
}

int aalphanumeric_len(u8 *buf, u64 sz) {
  u64 i;
  int have_alpha = 0;
  u8 byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_aletter(byte)) {
      have_alpha++;
      continue;
    }
    if (is_adigit(byte)) continue;
    return 0;
  }
  if (have_alpha) return 1;
  return 0;
}

void aletter_loop() {
  for (char letter = 'a'; letter < '{'; letter++) {
    printf("Letter %c\n", letter);
  }
}

int ascii_len(u8 *dat, u64 sz) {
  u8 b;
  u64 n = 0;
  for (n = 0; n < sz; n++) { b = dat[n];
    if (!is_ascii(b)) break;
  }
  return n;
}

/*
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

u64 word_len(u8 *string, u64 len) {
  int i;
  if ((len < WORD_LEN_MIN) || (len > WORD_LEN_MAX)) {
    if (len < WORD_LEN_MIN) {
  if (BUG) {
    fprintf(stderr, "A word is at the least 1 letter.\n");
  }
  return 0;
    } else {
  if (len > WORD_LEN_MAX) {
    if (BUG) {
      fprintf(stderr, "A word is not more than 78 letters.\n");
    }
    return 0;
  }
    }
  }
  for (i = 0; i < len; i++) {
    if (!(is_aletter(string[i]))) {
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
  if (is_aletter(c)) return LETTER;
  else if (is_adigit(c)) return NUMBER;
  else if (is_ablank(c)) return BLANK;
  else if (is_adodad(c)) return DOD;
  else if (is_udohead(c)) return DOD;
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
        ret = is_udohead(c);
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

/*

Here we define an explicit hyper-meta-standard that is first and obstesively
described in a finite length of bytes that are a linear sequence of valid
unicode code points that are encoded per the utf8 system. Those unicode code
points correspond exactly to certain glyphs that are essentially identically
quantized at a certain resolution and thus we attempt to close the loop
by describing a system of drawing the characters of any kind of font via
describing how to decode the binart sfnt format and draw the encoded beizer
curves using a number of methods such as gcode, canvas, mouse movements,
hand movements, etc. Essentially a Rewriter. We further embelish this
mental contrivance train with a virtual memory space that is essentially
linear in addrressing however a portion of this space correlates precicely
to the positions on the surface of a large sphere or torus, (A sphere and
a spindle torus at the limit are almost the same thing so this doesn't matter
for the purposes of this standard which is reconstructive and essentially
hyper-cyclic as it is a hypergraph with hypercycles in conceptual sense.
However in-silico computationally, we are always going to be breaking things
up into well described data types in a tracable sequence of transformations
we call the program.
*/

/*
 *
 *
 * *F{*[]} == u64 *(u8 *buf, u64 sz);
 *
 * */
