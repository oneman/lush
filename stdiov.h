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

#endif
