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

#define SP (US + 1)
#define SP_STR "SP"

#define DEL 127
#define DEL_STR "DEL"

#define ASCII_TEXT_MIN (SP + 1)
#define ASCII_TEXT_MAX (DEL - 1)
#define UTF8_TEXT_MIN (DEL + 1)

const char *nato_alphabet(void) {
  static const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
  return alphabet;
}

uint8_t isascii_cc(uint8_t byte) {
  if ((byte >= NUL) && (byte <= US)) return 1;
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

uint8_t isvscii_nocap(uint8_t c) {
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

uint8_t isvscii(uint8_t c) {
  return isvscii_nocap(c);
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


/*
 * UTF-8 sequence.
 * 11000010-11011111 C2-DF start of 2-byte sequence
 * 11100000-11101111 E0-EF start of 3-byte sequence
 * 11110000-11110100 F0-F4 start of 4-byte sequence
 */

#include <sys/types.h>

#include <stdlib.h>

static const wchar_t utf8_modifier_table[] = {
	0x1F1E6,
	0x1F1E7,
	0x1F1E8,
	0x1F1E9,
	0x1F1EA,
	0x1F1EB,
	0x1F1EC,
	0x1F1ED,
	0x1F1EE,
	0x1F1EF,
	0x1F1F0,
	0x1F1F1,
	0x1F1F2,
	0x1F1F3,
	0x1F1F4,
	0x1F1F5,
	0x1F1F6,
	0x1F1F7,
	0x1F1F8,
	0x1F1F9,
	0x1F1FA,
	0x1F1FB,
	0x1F1FC,
	0x1F1FD,
	0x1F1FE,
	0x1F1FF,
	0x1F3FB,
	0x1F3FC,
	0x1F3FD,
	0x1F3FE,
	0x1F3FF
};

/* Has this got a zero width joiner at the end? */
int
utf8_has_zwj(const struct utf8_data *ud)
{
	if (ud->size < 3)
		return (0);
	return (memcmp(ud->data + ud->size - 3, "\342\200\215", 3) == 0);
}

/* Is this a zero width joiner? */
int
utf8_is_zwj(const struct utf8_data *ud)
{
	if (ud->size != 3)
		return (0);
	return (memcmp(ud->data, "\342\200\215", 3) == 0);
}

/* Is this a variation selector? */
int
utf8_is_vs(const struct utf8_data *ud)
{
	if (ud->size != 3)
		return (0);
	return (memcmp(ud->data, "\357\270\217", 3) == 0);
}

/* Is this in the modifier table? */
int
utf8_is_modifier(const struct utf8_data *ud)
{
	wchar_t	wc;

	if (utf8_towc(ud, &wc) != UTF8_DONE)
		return (0);
	if (!utf8_in_table(wc, utf8_modifier_table,
	    nitems(utf8_modifier_table)))
		return (0);
	return (1);
}


int utf8(u_char ch)
{
	memset(ud, 0, sizeof *ud);
	if (ch >= 0xc2 && ch <= 0xdf)
		ud->size = 2;
	else if (ch >= 0xe0 && ch <= 0xef)
		ud->size = 3;
	else if (ch >= 0xf0 && ch <= 0xf4)
		ud->size = 4;
	else
		return (UTF8_ERROR);
	utf8_append(ud, ch);
	return (UTF8_MORE);
}



#endif
