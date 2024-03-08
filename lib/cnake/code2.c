/*C

 letters  (2 rep letters max)
 a = any
 e
 i = 1
 o
 u = 2
 syllables
  words
  labels

 checkdef cont-ext context

 h = ?
 went ~ to the partition
 gone: left from the partition
 did -3-
 go -Z-
 going -=-


 well if the least we can know about the universe is 1 bit,
 then the most we can know is a whole bunch of bitches

 /word/seen
 #occurances filenym

 /word/heard
 #occurances filenym

 /word/read
 #occurances filenym


 whiches, whitespace seperated

 'reccurrences with prev/next page'


 filenymsys

 register names:
   sha256 contents
   subject: math, source = mathwords
   "collections"
   datasheet/paper/webpage/photo/level "media presentate type"
*/

#define _STDIOV_H 26
#define _GNU_SOURCE
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

/* !danger includes string.h and locale.h bewares! */
#include <string.h>
#include <locale.h>
#include <sys/uio.h>
#define HIGH 248

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

typedef float    f32;
typedef double   f64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef size_t   usize;
typedef ssize_t  isize;

#include <krad/radio/client.h>

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

!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
0123456789

typedef struct str {
  char *p;
  size_t sz;
}
*/

#define yeap return 1;
#define nope return 0;
#define otherwise

int is_leap_year(uint64_t year) {
  if ((year % 400) == 0) yeap
  if ((year % 100) == 0) nope
  if ((year % 4) == 0) yeap
  otherwise nope
}

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>

  void ft_fail(int ft_errno) {
    printf("Freetype Error: %s\n", FT_Error_String(ft_errno));
    exit(1);
  }

  void *ft_alloc(FT_Memory ft_mem, long sz) {
    void *buf;
    static size_t tb = 0;
    tb += sz;
    printf("Freetype: alloc %ld bytes. [Total %ld]\n", sz, tb);
    buf = malloc(sz);
    memset(buf, 0, sz);
    return buf;
  }

  void ft_free(FT_Memory ft_mem, void *buf) {
    printf("Freetype: free\n");
    free(buf);
  }

  void *ft_realloc(FT_Memory ft_mem, long old_sz, long new_sz, void *buf) {
    void *newbuf;
    printf("Freetype: realloc %ld -> %ld bytes\n", old_sz, new_sz);
    newbuf = malloc(new_sz);
    memset(newbuf, 0, new_sz);
    memcpy(newbuf, buf, old_sz);
    free(buf);
    return newbuf;
  }

void ft_test(uint8_t *buf, size_t sz) {
  int ft_errno = 0;
  FT_Open_Args ft_args;
  FT_Library ft_lib = NULL;
  FT_Face f = NULL;
  struct FT_MemoryRec_ ft_mem;

  memset(&ft_args, 0, sizeof(ft_args));
  memset(&ft_lib, 0, sizeof(ft_lib));
  memset(&f, 0, sizeof(f));

  ft_mem.user = "demo";
  ft_mem.alloc = ft_alloc;
  ft_mem.free = ft_free;
  ft_mem.realloc = ft_realloc;

  ft_errno = FT_New_Library(&ft_mem, &ft_lib);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft new lib\n");

  FT_Add_Default_Modules(ft_lib);
  printf("ft add def mod\n");
  FT_Set_Default_Properties(ft_lib);
  printf("ft set def prop\n");
  ft_args.flags = FT_OPEN_MEMORY;
  ft_args.memory_base = buf;
  ft_args.memory_size = sz;
  printf("ft pre open face: %p \n", ft_lib);
  ft_errno = FT_Open_Face(ft_lib, &ft_args, 0, &f);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft open face\n");

  int pxl_w = 12 * 10;
  int pxl_h = pxl_w;
  int w = pxl_w * 64;
  int h = pxl_h * 64;
  int dpi = 720;

  ft_errno = FT_Set_Char_Size(f, w, h, dpi, dpi);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft set char size\n");

  ft_errno = FT_Set_Pixel_Sizes(f, pxl_w, pxl_h);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft set px size\n");

  uint32_t char_code = 'a';
  ft_errno = FT_Load_Char(f, char_code, FT_LOAD_DEFAULT);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft load char\n");

  ft_errno = FT_Render_Glyph(f->glyph, FT_RENDER_MODE_NORMAL);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft render glyph\n");

  FT_Bitmap pxmap;
  pxmap = f->glyph->bitmap;
  printf("ft bitmap: %u x %u stride: %d grays: %u mode:  %u\n", pxmap.width,
    pxmap.rows, pxmap.pitch, pxmap.num_grays, pxmap.pixel_mode);

  ft_errno = FT_Done_Face(f);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft done face\n");

  ft_errno = FT_Done_Library(ft_lib);
  if (ft_errno) ft_fail(ft_errno);
  printf("ft done lib\n");
}

#include <mupdf/fitz.h>

int doc(int argc, char **argv)
{
	char *input;
	float zoom, rotate;
	int page_number, page_count;
	fz_context *ctx;
	fz_document *doc;
	fz_pixmap *pix;
	fz_matrix ctm;
	int x, y;

	if (argc < 3)
	{
		fprintf(stderr, "usage: example input-file page-number [ zoom [ rotate ] ]\n");
		fprintf(stderr, "\tinput-file: path of PDF, XPS, CBZ or EPUB document to open\n");
		fprintf(stderr, "\tPage numbering starts from one.\n");
		fprintf(stderr, "\tZoom level is in percent (100 percent is 72 dpi).\n");
		fprintf(stderr, "\tRotation is in degrees clockwise.\n");
		return EXIT_FAILURE;
	}

	input = argv[1];
	page_number = atoi(argv[2]) - 1;
	zoom = argc > 3 ? atof(argv[3]) : 100;
	rotate = argc > 4 ? atof(argv[4]) : 0;

	/* Create a context to hold the exception stack and various caches. */
	ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (!ctx)
	{
		fprintf(stderr, "cannot create mupdf context\n");
		return EXIT_FAILURE;
	}

	/* Register the default file types to handle. */
	fz_try(ctx)
		fz_register_document_handlers(ctx);
	fz_catch(ctx)
	{
		//fz_report_error(ctx);
		fprintf(stderr, "cannot register document handlers\n");
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

	/* Open the document. */
	fz_try(ctx)
		doc = fz_open_document(ctx, input);
	fz_catch(ctx)
	{
		//fz_report_error(ctx);
		fprintf(stderr, "cannot open document\n");
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

	/* Count the number of pages. */
	fz_try(ctx)
		page_count = fz_count_pages(ctx, doc);
	fz_catch(ctx)
	{
		//fz_report_error(ctx);
		fprintf(stderr, "cannot count number of pages\n");
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

	if (page_number < 0 || page_number >= page_count)
	{
		fprintf(stderr, "page number out of range: %d (page count %d)\n", page_number + 1, page_count);
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

	/* Compute a transformation matrix for the zoom and rotation desired. */
	/* The default resolution without scaling is 72 dpi. */
	ctm = fz_scale(zoom / 100, zoom / 100);
	ctm = fz_pre_rotate(ctm, rotate);

	/* Render page to an RGB pixmap. */
	fz_try(ctx)
		pix = fz_new_pixmap_from_page_number(ctx, doc, page_number, ctm, fz_device_rgb(ctx), 0);
	fz_catch(ctx)
	{
		//fz_report_error(ctx);
		fprintf(stderr, "cannot render page\n");
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

	/* Print image data in ascii PPM format. */
	printf("P3\n");
	printf("%d %d\n", pix->w, pix->h);
	printf("255\n");
	for (y = 0; y < pix->h; ++y)
	{
		unsigned char *p = &pix->samples[y * pix->stride];
		for (x = 0; x < pix->w; ++x)
		{
			if (x > 0)
				printf("  ");
			printf("%3d %3d %3d", p[0], p[1], p[2]);
			p += pix->n;
		}
		printf("\n");
	}

	/* Clean up. */
	fz_drop_pixmap(ctx, pix);
	fz_drop_document(ctx, doc);
	fz_drop_context(ctx);
	return EXIT_SUCCESS;
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

#define ASCII_MARK_MIN (SP + 1)
#define ASCII_MARK_MAX (DEL - 1)
#define ASCII_MARKS = ASCII_MARKS_MAX - ASCII_MARK_MIN

//static const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

uint8_t is_ascii_control(uint8_t byte) {
  if (byte == DEL) return 1;
  if ((byte > NUL) && (byte < SP)) return 1;
  return 0;
}

uint8_t is_ascii_char(uint8_t byte) {
  if ((byte > SP) && (byte < DEL)) {
    return 1;
  }
  return 0;
}

int is_ascii(unsigned char byte) {
  if ((byte > 0) && (byte < 128)) return 1;
  return 0;
}

/*

E0     A0..BF
ED     80..9F
F0     90..BF
F4     80..8F

224 160-191
237 128-159
240 144-191
244 128-143

*/

int is_unicode_tail(uint8_t byte) {
  if ((byte >= 128) && (byte <= 191)) return 1;
  return 0;
}

int is_unicode_neckbeard(uint8_t head, uint8_t neck) {
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

int is_unicode_head(uint8_t byte) {
  if (byte <= 191) return 0;
  if (byte >= 248) return 0;
  if (byte <= 223) return 2;
  if (byte <= 239) return 3;
  if (byte <= 247) return 4;
  return 0;
}

uint8_t is_ascii_blank(uint8_t byte) {
  if (byte == SP) return 1;
  if (byte == LF) return 1;
  if (byte == CR) return 1;
  if (byte == VT) return 1;
  if (byte == HT) return 1;
  if (byte == FF) return 1;
  return 0;
}

uint8_t is_blank(uint8_t byte) {
  return is_ascii_blank(byte);
}

size_t blank_len(uint8_t *buf, size_t sz) {
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

size_t text_len(uint8_t *buf, size_t sz) {
  uint8_t byte;
  size_t i;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_ascii_char(byte)) continue;
    if (is_ascii_blank(byte)) continue;
    int u = is_unicode_head(byte);
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

void get_adj(points *adj, point *pt, int w, int h) {
  adj->n = 0;
  if (pt->x < 1) return;
  if (pt->y < 1) return;
  if (pt->x > (w - 1)) return;
  if (pt->y > (h - 1)) return;

  adj->pt[adj->n].x = pt->x - 1;
  adj->pt[adj->n++].y = pt->y - 1;

  adj->pt[adj->n].x = pt->x;
  adj->pt[adj->n++].y = pt->y - 1;

  adj->pt[adj->n].x = pt->x + 1;
  adj->pt[adj->n++].y = pt->y - 1;

  adj->pt[adj->n].x = pt->x - 1;
  adj->pt[adj->n++].y = pt->y;

  adj->pt[adj->n].x = pt->x + 1;
  adj->pt[adj->n++].y = pt->y;

  adj->pt[adj->n].x = pt->x - 1;
  adj->pt[adj->n++].y = pt->y + 1;

  adj->pt[adj->n].x = pt->x;
  adj->pt[adj->n++].y = pt->y + 1;

  adj->pt[adj->n].x = pt->x + 1;
  adj->pt[adj->n++].y = pt->y + 1;

  printf("From point %d,%d %d adjacent pixels\n", pt->x, pt->y, adj->n);
  int i;
  for (i = 0; i < adj->n; i++) {
    printf(" adj point: %d,%d\n", adj->pt[i].x, adj->pt[i].y);
  }
}
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

static int pr[4096 * 8];
static int nr = 0;
static int nrj = 0;

int active_regions() {
  return nr - nrj;
}

int pixel_region(int x, int y, int w) {
  int n = (y * w) + x;
  int r = pr[n];
  //printf("n=%d\n", n);
  //printf("get region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_new(int x, int y, int w) {
  int n = (y * w) + x;
  int r = ++nr;
  pr[n] = r;
  //printf("n=%d\n", n);
  //printf("new region: %d,%d: %d\n", x, y, r);
  return r;
}

int pixel_region_join(int r, int x, int y, int w) {
  int n = (y * w) + x;
  int old_r = pr[n];
  pr[n] = r;
  //printf("n=%d\n", n);
  //printf(" join region: %d,%d: %d -> %d\n", x, y, old_r, r);
  if ((old_r) && (old_r != r)) {
    nrj++;
    for (int i = 0; i < n; i++) {
      if (pr[i] == old_r) {
        pr[i] = r;
      //  printf(" %d moved to: %d\n", i, r);
      }
    }
  }
  return r;
}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} pixel_t;

typedef struct {
  int w;
  int h;
} area_t;

void pixel_probe(uint8_t *px, point *pt, pixel_t *p, int w, int pxl_sz) {
  int i;
  i = (pt->x * pxl_sz) + (pt->y * w * pxl_sz);
  p->a = px[i];
  p->r = px[i + 1];
  p->g = px[i + 2];
  p->b = px[i + 3];
}

int rgbcmp(pixel_t *p1, pixel_t *p2) {
  if ((p1->r == p2->r) &&
      (p1->g == p2->g) &&
      (p1->b == p2->b)) {
    return 0;
  }
  return 1;
}

int samergb(pixel_t *p1, pixel_t *p2) {
  if (rgbcmp(p1, p2) == 0) return 1;
  return 0;
}

int argbcmp(pixel_t *a, pixel_t *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

int pixcmp(pixel_t *a, pixel_t *b) {
  if ((a->a == b->a) &&
      (a->r == b->r) &&
      (a->g == b->g) &&
      (a->b == b->b)) {
    return 0;
  }
  return 1;
}

void pixel_trace(uint8_t *px, int x, int y, int w, int h, int pixel_size) {
  points adj;
  point pt;
  pixel_t this_p;
  pixel_t near_p;
  printf(" pixel trace from: %d, %d - %d bytes per pixel\n", x, y, pixel_size);
  pt.x = x;
  pt.y = y;
  pixel_probe(px, &pt, &this_p, w, pixel_size);
  get_adj(&adj, &pt, w, h);
  int i;
  for (i = 0; i < adj.n; i++) {
    pixel_probe(px, &adj.pt[i], &near_p, w, pixel_size);
    if (samergb(&near_p, &this_p)) {
      printf("matching values at %d, %d -> %d, %d\n", x, y,
       adj.pt[i].x, adj.pt[i].y);
    }
  }
}

void print_pixel_regions(uint32_t *px, int w, int h) {
  int x;
  int y;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      printf("%4d ", pixel_region(x, y, w));
    }
    printf("\n");
  }
  printf("\n");
}

void pixel_scan(uint32_t *px, int w, int h) {
  printf("pixel scan: %d x %d - %d bytes per pixel\n", w, h, 4);
  int x;
  int y;
  int r;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      pixel_t *upleft = NULL;
      pixel_t *up = NULL;
      pixel_t *left = NULL;
      pixel_t *upright = NULL;
      pixel_t *cur = &px[(y * w) + x];
      if (x == 0) printf("%d", y);
      printf(" #%02X%02X%02X", cur->r, cur->g, cur->b);
      if (y > 0) {
        up = &px[((y - 1) * w) + x];
        if (x > 0) upleft = &px[((y - 1) * w) + (x - 1)];
        if (x < (w - 1)) upright = &px[((y - 1) * w) + (x + 1)];
      }
      if (x > 0) left = &px[(y * w) + (x - 1)];
      if (upleft) {
        if (samergb(cur, upleft)) {
          r = pixel_region(x - 1, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (up) {
        if (samergb(cur, up)) {
          r = pixel_region(x, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (left) {
        if (samergb(cur, left)) {
          r = pixel_region(x - 1, y, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (upright) {
        if (samergb(cur, upright)) {
          r = pixel_region(x + 1, y - 1, w);
          pixel_region_join(r, x, y, w);
        }
      }
      if (!pixel_region(x, y, w)) {
        pixel_region_new(x, y, w);
      }
    }
    printf("\n");
  }
  printf("\n");
  printf("%d active regions %d created, %d joined\n",
          active_regions(), nr, nrj);
  print_pixel_regions(px, w, h);
}




int mom(int argc, char *argv[]) {
  cairo_surface_t *surface;
  cairo_t *cr;
  time_t seconds;
  seconds = time(NULL);
  surface = NULL;
  cr = NULL;
  char filename[128];
  snprintf(filename, sizeof(filename), "%s/mom_%ld.png", getenv("HOME"),
    seconds);
  int w = 320;
  int h = 320;
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
  cr = cairo_create(surface);
  //unsigned char *px;
  //px = cairo_image_surface_get_data(surface);
  /* paper */
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);
  /* crossword grid */
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 0, 0, 0);
  int i;
  for (i = 0; i < 16; i++) {
    cairo_move_to(cr, i * 20, 0);
    cairo_line_to(cr, i * 20, 320);
    cairo_stroke(cr);
    cairo_move_to(cr, 0, i * 20);
    cairo_line_to(cr, 320, i * 20);
    cairo_stroke(cr);
  }
  cairo_surface_write_to_png(surface, filename);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  return 0;
}

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
  int w = 3840;
  int h = 2160;
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
  cr = cairo_create(surface);
  unsigned char *px;
  px = cairo_image_surface_get_data(surface);

  /* paper */
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  cairo_set_line_width(cr, 0.5);
  cairo_set_source_rgb(cr, 0, 0, 0);
  int i;
  for (i = 0; i < 16; i++) {
    cairo_move_to(cr, i * 20, 0);
    cairo_line_to(cr, i * 20, 320);
    cairo_stroke(cr);
    cairo_move_to(cr, 0, i * 20);
    cairo_line_to(cr, 320, i * 20);
    cairo_stroke(cr);
  }

  // quad rule
  cairo_set_source_rgba(cr, 0.26, 0.26, 0.99, 1);
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

size_t text_scan(uint8_t *buf, size_t sz) {
  text_mode last = BLANK;
  text_mode mode = last;
  int ret;
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

size_t binary_scan(uint8_t *buf, size_t sz) {
  size_t n;
  size_t z;
  for (n = 0; n < sz;) {
    //usleep(10000);
    //printf("n %u sz %u\n", n, sz);
    u8 b = buf[n];
    z = text_len(buf + n, sz - n);
    if (z == 0) { printf("%02X", b); n++; continue; }
    for (int i = 0; z > i; i++) putchar(buf[n] + i);
    n += z;
    continue;
  }
  return sz;
}

size_t data_scan(uint8_t *buf, size_t sz) {
  int n = text_len(buf, sz);
  if (n == sz) return text_scan(buf, sz);
  return binary_scan(buf, sz);
}

int file_scan(char *path) {
  size_t sz;
  uint8_t *data;
  kr_file2 *file;
  kr_file2_info info;
  kr_fs_setup setup;
  kr_file_set *fs;
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
    printf("Process: %s\n", path);
    data_scan(data, info.sz);
  }
  return kr_file_set_destroy(fs);
}

int path_scan() {
  int ret;
  int opt;
  FTS *fs;
  FTSENT *n;
  opt = FTS_PHYSICAL;
  char * const paths[] = {
    /* "/home", */
    "/run/media",
    NULL
  };
  fs = fts_open(paths, opt, NULL);
  if (!fs) exit(1);
  for (;;) {
    n = fts_read(fs);
    if (!n) break;
    if (n->fts_info == FTS_F) {
      /* n->fts_name */
      printf("%s\n", n->fts_path);
      file_scan(n->fts_path);
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
  //int stride;
  //stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 5);
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
  if ((letter == 'x') || (letter == 'X')) return "xray";
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

void pixel_file_scan(char *filename) {
  cairo_surface_t *surface;
  cairo_t *cr;
  int w;
  int h;
  uint32_t *argb_px;
  printf("pixel file scan:\n%s\n\n", filename);
  surface = cairo_image_surface_create_from_png(filename);
  w = cairo_image_surface_get_width(surface);
  h = cairo_image_surface_get_height(surface);
  cr = cairo_create(surface);
  argb_px = cairo_image_surface_get_data(surface);
  pixel_scan(argb_px, w, h);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

/*
! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
! " #   % & ' ( ) * + , - . / : ; < = > ?   [ \ ] ^ _   { | } ~
      X                                   X           X
*/

int oldmain(int argc, char *argv[]) {
  int err;
  struct stat s;
  if (argc > 1) {
    err = stat(argv[1], &s);
    if (!err) {
      /*if (S_ISDIR(s.st_mode)) {*/
      if (S_ISREG(s.st_mode)) {
        file_scan(argv[1]);
        return 0;
      }
    }
  }
  //superuser();
  err = path_scan();
  return err;
}

void insert_coin_to_continue() {
  char c;
  printf("Insert Coin To Continue!$# ");
  scanf("%c", &c);
  printf("\nOK %c\n", c);
}

int wayland_dis(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_WAYLAND;
  sprintf(nfo.wl.display_name, "wayland-1");
  nfo.wl.state = KR_WL_CONNECTED;
  ret = kr_xpdr_make(client, "oneway", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_WAYLAND_OUT;
  nfo.wl_out.width = 1920;
  nfo.wl_out.height = 1080;
  nfo.wl_out.fullscreen = 0;
  ret = kr_xpdr_make(client, "oneway/onewin", &nfo);

  return ret;
}

int jack_cap(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK;
  sprintf(nfo.jack.client_name, "demo");
  sprintf(nfo.jack.server_name, "");
  nfo.jack.sample_rate = 48000;
  nfo.jack.period_size = 1024;
  ret = kr_xpdr_make(client, "jackpipe", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK_IN;
  sprintf(nfo.jack_in.name, "master");
  nfo.jack_in.channels = 2;
  nfo.jack_in.direction = KR_JACK_INPUT;
  ret = kr_xpdr_make(client, "jackpipe/master", &nfo);

  return ret;
}

int v4l2_cap(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_V4L2;
  nfo.v4l2.dev = 0;
  nfo.v4l2.priority = 0;
  ret = kr_xpdr_make(client, "v4l2", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_V4L2_IN ;
  nfo.v4l2_in.width = 1920;
  nfo.v4l2_in.height = 1080;
  nfo.v4l2_in.num = 60;
  nfo.v4l2_in.den = 1;
  ret = kr_xpdr_make(client, "v4l2/cam", &nfo);

  return ret;

}

int rig(int argc, char *argv[]) {
  int ret;
  kr_client *client;
  char *sysname = "demo";
  client = kr_client_create("demo cmdr");
  if(client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  ret = jack_cap(client);
  ret = v4l2_cap(client);
 // ret = wayland_dis(client);
  kr_client_destroy(&client);
  return ret;
}
