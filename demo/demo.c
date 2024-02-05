/* the goal of the present file is to encode a multi-modal capability to 
rebase the expression string response function by expressing its data type
specimines in certain prefered forms. All symbologies are dependent on the
arrow line diagram, that is a bunch of arrows drawn on a plane sheet, or a 2d
vector. 

We want to connect the following data types: binarray, vscii, ascii, unicode,
int, and rasterized vector pixel buffer.

"pointers to small pixmaps are regions of a larger pixmap"

One always has from any sequence point, a system carrat pointer. That is
a pointer from an outside system to the inner systems kernelspace and userspace.

the unicode pointer to pixmap is an embodiement of the alien ring structure from
iut thery. 

A unicode string with a newline character in it...

The ascii character set includes two arrows pointing next > or < previous in its
sequence < and >.

It can be seen that every subsequence of graphical characters seperated by
more rather than less space implies a certain set of arrows for certain common
interpretation of there graphomorphic processing.

These words all go together.
> <These><words><all><go><together> <

It is most notable that the above string is only a mutation on the graphical
characters that do not have single character vscii reconstructable
representations. A line requires two points. A line implies arrows in regards
to its sitationship status.

vscii characters are global timeline operators.

Decoding Measurement of Authoritarian Alignment Results in Monty
Python Triality 

*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
/* there is no string.h, except for this superstring.h */
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <time.h>
#include <krad/io/file2.h>

#ifndef _STDIOV_H
#define _STDIOV_H 26

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE ((26 * 26) + 26)
#endif

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/* !danger includes string.h and locale.h bewares! */
#include <string.h>
#include <locale.h>

/* a more useful include */
#include <sys/uio.h>

/*

char *png = 0x89504E470D0A1A0A;

unsigned char *np = "H, He, Li, Be, B, C, N, O, F, Ne, Na, Mg, Al, Si, P, S,
Cl, Ar, K, Ca, Sc, Ti, V, Cr, Mn, Fe, Ni, Co, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe, Cs, Ba,
La, Ce, Pr, Nd, Pm, Sm, Eu, Gd, Tb, Dy, Ho, Er, Tm, Yb, Lu, Hf, Ta, W, Re, Os,
Ir, Pt, Au, Hg, Tl, Pb, Bi, Th, Pa, U, Np, Pu, Am, Cm, Bk, Cf, Es, Fm, Md, No,
Lr, Rf, Db, Sg, Bh, Hs, Mt, Ds, Rg, Cn, Nh, Fl, Mc, Lv, Ts, Og.";

AL, AK, AZ, AR, CA, CO, CT, DE, FL, GA, HI, ID, IL, IN, IA, KS, KY, LA, ME,
MD, MA, MI, MN, MS, MO, MT, NE, NV, NH, NJ, NM, NY, NC, ND, OH, OK, OR, PA,
RI, SC, SD, TN, TX, UT, VT, VA, WA, WV, WI, WY

11111111111111111111111111111111111111111111111111111111111111111111111111111
1        26                78

an as at be by do go he id if in is it me my no of on or so to up us we yo

us standard marginal quantum code page 8.5" x 11" 

local library virginia terminization


if (b & 0xF0 == 0xF0) { }
leads4 = 0xF0 240
leads3 = 0xE0 224
leads2 = 0xC0 192
leads1 = 0x80 128
01010101 = 85
10101010 = 170


11111111111111111111111111111111111111111111111111111111111111111111111111111

if (b & 0xF0 == 0xF0) { }
leads4 = 0xF0 240
leads3 = 0xE0 224
leads2 = 0xC0 192
follow1 = 0x80 128
01010101 = 85
10101010 = 170

! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
 
typedef struct str {
  char *p;
  size_t sz;
} 
*/

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

#define CC_START 0
#define CC_LAST US

#define SP (US + 1)
#define SP_STR "SP"

#define DEL 127
#define DEL_STR "DEL"

#define ASCII_TEXT_MIN (SP + 1)
#define ASCII_TEXT_MAX (DEL - 1)
#define UTF8_TEXT_MIN (DEL + 1)

static const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

uint8_t isascii_cc(uint8_t byte) {
  if ((byte >= NUL) && (byte <= US)) return 1;
  if (byte == DEL) return 1;
  return 0;
}

uint8_t isascii_text(uint8_t byte) {
  if ((byte >= ASCII_TEXT_MIN) && (byte <= ASCII_TEXT_MAX)) {
    return 1;
  }
  return 0;
}

uint8_t isascii_splf(uint8_t byte) {
  if (byte == SP) return 1;
  if (byte == LF) return 1;
  return 0;
}

uint8_t isvscii(uint8_t c) {
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

/* 26 sporatic groups vscii contains tits group? */
#define VSCII_REBUG 27

/* A vscii character is 5 bits exactly */
#define VSCII_CHAR_BITS 5
#define VSCII_ASCII_CHAR_BITS 7
#define VSCII_ANSI_CHAR_BITS 5

/* There is 26 vscii characters and they are the lowercase NATO alphabet,
 * otherwise known as the american or latin alphabet or western alphabet. */

#define VSCII_CHARACTER_COUNT 26
#define VSCII_CHARS_NUM VSCII_CHARACTER_COUNT
#define VSCII_NUM_CHARS VSCII_CHARACTER_COUNT
#define VSCII_LETTER_NUM VSCII_CHARS_NUM
#define VSCII_ALPHABET_SZ VSCII_LETTER_NUM

/* A vscii "vword" or "word" or "a single standard normal a.f. word no cap */

#define VWORD_LEN_MIN 1
#define VWORD_MOST_LEN_LESSTHAN (26 - 10)
#define VWORD_LEN_MAX 26 * 3
#define VSCII_WORD_LEN_MIN VWORD_LEN_MIN
#define VSCII_WORD_LEN_MAX VWORD_LEN_MAX
#define VSCII_WORD_LENGTH_MINIMUM VWORD_LEN_MIN
#define VSCII_WORD_LENGTH_MAXIMUM VWORD_LEN_MAX

#define VSCII_WORD_TYPE "morphonym"

/* ALL vscii words are no longer than (26 * 3) 78 characters in length, because
 * we use 80 column terminals here in Virginia */

#define VLINE VWORD_LEN_MAX

/* Most vscii words that are longer than ceil((phi * 26)) (16) or rather most
 * other vscii words, are actually caticombinations of vscii words */

#define VWORD_26_CHARACTER_EXAMPLE "radioimmunoelectrophoresis"

/* "acrylonitrile-butadiene-styrene" is not a vscii word for example, it is
 * however a dictionary word that is 31 characters long, two of the characters
 * are dashes which is ascii, thus "acrylonitrile-butadiene-styrene" is a 5
 * word vscii sequence/vector in the vscii-ascii "context sense" "combomode"
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
#include <cairo/cairo.h>

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

int tryman(int argc, char *argv[]) {
  cairo_surface_t *surface;
  cairo_t *cr;
  time_t seconds;
  seconds = time(NULL);
  surface = NULL;
  cr = NULL;
  char filename[128];
  snprintf(filename, sizeof(filename), "%s/lush_%ld.png", getenv("HOME"),
    seconds);
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 4096 * 2, 4096 * 2);
  cr = cairo_create(surface);
  unsigned char *px;
  px = cairo_image_surface_get_data(surface);

  /* white paper */
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  cairo_set_line_width(cr, 0.5);
  cairo_set_source_rgba(cr, 0.26, 0.26, 0.99, 1);

  // quad rule

  int c = 1;
  int r = 1;
  for (c = 1; c < 34; c++) {
    cairo_move_to(cr, c * 18, 0);
    cairo_line_to(cr, c * 18, 792);
    cairo_stroke(cr);
  }
  for (r = 1; r < 44; r++) {
    cairo_move_to(cr, 0, r * 18);
    cairo_line_to(cr, 612, r * 18);
    cairo_stroke(cr);
  }

  /* golden margin */
  cairo_set_line_width(cr, 2);
  cairo_set_source_rgba(cr, 1.0, 0.83, 0, 1);
  cairo_move_to(cr, 72, 72);
  cairo_line_to(cr, 540, 72);
  cairo_line_to(cr, 540, 720);
  cairo_line_to(cr, 72, 720);
  cairo_close_path(cr);
  cairo_stroke(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1.0, 0.84, 0.13, 0.71);
  cairo_rectangle(cr, 72, 72, 18 * 26, 18 * 26);
  cairo_fill(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1.0, 0.13, 0.66, 0.2);
  cairo_rectangle(cr, 72, 18 * 30, 18 * 16, 18 * 10);
  cairo_fill(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1.0, 0.53, 0.66, 0.4);
  cairo_rectangle(cr, (4 * 18) + (13 * 18), (4 * 18),
          (13 * 18), (5 * 18));
  cairo_fill(cr);
  
  /*
  cairo_new_path(cr);
  cairo_set_line_width (cr, 2.6);
  cairo_set_source_rgba(cr, 0.1, 0.3, 0.2, 1);
  cairo_arc (cr, gs_center, gs_center, gs_rad, 0, (3.1 / 180.0) * 360.0);
  cairo_stroke (cr);
  cairo_fill(cr);


  cairo_new_path(cr);
  cairo_set_line_width (cr, 2.6);
  cairo_set_source_rgba(cr, 1, 0.3, 0.2, 1);
  cairo_arc (cr, gs_center, gs_center, gs_rad, (3.1415926 / 180.0) * 90, (3.1415926 / 180.0) * 150.0);
  double x, y = 0;
  cairo_get_current_point(cr, &x, &y);
  printf("x,y: %f,%f\n", x, y); 
  cairo_stroke (cr);
  cairo_fill(cr);

  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 1, 0.1, 0.1, 1);
  cairo_arc(cr, x, y, 9, 0, 3.1415926 / 180.0 * 360);
  cairo_fill(cr);


  cairo_new_path(cr);
  cairo_set_line_width (cr, 1.6);
  cairo_set_source_rgba(cr, 0.3, 0.2, 0.1, 1);
  cairo_move_to(cr, x, y);
  cairo_line_to(cr, letterpaper_width_ptx - x, y);
  cairo_line_to(cr, letterpaper_width_ptx/2, letterpaper_gov_margin);
  cairo_close_path(cr);
  cairo_stroke (cr);


  cairo_new_path(cr);
  cairo_set_line_width (cr, 1.6);
  cairo_set_source_rgba(cr, 0.3, 0.6, 0.6, 1);
  cairo_move_to(cr, x, y);
  cairo_line_to(cr, letterpaper_width_ptx/2, letterpaper_gov_margin + (26 * 18));
  cairo_line_to(cr, letterpaper_width_ptx -x, y);

    
  cairo_close_path(cr);
  cairo_stroke (cr);


  cairo_new_path(cr);
  cairo_set_line_width (cr, 2.6);
  cairo_set_source_rgba(cr, 0.3, 0.5, 0.4, 1);
  cairo_arc (cr, gs_center, gs_center + gs_rad, gs_rad,
   0,
   (3.1415926 / 180.0) * 270.0);
  cairo_stroke (cr);
  cairo_fill(cr);

  cairo_new_path(cr);
  cairo_set_line_width (cr, 2.6);
  cairo_set_source_rgba(cr, 0.7, 0.2, 0.6, 1);
  cairo_arc (cr, gs_center, gs_center + gs_rad, gs_rad,
   (3.1415926 / 30.0) * 70.0, 0);
  cairo_stroke (cr);
  cairo_fill(cr);
  */

  cairo_surface_write_to_png(surface, filename);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  return 0;
}

uint8_t isvsciiword(char *string, size_t len) {
  int i;
  if ((len < VWORD_LEN_MIN) || (len > VWORD_LEN_MAX)) {
    if (len < VWORD_LEN_MIN) {
  if (VSCII_REBUG) {
    fprintf(stderr, "A vscii word is at the least 1 vscii character.\n");
  }
  return 0;
    } else {
  if (len > VWORD_LEN_MAX) {
    if (VSCII_REBUG) {
      fprintf(stderr, "A vscii word is not more than 78 vscii chars.\n");
    }
    return 0;
  }
    }
  }
  for (i = 0; i < len; i++) {
    if (!(isvscii(string[i]))) {
  if (VSCII_REBUG) {
    fprintf(stderr, "Character %d, %c is not a vscii character.\n", i,
      string[i]);
    return 0;
  }
    }
  }
  return len;
}

#endif

void comcut(uint8_t *bs, size_t sz) {
  size_t i;
  i = 0;
  printf("  comp len %lu: ", sz);
  for (i = 0; i < sz; i++) {
    printf("%c", bs[i]);
  }
  printf("\n");
}

void crosscut(uint8_t *bs, size_t sz) {
  size_t i;
  size_t start;
  size_t len;
  i = 0;
  start = 0;
  len = 0;
  printf(" line len %lu\n", sz);
  for (i = 0; i < sz; i++) {
    len = i - start;
    if (bs[i] == '/') {
  if (i == 0) {
    start = i + 1;
    continue;
  }
  if (!len) {
    /*printf("nothing!\n");*/
    start = i + 1;
    continue;
  }
  comcut(bs + start, len);
  start = i + 1;
  continue;
    }
  }
  len = i - start;
  if (len) comcut(bs + start, len);
  printf("\n");
}

void deepcut(uint8_t *bs, size_t sz) {
  size_t i;
  size_t start;
  size_t len;
  i = 0;
  start = 0;
  len = 0;
  printf("len %lu\n", sz);
  for (i = 0; i < sz; i++) {
    if (bs[i] == LF) {
  len = i - start;
  if (len) crosscut(bs + start, len);
  start = i + 1;
    }
  }
}

int find_files() {
  int ret;
  int opt;
  FTS *fs;
  FTSENT *n;
  opt = FTS_PHYSICAL;
  char * const paths[] = {"/home", NULL};
  fs = fts_open(paths, opt, NULL);
  if (!fs) exit(1);
  for (;;) {
    n = fts_read(fs);
    if (!n) break;
    if (n->fts_info == FTS_F) {
  /*n->fts_name*/
  printf("%s\n", n->fts_path);
    }
  }
  ret = fts_close(fs);
  return ret;
}

void superuser() {
  if (setreuid(0, 0) + setregid(0, 0)) {
    fprintf(stderr, "FAIL: need be superuser\n");
    exit(1);
  }
}

int process(int argc, char *argv[]) {
  char *path;
  size_t sz;
  uint8_t *data;
  kr_file2 *file;
  kr_file2_info info;
  kr_fs_setup setup;
  kr_file_set *fs;
  path = argv[1];
  sz = strlen(path);
  if (!kr_file2_exists(path, sz)) {
    printf("Doesn't exist %s\n", path);
    return 1;
  }
  setup.nfiles = 1;
  setup.user = NULL;
  setup.event_cb = NULL;
  fs = kr_file_set_create(&setup);
  if (!fs) {
    printf("this fs didn't create\n");
    return 1;
  }
  file = kr_file2_open(fs, path, sz);
  if (!file) {
    printf("Can't open %s\n", path);
    return kr_file_set_destroy(fs);
  }
  data = kr_file2_get_data(file);
  if (data && !kr_file2_get_info(file, &info)) {
    printf("teardown %s\n", path);
    deepcut(data, info.sz);
  }
  return kr_file_set_destroy(fs);
}


/*
char *has(char *bs, int len, char *a, int sz) {
}
*/

#include <cairo/cairo.h>
void print_pixmap(char *filename, char c) {

  cairo_surface_t *surface;
  cairo_t *cr;
  int width;
  int height;
  int x;
  int y;
  
  width = 5;
  height = 5;

  surface = cairo_image_surface_create_from_png(filename);
  cr = cairo_create(surface);

  int i;
  int stride;
  stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 5);
    
  unsigned char *pixmap;
  pixmap = cairo_image_surface_get_data(surface);
  
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
  int v;
  v = pixmap[y * 20 + (x * 4)];
  if (v > 0) v = 1;
  if (!v) { printf("%c", c); } else { printf(" "); }
    }
  } 
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
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
  if ((letter == 'j') || (letter == 'J')) return "juliet";
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
  if ((letter == 'x') || (letter == 'X')) return "x-ray";
  if ((letter == 'y') || (letter == 'Y')) return "yankee";
  if ((letter == 'z') || (letter == 'Z')) return "zulu"; 
  return "";
}

void dothis() {
  char letter = 'a';
  char filename[256];
  memset(filename, 0, sizeof(filename));
  letter = 'a';
  int i = 1;
  int j;
  while (letter < '{') {
    /*printf("%c", letter);*/
    for (j = 5 - 1; j >= 0; j--) {
  putchar(i & (1u << j) ? '1' : '0');
    }
    printf(" ");
    sprintf(filename, "/home/demo/src/lush/doc/1a2b3c/vscii_%c_5x5.png",
     letter);
     print_pixmap(filename, letter);
     printf(" %s\n", nato(letter));
    letter++;
    i++;
  }
}

/*
00001  aaa    a  aaa  a a  aaa  alpha
00010  b    b    bbb  b b  bbb  bravo
00011     cc   c    cc  charlie
00100    d    d  ddd  d d  ddd  delta
00101  eee  e e  eee  e    eee  echo
00110  fff  f    ff   f    f    foxtrot
00111 gggg g    g gg g  g gggg  golf
01000  h    h    hhh  h h  h h  hotel
01001   i    i    i    i    i   india
01010    j    j    j  j j  jjj  juliet
01011  k    k    k k  kk   k k  kilo
01100  l    l    l    l    lll  lima
01101       mmmmmm m mm m m mike
01110    nnn  n n  n n  november
01111  oo  o  o o  o  oo    oscar
10000  ppp  p p  ppp  p    p    papa
10001 qqq  q q  qqq    q    qq  quebec
10010   rrr  r    r    r    romeo
10011   ss  s    sss    s  ss   sierra
10100   t  ttttt  t    t    t   tango
10101    u u  u u  uuu  uniform
10110       v   v v v   v   victor
10111        w w ww w w whiskey
11000 x   x x x   x   x x x   x x-ray
11001 y   y y y   y   y   y     yankee
11010  zzz    z   z   z    zzz  zulu
*/

#define 🔺 continue;
#define byte unsigned char

void bfun() {
  /* based named */
  int n;
  for (n = 0; n < 256; n++) {
  printf("\n%*d ", 3, n);
  if (n == 0) { printf("bin zero"); 🔺 }
  if (n == SP) { printf("ascii space"); 🔺 }
  if (n == LF) { printf("ascii line"); 🔺 }
  if (n == HT) { printf("ascii tab"); 🔺 }
  if (n == VT) { printf("ascii virtual %s", "VT"); 🔺 }
  if (n == FF) { printf("ascii fuck %s", "FF"); 🔺 }
  if (n == CR) { printf("ascii return %s", "CR"); 🔺 }
  if (n < 33) { printf("ascii %s", ascii_cc_str[n]); 🔺 }
  if (n == 127) { printf("ascii DEL"); 🔺 }
  if (n < 33) { printf("ascii ctl %s", ascii_cc_str[n]); 🔺 }
  if (n == ' ') { printf("space"); 🔺 }
  if (n == '!') { printf("fuck"); 🔺 }
  if (n == '"') { printf("quote"); 🔺 }  
  if (n == '#') { printf("hash"); 🔺 }
  if (n == '$') { printf("dollar"); 🔺 }
  if (n == '%') { printf("percent"); 🔺 }
  if (n == '&') { printf("and"); 🔺 }
  if (n == '\'') { printf("mark"); 🔺 }
  if (n == '(') { printf("open"); 🔺 }
  if (n == ')') { printf("close"); 🔺 }
  if (n == '*') { printf("star"); 🔺 }
  if (n == '+') { printf("plus"); 🔺 }
  if (n == ',') { printf("comma"); 🔺 }
  if (n == '-') { printf("dash"); 🔺 }
  if (n == '.') { printf("dot"); 🔺 }
  if (n == '/') { printf("slash"); 🔺 }
  if (n == ':') { printf("with"); 🔺 }
  if (n == ';') { printf("end"); 🔺 }
  if (n == '<') { printf("less"); 🔺 }
  if (n == '=') { printf("same"); 🔺 }
  if (n == '>') { printf("more"); 🔺 }
  if (n == '?') { printf("what"); 🔺 }
  if (n == '@') { printf("at"); 🔺 }
  if (n == '[') { printf("in"); 🔺 }
  if (n == '\\') { printf("back"); 🔺 }
  if (n == ']') { printf("out"); 🔺 }
  if (n == '^') { printf("raise"); 🔺 }
  if (n == '_') { printf("under"); 🔺 }
  if (n == '`') { printf("tic"); 🔺 }
  if (n == '{') { printf("do"); 🔺 }
  if (n == '|') { printf("or"); 🔺 }
  if (n == '}') { printf("done"); 🔺 }
  if (n == '~') { printf("like"); 🔺 }
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
  if  ((n >= 128) && (n <= 191)) { printf("unico follower"); 🔺 }
  if  ((n >= 192) && (n <= 223)) { printf("unico one of two"); 🔺 }
  if  ((n >= 224) && (n <= 239)) { printf("unico first of three"); 🔺 }
  if ((n >= 240) && (n <= 247)) { printf("unico first of four"); 🔺 }
  if (n >= 248) { printf("bin high"); }
  }
  printf("\n");
}

void superfun(int argc, char *argv[]) {
  bfun();
}

int main(int argc, char *argv[]) {
  superfun(argc, argv);
  superuser();
  if (argc == 1) return find_files();
  return process(argc, argv);
}
