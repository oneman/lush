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

#define WORD_LEN_MAX 78

/* If the return value == the sz passed in, this buffer is valid utf8 */
/* to be specific the sz passed in and the value returned are the number
 * of bytes not the number of utf8 encoded unicode characters */
u64 text_len(u8 *buf, u64 sz);
u64 line_len(u8 *buf, u64 sz);
u64 word_len(u8 *buf, u64 sz);

int a_line(u8 c);
int a_space(u8 c);
int a_letter(u8 c);
int a_digit(u8 c);
int is_uadod(u8 c);
int a_dodad(u8 c);
int u_dohead(u8 c);

int a_vowel(u8 c);
int a_glide(u8 c);
int a_consonant(u8 c);

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

char otan(char *buf, u64 sz) {
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

u8 word_digit(char *buf, u64 sz) {
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

u8 a_blank(u8 byte) {
  if (byte == SP) return 1;
  if (byte == LF) return 1;
  if (byte == CR) return 1;
  if (byte == VT) return 1;
  if (byte == HT) return 1;
  if (byte == FF) return 1;
  return 0;
}

u8 a_achar(u8 byte) {
  if ((byte > SP) && (byte < DEL)) {
    return 1;
  }
  return 0;
}

int u_dohead(u8 byte) {
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

u8 a_ctlcode(u8 byte) {
  if (byte == DEL) return 1;
  if ((byte > NUL) && (byte < SP)) return 1;
  return 0;
}

int a_ascii(u8 byte) {
  if ((byte > 0) && (byte < 128)) return 1;
  return 0;
}

u64 blank_len(u8 *buf, u64 sz) {
  u64 i;
  u8 byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (a_blank(byte)) continue;
    break;
  }
  return i;
}

static char *ascii_dodads = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
#define ASCII_NDODADS 32

int a_dodad(u8 c) {
  int n;
  char dod;
  for (n = 0; n < ASCII_NDODADS; n++) {
    dod = ascii_dodads[n];
    if (c == dod) return 1;
  }
  return 0;
}

int is_uadod(u8 c) {
  if (a_dodad(c)) return 1;
  return u_dohead(c);
}

int is_hexdigit(u8 c) {
  if (c == 'A') return 1;
  if (c == 'B') return 1;
  if (c == 'C') return 1;
  if (c == 'D') return 1;
  if (c == 'E') return 1;
  if (c == 'F') return 1;
  if (c == 'a') return 1;
  if (c == 'b') return 1;
  if (c == 'c') return 1;
  if (c == 'd') return 1;
  if (c == 'e') return 1;
  if (c == 'f') return 1;
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

int a_digit(u8 c) {
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
    if (a_digit(byte)) continue;
    break;
  }
  return i;
}

int a_letter(u8 c) {
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

int a_vowel(u8 c) {
  if ((c == 'a') || (c == 'A')) return 1;
  if ((c == 'e') || (c == 'E')) return 1;
  if ((c == 'i') || (c == 'I')) return 1;
  if ((c == 'o') || (c == 'O')) return 1;
  if ((c == 'u') || (c == 'U')) return 1;
  return 0;
}

int a_glide(u8 c) {
  if ((c == 'w') || (c == 'W')) return 1;
  if ((c == 'y') || (c == 'Y')) return 1;
  return 0;
}

int a_consonant(u8 c) {
  if ((a_letter(c)) && (!a_vowel(c))) return 1;
  return 0;
}

int a_letter_or_a_number(u8 c) {
  if (a_letter(c)) return 1;
  if (a_digit(c)) return 1;
  return 0;
}

int a_alphanumeric(u8 c) {
  return a_letter_or_a_number(c);
}

int a_space(u8 c) {
  if (c == SP) return 1;
  return 0;
}

int a_line(u8 c) {
  if (c == LF) return 1;
  return 0;
}

int aalphanumeric_len(u8 *buf, u64 sz) {
  u64 i;
  int have_alpha = 0;
  u8 byte;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (a_letter(byte)) {
      have_alpha++;
      continue;
    }
    if (a_digit(byte)) continue;
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
    if (!a_ascii(b)) break;
  }
  return n;
}

u64 word_len(u8 *buf, u64 sz) {
  if (!buf || (sz < 1) || (sz > WORD_LEN_MAX)) return 0;
  int i;
  char letter;
  int sillyness = 0;
  int srsly = 0;
  for (i = 0; i < sz; i++) {
    if (!a_letter(buf[i])) break;
    letter = buf[i];
    if ((a_vowel(letter)) || (a_glide(letter))) { sillyness++; srsly = 0; }
    if (a_consonant(letter)) { srsly++; sillyness = 0; }
    if ((sillyness > 4) || (srsly > 4)) { return i - 4; }
  }
  return i;
}

u64 line_len(u8 *buf, u64 sz) {
  int n;
  if (sz > 4096) sz = 4096;
  for (n = 0; n < sz; n++) {
    if (buf[n] == LF) break;
    if (buf[n] == NUL) break;
  }
  return n;
}

u64 text_len(u8 *buf, u64 sz) {
  u8 byte;
  u64 i;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (a_letter(byte)) continue;
    if (a_digit(byte)) continue;
    if (a_dodad(byte)) continue;
    if (a_blank(byte)) continue;
    int u = u_dohead(byte);
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

typedef struct {
  u64 sz;
  u64 dodads;
  u64 udodads;
  u64 digits;
  u64 letters;
  u64 words;
  u64 substrings;
  u64 nchar[256];
} text_nfo;

u64 text_scan(u8 *buf, u64 sz) {
  if (!buf) fubar;
  if (sz < 1) fubar;
  text_nfo nfo;
  memset(&nfo, 0, sizeof(nfo));
  nfo.sz = sz;
  int n = 0;
  char prev = 0;
  for (n = 0; n < sz; n++) {
    char c = buf[n];
    nfo.nchar[c]++;
    if (a_letter(c)) nfo.letters++;
    if (a_digit(c)) nfo.digits++;
    if (a_dodad(c)) nfo.dodads++;
    if ((a_blank(prev)) && (!a_blank(c))) nfo.substrings++;
    prev = c;
  }
  printf("Text, %lu bytes\n", nfo.sz);
}

void testext() {
  u8 *customer_statement = "\
Let's do that thing with, as an inline example, this silly sentence, the one\
your reading right here starting with Let's and ending soon after here with\
some immediate regrets.";
  text_scan(customer_statement, strlen(customer_statement));
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
